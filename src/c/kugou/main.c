#include <stdio.h>
#include "engine.h"
#include "kugou_music_api.h"
#include <string.h> 

#ifdef _WIN32
#define strdup _strdup
#endif

extern const char kugou_music_api_bundle_code[];

ProcessEnv globalEnv;

// 存储编译后的字节码，方便多次创建上下文时直接加载执行
ByteCodeJs *kugou_music_api_bytecode = NULL;

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

    int result = eval_js(ctx, jsCode);
    if (result != 0)
    {
        fprintf(stderr, "Error: Failed to register environment variables\n");
    }
}

JSContext *get_kugou_context()
{
    JSContext *ctx = _get_context();
    if (kugou_music_api_bytecode)
    {
        int result = load_js_code(ctx, kugou_music_api_bytecode);
        if (result != 0)
        {
            fprintf(stderr, "Error: Failed to load kugou_music_api bytecode\n");
            // destroy_context(ctx);
            return NULL;
        }
    }
    else
    {
        // 从源代码编译字节码并执行
        kugou_music_api_bytecode = genderByteCodeJs(ctx, kugou_music_api_bundle_code);
        if (!kugou_music_api_bytecode || !kugou_music_api_bytecode->data)
        {
            fprintf(stderr, "Error: Failed to compile kugou_music_api_bundle_code\n");
            // destroy_context(ctx);
            return NULL;
        }
        printf("kugou_music_api bytecode generated, size: %u bytes\n", kugou_music_api_bytecode->size);
        int result = load_js_code(ctx, kugou_music_api_bytecode);
        if (result != 0)
        {
            printf("Error: Failed to load kugou_music_api bytecode on first load\n");
            fprintf(stderr, "Error: Failed to load kugou_music_api bytecode\n");
            // destroy_context(ctx);
            return NULL;
        }
    }
    // eval_js(ctx, kugou_music_api_bundle_code);

    eval_js(ctx, "globalThis.kuGouMusicApi = new KuGouMusicApi();\n");
    return ctx;
}

// 库初始化
JSContext *kugou_init(ProcessEnv *env)
{
    globalEnv.platform = strdup(env && env->platform ? env->platform : "");
    globalEnv.KUGOU_API_GUID = strdup(env && env->KUGOU_API_GUID ? env->KUGOU_API_GUID : "");
    globalEnv.KUGOU_API_DEV = strdup(env && env->KUGOU_API_DEV ? env->KUGOU_API_DEV : "");
    globalEnv.KUGOU_API_MAC = strdup(env && env->KUGOU_API_MAC ? env->KUGOU_API_MAC : "");

    JSContext *ctx = get_kugou_context();
    return ctx;
}

JSContext *kugou_init_simple()
{
    static ProcessEnv default_env = {NULL, NULL, NULL, NULL};
    return kugou_init(&default_env);
}

// 库销毁
int kugou_destroy()
{
    if (globalEnv.platform)
    {
        free(globalEnv.platform);
        globalEnv.platform = NULL;
    }
    if (globalEnv.KUGOU_API_GUID)
    {
        free(globalEnv.KUGOU_API_GUID);
        globalEnv.KUGOU_API_GUID = NULL;
    }
    if (globalEnv.KUGOU_API_DEV)
    {
        free(globalEnv.KUGOU_API_DEV);
        globalEnv.KUGOU_API_DEV = NULL;
    }
    if (globalEnv.KUGOU_API_MAC)
    {
        free(globalEnv.KUGOU_API_MAC);
        globalEnv.KUGOU_API_MAC = NULL;
    }
    if (kugou_music_api_bytecode)
    {
        free(kugou_music_api_bytecode->data);
        free(kugou_music_api_bytecode);
        kugou_music_api_bytecode = NULL;
    }
    return 0;
}

char *kugou_request(JSContext *ctx,
                    const char *route,
                    const char *cookies,
                    const char *params,
                    ProcessEnv *env)
{
    // 在每次请求时更新环境变量
    const char *platform = (env && env->platform) ? env->platform : globalEnv.platform;
    const char *guid = (env && env->KUGOU_API_GUID) ? env->KUGOU_API_GUID : globalEnv.KUGOU_API_GUID;
    const char *dev = (env && env->KUGOU_API_DEV) ? env->KUGOU_API_DEV : globalEnv.KUGOU_API_DEV;
    const char *mac = (env && env->KUGOU_API_MAC) ? env->KUGOU_API_MAC : globalEnv.KUGOU_API_MAC;

    // 调用 registerEnv
    registerEnv(ctx, platform, guid, dev, mac);

    return _request(ctx, "kuGouMusicApi", "route", (const char *[]){route, cookies, params}, 3);
}

char *kugou_request_simple(JSContext *ctx,
                           const char *route,
                           const char *cookies,
                           const char *params)
{
    static ProcessEnv default_env = {NULL, NULL, NULL, NULL};
    return kugou_request(ctx, route, cookies, params, &default_env);
}