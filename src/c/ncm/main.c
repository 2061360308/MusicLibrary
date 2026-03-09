#include <stdio.h>
#include "engine.h"
#include "ncm_music_api.h"
#include <string.h> 

#ifdef _WIN32
#define strdup _strdup
#endif

extern const char ncm_music_api_bundle_code[];

static NcmProcessEnv globalEnv;

ByteCodeJs *ncm_music_api_bytecode = NULL;

static void registerEnv(JSContext *ctx, const char *cnIp, const char *ANONYMOUS_TOKEN)
{
    printf("Registering environment variables: cnIp=%s, ANONYMOUS_TOKEN=%s\n", cnIp, ANONYMOUS_TOKEN);
    // 构建 JS 脚本，将值挂载到 globalThis.process.env
    char jsCode[4096];
    int n = snprintf(jsCode, sizeof(jsCode),
         "globalThis.process = globalThis.process || {};\n"
         "globalThis.process.env = globalThis.process.env || {};\n"
         "globalThis.process.env.cnIp = \"%s\";\n"
         "globalThis.process.env.ANONYMOUS_TOKEN = \"%s\";",
         cnIp, ANONYMOUS_TOKEN);
    
    if (n >= sizeof(jsCode)) {
        fprintf(stderr, "Error: jsCode buffer too small, env string truncated!\n");
    }

    int result = eval_js(ctx, jsCode, "<Ncm-registerEnv>");
    if (result != 0)
    {
        fprintf(stderr, "Error: Failed to register environment variables\n");
    }
}

JSContext *get_ncm_context()
{
    JSContext *ctx = _get_context();
    if (ncm_music_api_bytecode)
    {
        int result = load_js_code(ctx, ncm_music_api_bytecode);
        if (result != 0)
        {
            fprintf(stderr, "Error: Failed to load ncm_music_api bytecode\n");
            // destroy_context(ctx);
            return NULL;
        }
    }
    else
    {
        // 从源代码编译字节码并执行
        ncm_music_api_bytecode = genderByteCodeJs(ctx, ncm_music_api_bundle_code, "<ncm_music_api_bundle_code>");
        if (!ncm_music_api_bytecode || !ncm_music_api_bytecode->data)
        {
            fprintf(stderr, "Error: Failed to compile ncm_music_api_bundle_code\n");
            // destroy_context(ctx);
            return NULL;
        }
        printf("ncm_music_api bytecode generated, size: %u bytes\n", ncm_music_api_bytecode->size);
        int result = load_js_code(ctx, ncm_music_api_bytecode);
        if (result != 0)
        {
            printf("Error: Failed to load ncm_music_api bytecode on first load\n");
            fprintf(stderr, "Error: Failed to load ncm_music_api bytecode\n");
            // destroy_context(ctx);
            return NULL;
        }
    }
    // eval_js(ctx, ncm_music_api_bundle_code);

    eval_js(ctx, "globalThis.ncmMusicApi = new NeteaseCloudMusicApiApi();\n", "<Ncm-init>");
    return ctx;
}

char *generate_random_cnIp(JSContext *ctx)
{
    char *result = eval_js_with_result(ctx, "NeteaseCloudMusicApiApi.generateRandomChineseIP();\n", "<Ncm-generate_random_cnIp>");
    return result;
}

char *generate_anonimous_token(JSContext *ctx)
{
    char *result = eval_js_with_promise_result(ctx, "ncmMusicApi.get_anonimous_cookie();\n", "<Ncm-generate_anonimous_token>");
    return result;
}

// 库初始化
JSContext *ncm_init(NcmProcessEnv *env)
{
    JSContext *ctx = get_ncm_context();

    globalEnv.cnIp = strdup(env && env->cnIp ? env->cnIp : generate_random_cnIp(ctx));
    globalEnv.ANONYMOUS_TOKEN = strdup(env && env->ANONYMOUS_TOKEN ? env->ANONYMOUS_TOKEN : generate_anonimous_token(ctx));
    return ctx;
}

// 库销毁
int ncm_destroy(){
    if (globalEnv.cnIp)
    {
        free(globalEnv.cnIp);
        globalEnv.cnIp = NULL;
    }
    if (globalEnv.ANONYMOUS_TOKEN)
    {
        free(globalEnv.ANONYMOUS_TOKEN);
        globalEnv.ANONYMOUS_TOKEN = NULL;
    }
    return 0;
}

char *ncm_request(JSContext *ctx,
                    const char *route,
                    const char *cookies,
                    const char *params,
                    const NcmProcessEnv *env
                )
{

    // 在每次请求时更新环境变量
    const char *cnIp = (env && env->cnIp) ? env->cnIp : globalEnv.cnIp;
    const char *ANONYMOUS_TOKEN = (env && env->ANONYMOUS_TOKEN) ? env->ANONYMOUS_TOKEN : globalEnv.ANONYMOUS_TOKEN;

    // 调用 registerEnv
    registerEnv(ctx, cnIp, ANONYMOUS_TOKEN);

    return _request(ctx, "ncmMusicApi", "route", (const char *[]){route, cookies, params}, 3);
}