#ifdef _WIN32
#define __declspec(dllexport)
#endif

// #include "tool.h"
#include "quickjs-libc.h"
#include "quickjs.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tinycthread.h"
#include "engine.h"

// 声明在 http.c 中实现的模块初始化函数
JSModuleDef *js_init_module_http(JSContext *ctx, const char *module_name);

JSRuntime *rt;

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

// 从 js 代码编译为字节码
ByteCodeJs *genderByteCodeJs(JSContext *ctx, const char *code)
{
    ByteCodeJs *errObj = malloc(sizeof(ByteCodeJs));
    if (errObj)
    {
        errObj->data = NULL;
        errObj->size = 0;
    }
    // 编译 code 为字节码
    JSValue bytecode = JS_Eval(ctx, (const char *)code,
                               strlen(code),
                               "<kugou_music_api>",
                               JS_EVAL_TYPE_GLOBAL | JS_EVAL_FLAG_COMPILE_ONLY);

    if (JS_IsException(bytecode))
    {
        js_std_dump_error(ctx);
        fprintf(stderr, "Error: Failed to compile code\n");
        return errObj;
    }

    // 序列化字节码
    size_t bytecode_size;
    uint8_t *bytecode_buffer = JS_WriteObject(ctx, &bytecode_size, bytecode, JS_WRITE_OBJ_BYTECODE);
    JS_FreeValue(ctx, bytecode);

    if (!bytecode_buffer)
    {
        fprintf(stderr, "Error: Failed to serialize bytecode\n");
        return errObj;
    }

    uint8_t *data = malloc(bytecode_size);
    if (!data)
    {
        fprintf(stderr, "Error: Failed to allocate memory for ByteCodeJs data\n");
        js_free(ctx, bytecode_buffer);
        return errObj;
    }

    memcpy((void *)data, bytecode_buffer, bytecode_size);
    uint32_t size = bytecode_size;
    js_free(ctx, bytecode_buffer);

    ByteCodeJs *result = malloc(sizeof(ByteCodeJs));
    if (!result)
    {
        free(data);
        return errObj;
    }
    result->data = data;
    result->size = bytecode_size;
    return result;
}

// 执行js字节码
int load_js_code(JSContext *ctx, const ByteCodeJs *existingByteCode)
{
    // 直接执行已有的字节码
    // JSValue result = JS_ReadObject(ctx, existingByteCode->data, existingByteCode->size, JS_READ_OBJ_BYTECODE);
    // if (JS_IsException(result))
    // {
    //     js_std_dump_error(ctx);
    //     fprintf(stderr, "Error: Failed to execute ByteCodeJs \n");
    //     JS_FreeValue(ctx, result);
    //     return 1;
    // }

    // JSValue eval_result = JS_EvalFunction(ctx, result);
    // if (JS_IsException(eval_result))
    // {
    //     js_std_dump_error(ctx);
    //     fprintf(stderr, "Error: Failed to evaluate ByteCodeJs \n");
    //     JS_FreeValue(ctx, eval_result);
    //     JS_FreeValue(ctx, result);
    //     return 1;
    // }
    // JS_FreeValue(ctx, eval_result);
    // JS_FreeValue(ctx, result);
    js_std_eval_binary(ctx, existingByteCode->data, existingByteCode->size, 0);
    return 0;
}

// 执行js代码
int eval_js(JSContext *ctx, char *code)
{
    JSValue r2 = JS_Eval(ctx, code, strlen(code), "<input>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(r2))
    {
        js_std_dump_error(ctx); // 打印 JS 错误，比如 KuGouMusicApi 未定义之类
        JS_FreeValue(ctx, r2);
        return 1;
    }
    JS_FreeValue(ctx, r2);
    return 0;
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
    
    return ctx;
}

// 创建新上下文并返回
JSContext *_get_context()
{
    return JS_GetContext(rt);
}

// 引擎库初始化
int init_engine()
{
    if (rt)
    {
        // 已经初始化过，不重复创建
        return 0;
    }
    ctxList_init();
    rt = JS_NewRuntime();
    js_std_init_handlers(rt);
    js_std_set_worker_new_context_func(JS_GetContext);
    JS_SetModuleLoaderFunc2(rt, NULL, js_module_loader, js_module_check_attributes, NULL);
    return 0;
}

// 销毁上下文
int destroy_context(JSContext *ctx)
{
    ctxList_del(ctx);
    return 0;
};

// 引擎库销毁
int destroy_engine()
{
    js_std_free_handlers(rt);
    ctxList_destroyAll();
    if (rt){
        JS_FreeRuntime(rt);
        rt = NULL;
    }
    return 0;
};

static char *toMallocString(const char *str)
{
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result)
        memcpy(result, str, len + 1);
    return result;
}

char *_request(JSContext *ctx,
               const char *apiName,
               const char *funcName,
               const char *argv[],
               int argc)
{
    // 1. 取 kuGouMusicApi.route
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue api = JS_GetPropertyStr(ctx, global_obj, apiName);

    if (JS_IsUndefined(api) || JS_IsNull(api))
    {
        JS_FreeValue(ctx, global_obj);
        JS_FreeValue(ctx, api);
        char buf[128];
        snprintf(buf, sizeof(buf), "%s is not defined", apiName);
        return toMallocString(buf);
    }

    JSValue func = JS_GetPropertyStr(ctx, api, funcName);
    if (!JS_IsFunction(ctx, func))
    {
        JS_FreeValue(ctx, global_obj);
        JS_FreeValue(ctx, api);
        JS_FreeValue(ctx, func);
        char buf[128];
        snprintf(buf, sizeof(buf), "%s.%s is not a function", apiName, funcName);
        return toMallocString(buf);
    }

    // 2. 调用 route 得到 Promise
    JSValue *js_argv = malloc(sizeof(JSValue) * argc);
    for (int i = 0; i < argc; ++i) {
        js_argv[i] = JS_NewString(ctx, argv[i] ? argv[i] : "");
    }

    JSValue p = JS_Call(ctx, func, api, argc, js_argv);

    for (int i = 0; i < argc; ++i) {
        JS_FreeValue(ctx, js_argv[i]);
    }
    free(js_argv);
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