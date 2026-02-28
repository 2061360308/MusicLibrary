#ifdef _WIN32
#define __declspec(dllexport)
#endif

// #include "tool.h"
#include "quickjs-libc.h"
#include "quickjs.h"
#include <stdio.h>
#include <inttypes.h>
#include "tinycthread.h"
#include "kugou_music_api.h"
#include "js_bundle.h"

#ifdef _WIN32
#define strdup _strdup
#endif

// 存储编译后的字节码，方便多次创建上下文时直接加载执行
uint32_t kugou_music_api_bundle_size = 0;
uint8_t *kugou_music_api_bundle = NULL;

// 声明在 http.c 中实现的模块初始化函数
JSModuleDef *js_init_module_http(JSContext *ctx, const char *module_name);

JSRuntime *rt;

ProcessEnv globalEnv;

typedef struct
{
  JSContext **list;
  size_t size;
  size_t cap;
  mtx_t lock; // tinycthread互斥锁
} CtxList;

CtxList ctxList = {
    NULL,
    0,
    0,
};

static void ctxList_init()
{
  mtx_init(&ctxList.lock, mtx_plain);
}

static void ctxList_add(JSContext *ctx)
{
  mtx_lock(&ctxList.lock);
  if (ctxList.size == ctxList.cap)
  {
    size_t new_cap = ctxList.cap == 0 ? 16 : ctxList.cap * 2;
    ctxList.list = realloc(ctxList.list, new_cap * sizeof(JSContext *));
    ctxList.cap = new_cap;
  }
  ctxList.list[ctxList.size++] = ctx;
  mtx_unlock(&ctxList.lock);
}

static void ctxList_del(JSContext *ctx)
{
  mtx_lock(&ctxList.lock);
  for (size_t i = 0; i < ctxList.size; ++i)
  {
    if (ctxList.list[i] == ctx)
    {
      // 释放该上下文
      JS_FreeContext(ctxList.list[i]);
      // 后面的元素前移
      for (size_t j = i + 1; j < ctxList.size; ++j)
      {
        ctxList.list[j - 1] = ctxList.list[j];
      }
      ctxList.size--;
      break;
    }
  }
  mtx_unlock(&ctxList.lock);
}

static void ctxList_destroyAll()
{
  mtx_lock(&ctxList.lock);
  for (size_t i = 0; i < ctxList.size; ++i)
  {
    JS_FreeContext(ctxList.list[i]);
  }
  free(ctxList.list);
  ctxList.list = NULL;
  ctxList.size = ctxList.cap = 0;
  mtx_unlock(&ctxList.lock);
  mtx_destroy(&ctxList.lock);
}

// 从 js 代码编译为字节码并执行
static void load_js_code(JSContext *ctx, const char *code)
{
  if (kugou_music_api_bundle == NULL || kugou_music_api_bundle_size == 0)
  {
    // 编译 kugou_music_api_bundle_code 为字节码
    JSValue bytecode = JS_Eval(ctx, (const char *)kugou_music_api_bundle_code,
                               strlen(kugou_music_api_bundle_code),
                               "<kugou_music_api>",
                               JS_EVAL_TYPE_GLOBAL | JS_EVAL_FLAG_COMPILE_ONLY);

    if (JS_IsException(bytecode))
    {
      js_std_dump_error(ctx);
      fprintf(stderr, "Error: Failed to compile kugou_music_api_bundle_code\n");
      exit(1);
    }

    // 序列化字节码
    size_t bytecode_size;
    uint8_t *bytecode_buffer = JS_WriteObject(ctx, &bytecode_size, bytecode, JS_WRITE_OBJ_BYTECODE);
    JS_FreeValue(ctx, bytecode);

    if (!bytecode_buffer)
    {
      fprintf(stderr, "Error: Failed to serialize kugou_music_api_bundle_code\n");
      exit(1);
    }

    // 写出字节码到 kugou_music_api_bundle
    kugou_music_api_bundle = malloc(bytecode_size);
    if (!kugou_music_api_bundle)
    {
      fprintf(stderr, "Error: Failed to allocate memory for kugou_music_api_bundle\n");
      js_free(ctx, bytecode_buffer);
      exit(1);
    }
    memcpy((void *)kugou_music_api_bundle, bytecode_buffer, bytecode_size);
    kugou_music_api_bundle_size = bytecode_size;

    js_free(ctx, bytecode_buffer);

    // 执行字节码
    JSValue result = JS_ReadObject(ctx, kugou_music_api_bundle, kugou_music_api_bundle_size, JS_READ_OBJ_BYTECODE);
    if (JS_IsException(result))
    {
      js_std_dump_error(ctx);
      printf("Error: Failed to execute kugou_music_api_bundle_code\n");
    }
    else
    {
      JSValue eval_result = JS_EvalFunction(ctx, result);
      if (JS_IsException(eval_result))
      {
        js_std_dump_error(ctx);
        printf("Error: Failed to evaluate kugou_music_api_bundle_code\n");
      }
      JS_FreeValue(ctx, eval_result);
    }
  }
  else
  {
    // 直接执行已有的字节码
    JSValue result = JS_ReadObject(ctx, kugou_music_api_bundle, kugou_music_api_bundle_size, JS_READ_OBJ_BYTECODE);
    if (JS_IsException(result))
    {
      js_std_dump_error(ctx);
      fprintf(stderr, "Error: Failed to execute kugou_music_api_bundle\n");
      exit(1);
    }

    JSValue eval_result = JS_EvalFunction(ctx, result);
    if (JS_IsException(eval_result))
    {
      js_std_dump_error(ctx);
      fprintf(stderr, "Error: Failed to evaluate kugou_music_api_bundle\n");
    }
    JS_FreeValue(ctx, eval_result);
  }
}

static JSContext *JS_GetContext(JSRuntime *rt)
{
  JSContext *ctx = JS_NewContext(rt);
  ctxList_add(ctx);
  if (!ctx)
    return NULL;
  js_init_module_os(ctx, "os");
  js_init_module_std(ctx, "std");
  js_init_module_http(ctx, "http");

  const char *str =
      "import * as std from 'std'\n"
      "import * as http from 'http'\n"
      "import * as os from 'os'\n"
      "globalThis.std = std\n"
      "globalThis.http = http\n"
      "globalThis.os = os\n"
      "var console = {};\n"
      "console.log = function(msg) {\n"
      "  std.printf(msg + '\\n');\n"
      "}\n"
      "globalThis.console = console;\n";
  JSValue init_compile = JS_Eval(
      ctx, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
  js_module_set_import_meta(ctx, init_compile, 1, 1);
  JSValue init_run = JS_EvalFunction(ctx, init_compile);
  JS_FreeValue(ctx, init_run);

  load_js_code(ctx, kugou_music_api_bundle_code);

  const char *str3 = "globalThis.kuGouMusicApi = new KuGouMusicApi();\n";
  // const char *str3 = "console.log(JSON.stringify(globalThis.KuGouMusicApi));console.log(JSON.stringify(Object.getOwnPropertyDescriptor(globalThis, 'KuGouMusicApi'))); // 查看属性描述符";
  JSValue r2 = JS_Eval(ctx, str3, strlen(str3), "<input>", JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(r2))
  {
    printf("Error: Failed to initialize kuGouMusicApi\n");
    js_std_dump_error(ctx); // 打印 JS 错误，比如 KuGouMusicApi 未定义之类
  }
  JS_FreeValue(ctx, r2);

  return ctx;
}

// 创建新上下文并返回
JSContext *get_context()
{
  return JS_GetContext(rt);
}

// 库初始化
JSContext *init(ProcessEnv *env)
{
  globalEnv.platform = strdup(env && env->platform ? env->platform : "");
  globalEnv.KUGOU_API_GUID = strdup(env && env->KUGOU_API_GUID ? env->KUGOU_API_GUID : "");
  globalEnv.KUGOU_API_DEV = strdup(env && env->KUGOU_API_DEV ? env->KUGOU_API_DEV : "");
  globalEnv.KUGOU_API_MAC = strdup(env && env->KUGOU_API_MAC ? env->KUGOU_API_MAC : "");

  ctxList_init();
  rt = JS_NewRuntime();
  js_std_init_handlers(rt);
  js_std_set_worker_new_context_func(JS_GetContext);
  JS_SetModuleLoaderFunc2(rt, NULL, js_module_loader, js_module_check_attributes, NULL);
  return JS_GetContext(rt);
};

// 销毁上下文
int destroy_context(JSContext *ctx)
{
  ctxList_del(ctx);
  return 0;
};

// 库销毁
int destroy()
{
  js_std_free_handlers(rt);
  ctxList_destroyAll();
  free(kugou_music_api_bundle);
  JS_FreeRuntime(rt);
  free(globalEnv.platform);
  free(globalEnv.KUGOU_API_GUID);
  free(globalEnv.KUGOU_API_DEV);
  free(globalEnv.KUGOU_API_MAC);
  return 0;
};

char *toMallocString(const char *str)
{
  size_t len = strlen(str);
  char *result = malloc(len + 1);
  if (result)
    memcpy(result, str, len + 1);
  return result;
}

static void registerEnv(JSContext *ctx, const char *platform, const char *guid, const char *dev, const char *mac)
{
  // 构建 JS 脚本，将值挂载到 globalThis.process.env
  char jsCode[1024];
  snprintf(jsCode, sizeof(jsCode),
           "globalThis.process = globalThis.process || {};\n"
           "globalThis.process.env = globalThis.process.env || {};\n"
           "globalThis.process.env.platform = '%s';\n"
           "globalThis.process.env.KUGOU_API_GUID = '%s';\n"
           "globalThis.process.env.KUGOU_API_DEV = '%s';\n"
           "globalThis.process.env.KUGOU_API_MAC = '%s';",
           platform, guid, dev, mac);

  JSValue result = JS_Eval(ctx, jsCode, strlen(jsCode), "<registerEnv>", JS_EVAL_TYPE_GLOBAL);

  if (JS_IsException(result))
  {
    js_std_dump_error(ctx); // 打印 JS 错误
  }

  JS_FreeValue(ctx, result);
}

char *request(JSContext *ctx,
              const char *route,
              const char *cookies,
              const char *params,
              ProcessEnv *env)
{
  // 在每次请求时更新环境变量
  ProcessEnv *_env = env ? env : &globalEnv;
  registerEnv(ctx, _env->platform, _env->KUGOU_API_GUID, _env->KUGOU_API_DEV, _env->KUGOU_API_MAC);

  // 1. 取 kuGouMusicApi.route
  JSValue global_obj = JS_GetGlobalObject(ctx);
  JSValue api = JS_GetPropertyStr(ctx, global_obj, "kuGouMusicApi");

  if (JS_IsUndefined(api) || JS_IsNull(api))
  {
    JS_FreeValue(ctx, global_obj);
    JS_FreeValue(ctx, api);
    return toMallocString("kuGouMusicApi.route is not a function");
  }

  JSValue func = JS_GetPropertyStr(ctx, api, "route");
  if (!JS_IsFunction(ctx, func))
  {
    JS_FreeValue(ctx, global_obj);
    JS_FreeValue(ctx, api);
    JS_FreeValue(ctx, func);
    return toMallocString("kuGouMusicApi.route is not a function");
  }

  // 2. 调用 route 得到 Promise
  JSValue argv[3];
  argv[0] = JS_NewString(ctx, route ? route : "");
  argv[1] = JS_NewString(ctx, cookies ? cookies : "");
  argv[2] = JS_NewString(ctx, params ? params : "");

  JSValue p = JS_Call(ctx, func, api, 3, argv);

  JS_FreeValue(ctx, argv[0]);
  JS_FreeValue(ctx, argv[1]);
  JS_FreeValue(ctx, argv[2]);
  JS_FreeValue(ctx, func);
  JS_FreeValue(ctx, api);

  if (JS_IsException(p))
  {
    js_std_dump_error(ctx);
    JSValue exception = JS_GetException(ctx);
    const char *err_str = JS_ToCString(ctx, exception);
    JS_FreeValue(ctx, exception);
    JS_FreeValue(ctx, global_obj);
    if (!err_str)
      return toMallocString("JS_Call exception");
    char *result = toMallocString(err_str);
    JS_FreeCString(ctx, err_str);
    return result;
  }

  // 3. 把 Promise 挂到全局，并设置 then/catch 把结果写到全局变量
  JS_SetPropertyStr(ctx, global_obj, "__pendingRoutePromise", JS_DupValue(ctx, p));

  const char *code =
      "globalThis.__routeDone = false;\n"
      "globalThis.__lastRouteResult = undefined;\n"
      "globalThis.__lastRouteError = undefined;\n"
      "globalThis.__pendingRoutePromise\n"
      "  .then(res => { globalThis.__lastRouteResult = res; globalThis.__routeDone = true; })\n"
      "  .catch(err => { globalThis.__lastRouteError = err; globalThis.__routeDone = true; });\n";

  JSValue eval_res = JS_Eval(ctx, code, strlen(code),
                             "<route-wrapper>", JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(eval_res))
  {
    js_std_dump_error(ctx);
    JS_FreeValue(ctx, eval_res);
    JS_FreeValue(ctx, p);
    JS_FreeValue(ctx, global_obj);
    return toMallocString("failed to setup route wrapper");
  }
  JS_FreeValue(ctx, eval_res);
  JS_FreeValue(ctx, p);

  // 4. 跑事件循环，直到 Promise 处理完成
  js_std_loop(ctx);

  // 5. 从全局读结果/错误
  JSValue done = JS_GetPropertyStr(ctx, global_obj, "__routeDone");
  int is_done = JS_ToBool(ctx, done);
  JS_FreeValue(ctx, done);

  if (!is_done)
  {
    JS_FreeValue(ctx, global_obj);
    return toMallocString("route did not finish");
  }

  char *result;

  JSValue err = JS_GetPropertyStr(ctx, global_obj, "__lastRouteError");
  if (!JS_IsUndefined(err) && !JS_IsNull(err))
  {
    const char *err_str = JS_ToCString(ctx, err);
    JS_FreeValue(ctx, err);
    JS_FreeValue(ctx, global_obj);
    if (!err_str)
      return toMallocString("unknown error");
    size_t len = strlen(err_str);
    result = malloc(len + 1);
    if (result)
      memcpy(result, err_str, len + 1);
    JS_FreeCString(ctx, err_str);
  }
  else
  {
    JSValue res = JS_GetPropertyStr(ctx, global_obj, "__lastRouteResult");
    const char *qjs_str = JS_ToCString(ctx, res);
    JS_FreeValue(ctx, res);
    JS_FreeValue(ctx, global_obj);

    if (!qjs_str)
      return toMallocString("failed to convert result to string");

    size_t len = strlen(qjs_str);
    result = malloc(len + 1);
    if (result)
    {
      memcpy(result, qjs_str, len + 1); // 包含结尾\\0
    }
    JS_FreeCString(ctx, qjs_str);
  }

  return result;
}