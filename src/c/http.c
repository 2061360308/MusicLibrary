#include <stdio.h>
#include "curl/curl.h"
#include "quickjs.h"
#include "quickjs-libc.h"

#ifdef USE_LIBUV
#include "uv.h"
#endif

#include "cutils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef USE_LIBUV
uv_loop_t *loop;
#endif

typedef struct _http_ctx_t
{
  JSContext *ctx;
  JSValue func;   // JS 回调函数
  JSValue config; // 原始配置对象

  #ifdef USE_LIBUV
  uv_timer_t handle;
  #endif

  char *url;                      // 完整 URL
  char *method;                   // HTTP 方法
  char *req_body;                 // 请求体(JSON 字符串)
  struct curl_slist *req_headers; // 请求头

  char *resp_body; // 响应体
  size_t resp_size;
  size_t resp_cap;

  char *resp_headers; // 原始响应头
  size_t resp_hdr_size;
  size_t resp_hdr_cap;

  long status;          // HTTP 状态码
  long timeout_ms;      // 超时（毫秒），0 表示不限
  int response_type;    // 0=string  1=arraybuffer
  int req_body_is_form; // 1=字符串（URLEncoded）  0=对象（JSON）
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
  free(ctx->resp_headers);
  if (ctx->ctx)
  {
    JS_FreeValue(ctx->ctx, ctx->func);
    JS_FreeValue(ctx->ctx, ctx->config);
  }
  free(ctx);
}

static int js_http_init(JSContext *ctx, JSModuleDef *m);
JSModuleDef *js_init_module_http(JSContext *ctx, const char *module_name);
static JSValue js_http_get_request(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
static int is_unreserved_char(unsigned char c);
static char *url_encode_component(const char *src);
static char *build_query_string(JSContext *ctx, JSValueConst obj);
static struct curl_slist *build_headers_list(JSContext *ctx, JSValueConst headers_obj);
static void curl_main(http_ctx_t *hctx);

#ifdef USE_LIBUV
static void js_http_get_request_async(uv_timer_t *handle);
#endif

static size_t header_write_callback(char *buffer, size_t size, size_t nitems, void *userdata);
static size_t readData(void *ptr, size_t size, size_t nmemb, void *stream);
static void build_and_call_js_callback(http_ctx_t *hctx, CURLcode res);
static JSValue parse_response_headers(JSContext *ctx, const char *raw_headers);

/*
初始化阶段
js_init_module_http
  └── JS_NewCModule(... js_http_init ...)   // 注册模块
  └── JS_AddModuleExportList(...)           // 声明导出
        └── js_http_init                   // 模块加载时由 QuickJS 调用
              └── JS_SetModuleExportList  // 绑定 get_request → js_http_get_request
*/

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

/*
请求阶段（JS 调用 http.get_request(config, callback) 时）
js_http_get_request                        // JS 端触发
  ├── build_query_string(params)           // 构建 querystring（若有 params）
  ├── build_headers_list(headers)          // 构建请求头链表
  │
  ├── [sync=true]  → curl_main             // 同步直接执行
  └── [sync=false] → uv_timer_start(0)    // 异步，立即触发 timer
                        └── js_http_get_request_async   // timer 回调
                              └── curl_main
*/

#ifdef USE_LIBUV
// 异步调用包装
static void js_http_get_request_async(uv_timer_t *handle)
{
  http_ctx_t *data = (http_ctx_t *)handle->data;
  curl_main(data);
}
#endif

// RFC3986 未保留字符可直接保留，其余字符需要百分号编码
// 判断字符是否属于 URL 未保留字符
static int is_unreserved_char(unsigned char c)
{
  return (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~');
}

// 按 RFC3986 对字符串做 URL 组件编码（非未保留字符转为 %XX）
// 将参数值编码为 URL 安全字符串，避免 &, ?, = 等字符破坏 query 结构
static char *url_encode_component(const char *src)
{
  if (!src)
    return NULL;

  size_t src_len = strlen(src);
  size_t out_cap = src_len * 3 + 1;
  char *out = (char *)malloc(out_cap);
  if (!out)
    return NULL;

  size_t j = 0;
  for (size_t i = 0; i < src_len; i++)
  {
    unsigned char c = (unsigned char)src[i];
    if (is_unreserved_char(c))
    {
      out[j++] = (char)c;
    }
    else
    {
      static const char hex[] = "0123456789ABCDEF";
      out[j++] = '%';
      out[j++] = hex[(c >> 4) & 0x0F];
      out[j++] = hex[c & 0x0F];
    }
  }

  out[j] = '\0';
  return out;
}

// 从 JS 对象构建 querystring（key1=val1&key2=val2），对 key/value 执行 URL 编码
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
      char *enc_key = url_encode_component(key);
      char *enc_val = url_encode_component(val);

      if (!enc_key || !enc_val)
      {
        free(enc_key);
        free(enc_val);
        JS_FreeCString(ctx, key);
        JS_FreeCString(ctx, val);
        JS_FreeValue(ctx, v);
        JS_FreeAtom(ctx, atom);
        break;
      }

      size_t need = strlen(enc_key) + strlen(enc_val) + 2; // '=' 或 '&'
      if (cap < size + need + 1)
      {
        size_t new_cap = cap == 0 ? 128 : cap * 2;
        while (new_cap < size + need + 1)
          new_cap *= 2;
        char *nb = (char *)realloc(buf, new_cap);
        if (!nb)
        {
          free(enc_key);
          free(enc_val);
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
      memcpy(buf + size, enc_key, strlen(enc_key));
      size += strlen(enc_key);
      buf[size++] = '=';
      memcpy(buf + size, enc_val, strlen(enc_val));
      size += strlen(enc_val);
      buf[size] = '\0';

      free(enc_key);
      free(enc_val);
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

// 从 JS headers 对象构建 curl_slist
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

// 实际注册到 JS 的函数，负责解析配置对象，准备请求数据，并根据 sync 字段决定同步执行还是异步调度
// JS 端调用 http.get_request(config, callback, [sync]) 时触发
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
  const char *baseURL_cstr = JS_IsUndefined(baseURL_val) ? NULL : JS_ToCString(ctx, baseURL_val);
  const char *url_path_cstr = JS_IsUndefined(url_val) ? NULL : JS_ToCString(ctx, url_val);
  const char *baseURL = baseURL_cstr ? baseURL_cstr : "";
  const char *url_path = url_path_cstr ? url_path_cstr : "";

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
    if (baseURL_cstr)
      JS_FreeCString(ctx, baseURL_cstr);
    if (url_path_cstr)
      JS_FreeCString(ctx, url_path_cstr);
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

  if (baseURL_cstr)
    JS_FreeCString(ctx, baseURL_cstr);
  if (url_path_cstr)
    JS_FreeCString(ctx, url_path_cstr);
  JS_FreeValue(ctx, baseURL_val);
  JS_FreeValue(ctx, url_val);
  JS_FreeValue(ctx, params_val);
  free(query);

  // 解析 method，统一转大写（兼容 "post" / "POST"）
  JSValue method_val = JS_GetPropertyStr(ctx, config, "method");
  const char *method_cstr = JS_IsUndefined(method_val) ? NULL : JS_ToCString(ctx, method_val);
  const char *method_raw = method_cstr ? method_cstr : "GET";
  {
    size_t mlen = strlen(method_raw);
    data->method = (char *)malloc(mlen + 1);
    if (data->method)
      for (size_t mi = 0; mi <= mlen; mi++)
        data->method[mi] = (char)toupper((unsigned char)method_raw[mi]);
  }
  if (method_cstr)
    JS_FreeCString(ctx, method_cstr);
  JS_FreeValue(ctx, method_val);

  // 解析 data：字符串直接作为请求体，对象则序列化为 JSON
  JSValue data_val = JS_GetPropertyStr(ctx, config, "data");
  if (!JS_IsUndefined(data_val) && !JS_IsNull(data_val))
  {
    if (JS_IsString(data_val))
    {
      // 字符串（URLEncoded）：直接透传，保持 x-www-form-urlencoded
      const char *s = JS_ToCString(ctx, data_val);
      if (s)
      {
        data->req_body = c_strdup(s);
        JS_FreeCString(ctx, s);
      }
      data->req_body_is_form = 1;
    }
    else
    {
      // 对象：JSON 序列化，后续自动附加 application/json
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
      data->req_body_is_form = 0;
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

  // 解析 timeout（毫秒）
  JSValue timeout_val = JS_GetPropertyStr(ctx, config, "timeout");
  if (JS_IsNumber(timeout_val))
  {
    double t = 0;
    JS_ToFloat64(ctx, &t, timeout_val);
    data->timeout_ms = (long)t;
  }
  JS_FreeValue(ctx, timeout_val);

  // 解析 responseType
  JSValue rt_val = JS_GetPropertyStr(ctx, config, "responseType");
  if (JS_IsString(rt_val))
  {
    const char *rt_str = JS_ToCString(ctx, rt_val);
    if (rt_str)
    {
      if (strcmp(rt_str, "arraybuffer") == 0)
        data->response_type = 1;
      JS_FreeCString(ctx, rt_str);
    }
  }
  JS_FreeValue(ctx, rt_val);

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

/*
curl_main - 核心函数，执行 HTTP 请求并处理响应

curl_main
  ├── curl_easy_init / curl_easy_setopt    // 配置请求
  ├── [请求过程中 libcurl 回调]
  │     ├── header_write_callback          // 每次收到响应头行时触发
  │     └── readData                       // 每次收到响应体数据时触发
  ├── curl_easy_getinfo                    // 获取状态码
  ├── curl_easy_cleanup
  ├── build_and_call_js_callback           // 组装响应 → 调用 JS 回调
  │     └── parse_response_headers        // 解析原始响应头为 JS 对象
  └── http_ctx_free                        // 释放所有资源
*/

// 核心 HTTP 请求逻辑，使用 libcurl 执行请求，收集响应数据，并在完成后调用 JS 回调
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
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    // 自动解压 gzip / deflate / br
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    // 超时（0 表示不限）
    if (hctx->timeout_ms > 0)
      curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, hctx->timeout_ms);

    if (hctx->req_body && (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0 || strcmp(method, "PATCH") == 0))
    {
      // 检查用户是否已手动指定 Content-Type / Accept
      int has_ct = 0, has_accept = 0;
      for (struct curl_slist *h = hctx->req_headers; h; h = h->next)
      {
        const char *d = h->data;
        size_t i;
        /* content-type */
        for (i = 0; "content-type"[i] && d[i]; i++)
          if (tolower((unsigned char)d[i]) != "content-type"[i])
            break;
        if (!"content-type"[i])
        {
          has_ct = 1;
        }
        /* accept */
        for (i = 0; "accept"[i] && d[i]; i++)
          if (tolower((unsigned char)d[i]) != "accept"[i])
            break;
        if (!"accept"[i] && (d[i] == ':' || d[i] == ' '))
        {
          has_accept = 1;
        }
      }
      if (!has_ct)
      {
        // 字符串 data 保持 curl 默认的 x-www-form-urlencoded，对象 data 附加 application/json
        if (hctx->req_body_is_form)
          hctx->req_headers = curl_slist_append(hctx->req_headers, "Content-Type: application/x-www-form-urlencoded");
        else
          hctx->req_headers = curl_slist_append(hctx->req_headers, "Content-Type: application/json;charset=utf-8");
      }

      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, hctx->req_body);
    }

    // 默认附加 Accept（与 axios 保持一致）
    {
      int has_accept = 0;
      for (struct curl_slist *h = hctx->req_headers; h; h = h->next)
      {
        const char *d = h->data;
        size_t i;
        for (i = 0; "accept"[i] && d[i]; i++)
          if (tolower((unsigned char)d[i]) != "accept"[i])
            break;
        if (!"accept"[i] && (d[i] == ':' || d[i] == ' '))
        {
          has_accept = 1;
          break;
        }
      }
      if (!has_accept)
        hctx->req_headers = curl_slist_append(hctx->req_headers, "Accept: application/json, text/plain, */*");
    }

    if (hctx->req_headers)
    {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hctx->req_headers);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, hctx);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, hctx);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  // 启用调试输出

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

    // fprintf(stderr, "HTTP Response Status: %ld\n", hctx->status);
    // fprintf(stderr, "HTTP Response Headers:\n%s\n", hctx->resp_headers ? hctx->resp_headers : "(null)");
    // fprintf(stderr, "HTTP Response Body: %s\n", hctx->resp_body ? hctx->resp_body : "(null)");
  }

  if (curl)
    curl_easy_cleanup(curl);

  build_and_call_js_callback(hctx, res);
  http_ctx_free(hctx);
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

static size_t header_write_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
  http_ctx_t *hctx = (http_ctx_t *)userdata;
  size_t len = size * nitems;
  if (len == 0)
    return 0;

  if (hctx->resp_hdr_cap < hctx->resp_hdr_size + len + 1)
  {
    size_t new_cap = hctx->resp_hdr_cap == 0 ? 4096 : hctx->resp_hdr_cap * 2;
    while (new_cap < hctx->resp_hdr_size + len + 1)
      new_cap *= 2;
    char *new_buf = (char *)realloc(hctx->resp_headers, new_cap);
    if (!new_buf)
      return 0;
    hctx->resp_headers = new_buf;
    hctx->resp_hdr_cap = new_cap;
  }

  memcpy(hctx->resp_headers + hctx->resp_hdr_size, buffer, len);
  hctx->resp_hdr_size += len;
  hctx->resp_headers[hctx->resp_hdr_size] = '\0';
  return len;
}

/*
Js 回调阶段：curl_main 内部调用 build_and_call_js_callback
*/

// 将原始头字符串解析为 JS 对象，多个 set-cookie 聚合为字符串','分隔
static JSValue parse_response_headers(JSContext *ctx, const char *raw_headers)
{
  JSValue obj = JS_NewObject(ctx);
  if (!raw_headers || !*raw_headers)
    return obj;

  char *copy = c_strdup(raw_headers);
  if (!copy)
    return obj;

  char *p = copy;
  while (*p)
  {
    // 找行尾（兼容 \r\n 和 \n）
    char *eol = p;
    while (*eol && *eol != '\r' && *eol != '\n')
      eol++;

    size_t line_len = (size_t)(eol - p);
    char saved = *eol;
    *eol = '\0';

    // 跳过空行和状态行（HTTP/1.1 200 OK 或 HTTP/2 200）
    if (line_len > 0 && strncmp(p, "HTTP/", 5) != 0)
    {
      char *colon = strchr(p, ':');
      if (colon)
      {
        size_t key_len = (size_t)(colon - p);
        char *key = (char *)malloc(key_len + 1);
        if (key)
        {
          memcpy(key, p, key_len);
          key[key_len] = '\0';
          // 键名转小写
          for (size_t i = 0; i < key_len; i++)
            key[i] = (char)tolower((unsigned char)key[i]);
          // 去掉键名尾部空格
          while (key_len > 0 && (key[key_len - 1] == ' ' || key[key_len - 1] == '\t'))
            key[--key_len] = '\0';

          // 值：跳过前导空白
          const char *val = colon + 1;
          while (*val == ' ' || *val == '\t')
            val++;

          // set-cookie 多条用 ',' 分隔聚合为一个字符串
          if (strcmp(key, "set-cookie") == 0)
          {
            JSValue prev = JS_GetPropertyStr(ctx, obj, key);
            const char *prev_str = JS_IsUndefined(prev) ? NULL : JS_ToCString(ctx, prev);
            size_t prev_len = prev_str ? strlen(prev_str) : 0;
            size_t val_len = strlen(val);
            // ", " 占 2 字节，'\0' 占 1 字节，共需 +3
            size_t new_len = prev_len + val_len + 3;
            char *new_val = (char *)malloc(new_len);
            if (new_val)
            {
              if (prev_str && *prev_str)
                snprintf(new_val, new_len, "%s, %s", prev_str, val);
              else
                snprintf(new_val, new_len, "%s", val);
              JS_SetPropertyStr(ctx, obj, key, JS_NewString(ctx, new_val));
              free(new_val);
            }
            if (prev_str)
              JS_FreeCString(ctx, prev_str);
            JS_FreeValue(ctx, prev);
          }
          else
          {
            JS_SetPropertyStr(ctx, obj, key, JS_NewString(ctx, val));
          }
          free(key);
        }
      }
    }

    *eol = saved;
    p = eol;
    if (*p == '\r')
      p++;
    if (*p == '\n')
      p++;
  }

  free(copy);
  return obj;
}

static void build_and_call_js_callback(http_ctx_t *hctx, CURLcode res)
{
  JSContext *ctx = hctx->ctx;

  JSValue resp = JS_NewObject(ctx);

  // data: arraybuffer 或字符串，取决于 responseType
  JSValue data_val;
  if (hctx->response_type == 1)
  {
    // 将原始字节包装为 ArrayBuffer
    size_t blen = hctx->resp_size;
    const uint8_t *bptr = blen > 0 ? (const uint8_t *)hctx->resp_body : (const uint8_t *)"";
    data_val = JS_NewArrayBufferCopy(ctx, bptr, blen);
  }
  else
  {
    data_val = JS_NewString(ctx, hctx->resp_body ? hctx->resp_body : "");
  }
  JS_SetPropertyStr(ctx, resp, "data", data_val);

  // status
  JS_SetPropertyStr(ctx, resp, "status", JS_NewInt32(ctx, (int)hctx->status));

  // statusText
  const char *status_text = (res == CURLE_OK) ? "OK" : curl_easy_strerror(res);
  JS_SetPropertyStr(ctx, resp, "statusText", JS_NewString(ctx, status_text));

  // headers: 解析响应头
  JSValue headers_obj = parse_response_headers(ctx, hctx->resp_headers);
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
