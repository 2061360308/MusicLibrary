#include <stdio.h>
#include "curl/curl.h"
#include "quickjs.h"
#include "quickjs-libc.h"
#include "uv.h"
#include "cutils.h"
#include <string.h>
#include <stdlib.h>

uv_loop_t *loop;

typedef struct _http_ctx_t
{
  JSContext *ctx;
  JSValue func;   // JS 回调函数
  JSValue config; // 原始配置对象
  uv_timer_t handle;

  char *url;                      // 完整 URL
  char *method;                   // HTTP 方法
  char *req_body;                 // 请求体(JSON 字符串)
  struct curl_slist *req_headers; // 请求头

  char *resp_body; // 响应体
  size_t resp_size;
  size_t resp_cap;
  long status; // HTTP 状态码
} http_ctx_t;

static char *c_strdup(const char *s)
{
  if (!s)
    return NULL;
  size_t len = strlen(s);
  char *p = (char *)malloc(len + 1);
  if (!p)
    return NULL;
  memcpy(p, s, len + 1);
  return p;
}

static void http_ctx_free(http_ctx_t *ctx)
{
  if (!ctx)
    return;
  if (ctx->req_headers)
    curl_slist_free_all(ctx->req_headers);
  free(ctx->url);
  free(ctx->method);
  free(ctx->req_body);
  free(ctx->resp_body);
  if (ctx->ctx)
  {
    JS_FreeValue(ctx->ctx, ctx->func);
    JS_FreeValue(ctx->ctx, ctx->config);
  }
  free(ctx);
}

static size_t readData(void *ptr, size_t size, size_t nmemb, void *stream)
{
  http_ctx_t *ctx = (http_ctx_t *)stream;
  size_t len = size * nmemb;
  if (len == 0)
    return 0;

  if (ctx->resp_cap < ctx->resp_size + len + 1)
  {
    size_t new_cap = ctx->resp_cap == 0 ? 4096 : ctx->resp_cap * 2;
    while (new_cap < ctx->resp_size + len + 1)
      new_cap *= 2;
    char *new_buf = (char *)realloc(ctx->resp_body, new_cap);
    if (!new_buf)
      return 0; // 内存不足，中断
    ctx->resp_body = new_buf;
    ctx->resp_cap = new_cap;
  }

  memcpy(ctx->resp_body + ctx->resp_size, ptr, len);
  ctx->resp_size += len;
  ctx->resp_body[ctx->resp_size] = '\0';
  return len;
}

static void build_and_call_js_callback(http_ctx_t *hctx, CURLcode res)
{
  JSContext *ctx = hctx->ctx;

  JSValue resp = JS_NewObject(ctx);

  // data: 响应体字符串
  JSValue data_val = JS_NewString(ctx, hctx->resp_body ? hctx->resp_body : "");
  JS_SetPropertyStr(ctx, resp, "data", data_val);

  // status
  JS_SetPropertyStr(ctx, resp, "status", JS_NewInt32(ctx, (int)hctx->status));

  // statusText
  const char *status_text = (res == CURLE_OK) ? "OK" : curl_easy_strerror(res);
  JS_SetPropertyStr(ctx, resp, "statusText", JS_NewString(ctx, status_text));

  // headers: 先返回空对象，占位
  JSValue headers_obj = JS_NewObject(ctx);
  JS_SetPropertyStr(ctx, resp, "headers", headers_obj);

  // request: 先用 URL 字符串表示
  JS_SetPropertyStr(ctx, resp, "request", JS_NewString(ctx, hctx->url ? hctx->url : ""));

  // config: 直接返回原始配置对象副本
  JS_SetPropertyStr(ctx, resp, "config", JS_DupValue(ctx, hctx->config));

  //   printf("=========================HTTP Response=====================\n");
  JSValue func = hctx->func;
  JSValueConst this_obj = JS_UNDEFINED;
  JSValueConst argv[1] = {resp};
  JSValue ret = JS_Call(ctx, func, this_obj, 1, argv);
  if (JS_IsException(ret))
  {
    js_std_dump_error(ctx);
  }
  JS_FreeValue(ctx, ret);
  JS_FreeValue(ctx, resp);
  //   printf("===========================================================\n");
}

static void curl_main(http_ctx_t *hctx)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
  hctx->status = 0;

  curl = curl_easy_init();

  if (curl)
  {
    const char *method = hctx->method ? hctx->method : "GET";
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_URL, hctx->url);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // 忽略证书检查
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    if (hctx->req_headers)
    {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hctx->req_headers);
    }

    if (hctx->req_body && (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0 || strcmp(method, "PATCH") == 0))
    {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, hctx->req_body);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, hctx);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
      fprintf(stderr, "%s\n", curl_easy_strerror(res));
    }

    // 获取 HTTP 状态码
    long status = 0;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status) == CURLE_OK)
    {
      hctx->status = status;
    }
  }

  if (curl)
    curl_easy_cleanup(curl);

  build_and_call_js_callback(hctx, res);
  http_ctx_free(hctx);
}

// 从 JS 对象构建 querystring（key1=val1&key2=val2），未做 URL 编码
static char *build_query_string(JSContext *ctx, JSValueConst obj)
{
  if (!JS_IsObject(obj))
    return NULL;

  JSPropertyEnum *props = NULL;
  uint32_t len = 0;
  if (JS_GetOwnPropertyNames(ctx, &props, &len, obj, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY) < 0)
    return NULL;

  size_t cap = 0, size = 0;
  char *buf = NULL;

  for (uint32_t i = 0; i < len; ++i)
  {
    JSAtom atom = props[i].atom;
    const char *key = JS_AtomToCString(ctx, atom);
    JSValue v = JS_GetProperty(ctx, obj, atom);
    const char *val = JS_ToCString(ctx, v);

    if (key && val)
    {
      size_t need = strlen(key) + strlen(val) + 2; // '=' 或 '&'
      if (cap < size + need + 1)
      {
        size_t new_cap = cap == 0 ? 128 : cap * 2;
        while (new_cap < size + need + 1)
          new_cap *= 2;
        char *nb = (char *)realloc(buf, new_cap);
        if (!nb)
        {
          JS_FreeCString(ctx, key);
          JS_FreeCString(ctx, val);
          JS_FreeValue(ctx, v);
          JS_FreeAtom(ctx, atom);
          break;
        }
        buf = nb;
        cap = new_cap;
      }
      if (size > 0)
        buf[size++] = '&';
      memcpy(buf + size, key, strlen(key));
      size += strlen(key);
      buf[size++] = '=';
      memcpy(buf + size, val, strlen(val));
      size += strlen(val);
      buf[size] = '\0';
    }

    JS_FreeCString(ctx, key);
    JS_FreeCString(ctx, val);
    JS_FreeValue(ctx, v);
    JS_FreeAtom(ctx, atom);
  }

  js_free(ctx, props);
  if (!buf)
    return NULL;
  buf[size] = '\0';
  return buf;
}

// 从 headers 对象构建 curl_slist
static struct curl_slist *build_headers_list(JSContext *ctx, JSValueConst headers_obj)
{
  if (!JS_IsObject(headers_obj))
    return NULL;
  JSPropertyEnum *props = NULL;
  uint32_t len = 0;
  if (JS_GetOwnPropertyNames(ctx, &props, &len, headers_obj, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY) < 0)
    return NULL;

  struct curl_slist *list = NULL;

  for (uint32_t i = 0; i < len; ++i)
  {
    JSAtom atom = props[i].atom;
    const char *key = JS_AtomToCString(ctx, atom);
    JSValue v = JS_GetProperty(ctx, headers_obj, atom);
    const char *val = JS_ToCString(ctx, v);

    if (key && val)
    {
      size_t line_len = strlen(key) + strlen(val) + 3; // ": " 和结尾 0
      char *line = (char *)malloc(line_len);
      if (line)
      {
        snprintf(line, line_len, "%s: %s", key, val);
        list = curl_slist_append(list, line);
        free(line);
      }
    }

    JS_FreeCString(ctx, key);
    JS_FreeCString(ctx, val);
    JS_FreeValue(ctx, v);
    JS_FreeAtom(ctx, atom);
  }

  js_free(ctx, props);
  return list;
}

/*js请求方法绑定*/
static void js_http_get_request_async(uv_timer_t *handle)
{
  http_ctx_t *data = (http_ctx_t *)handle->data;
  curl_main(data);
}

static JSValue js_http_get_request(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
  if (argc < 2)
  {
    return JS_ThrowTypeError(ctx, "http.get_request expects (config, callback, [sync])");
  }

  JSValueConst config = argv[0];
  JSValueConst func = argv[1];

  if (!JS_IsObject(config) || !JS_IsFunction(ctx, func))
  {
    return JS_ThrowTypeError(ctx, "invalid arguments");
  }

  int sync = 0;
  if (argc >= 3)
  {
    sync = JS_ToBool(ctx, argv[2]);
  }

  http_ctx_t *data = (http_ctx_t *)calloc(1, sizeof(http_ctx_t));
  if (!data)
  {
    return JS_ThrowOutOfMemory(ctx);
  }

  data->ctx = ctx;
  data->func = JS_DupValue(ctx, func);
  data->config = JS_DupValue(ctx, config);

  // 解析 baseURL 和 url
  JSValue baseURL_val = JS_GetPropertyStr(ctx, config, "baseURL");
  JSValue url_val = JS_GetPropertyStr(ctx, config, "url");
  const char *baseURL = JS_IsUndefined(baseURL_val) ? "" : JS_ToCString(ctx, baseURL_val);
  const char *url_path = JS_IsUndefined(url_val) ? "" : JS_ToCString(ctx, url_val);

  // 解析 params，拼到 querystring
  JSValue params_val = JS_GetPropertyStr(ctx, config, "params");
  char *query = NULL;
  if (JS_IsObject(params_val))
  {
    query = build_query_string(ctx, params_val);
  }

  size_t full_len = strlen(baseURL) + strlen(url_path) + 1;
  if (query && query[0] != '\0')
  {
    full_len += 1 + strlen(query); // '?' + query
  }
  char *full_url = (char *)malloc(full_len);
  if (!full_url)
  {
    JS_FreeCString(ctx, baseURL);
    JS_FreeCString(ctx, url_path);
    JS_FreeValue(ctx, baseURL_val);
    JS_FreeValue(ctx, url_val);
    JS_FreeValue(ctx, params_val);
    free(query);
    http_ctx_free(data);
    return JS_ThrowOutOfMemory(ctx);
  }

  strcpy(full_url, baseURL);
  strcat(full_url, url_path);
  if (query && query[0] != '\0')
  {
    strcat(full_url, "?");
    strcat(full_url, query);
  }

  data->url = full_url;

  JS_FreeCString(ctx, baseURL);
  JS_FreeCString(ctx, url_path);
  JS_FreeValue(ctx, baseURL_val);
  JS_FreeValue(ctx, url_val);
  JS_FreeValue(ctx, params_val);
  free(query);

  // 解析 method
  JSValue method_val = JS_GetPropertyStr(ctx, config, "method");
  const char *method_str = JS_IsUndefined(method_val) ? "GET" : JS_ToCString(ctx, method_val);
  data->method = c_strdup(method_str);
  JS_FreeCString(ctx, method_str);
  JS_FreeValue(ctx, method_val);

  // 解析 data -> JSON 字符串作为请求体
  JSValue data_val = JS_GetPropertyStr(ctx, config, "data");
  if (!JS_IsUndefined(data_val) && !JS_IsNull(data_val))
  {
    JSValue json = JS_JSONStringify(ctx, data_val, JS_UNDEFINED, JS_UNDEFINED);
    if (!JS_IsException(json))
    {
      const char *json_str = JS_ToCString(ctx, json);
      if (json_str)
      {
        data->req_body = c_strdup(json_str);
        JS_FreeCString(ctx, json_str);
      }
      JS_FreeValue(ctx, json);
    }
  }
  JS_FreeValue(ctx, data_val);

  // 解析 headers
  JSValue headers_val = JS_GetPropertyStr(ctx, config, "headers");
  if (JS_IsObject(headers_val))
  {
    data->req_headers = build_headers_list(ctx, headers_val);
  }
  JS_FreeValue(ctx, headers_val);

  if (sync)
  {
    curl_main(data);
  }
  else
  {
    #ifdef USE_LIBUV
    data->handle.data = data;
    uv_timer_init(loop, &data->handle);
    uv_timer_start(&data->handle, js_http_get_request_async, 0, 0);
    #endif
  }

  return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_http_funcs[] = {
    JS_CFUNC_DEF("get_request", 1, js_http_get_request),
};

static int js_http_init(JSContext *ctx, JSModuleDef *m)
{
  return JS_SetModuleExportList(ctx, m, js_http_funcs, countof(js_http_funcs));
}

JSModuleDef *js_init_module_http(JSContext *ctx, const char *module_name)
{
  JSModuleDef *m = JS_NewCModule(ctx, module_name, js_http_init);
  if (!m)
    return NULL;
  JS_AddModuleExportList(ctx, m, js_http_funcs, countof(js_http_funcs));
  return m;
}