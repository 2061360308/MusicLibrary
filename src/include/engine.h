#include <inttypes.h>

typedef struct JSContext JSContext;

typedef struct ByteCodeJs {
    uint32_t size;
    uint8_t *data;
} ByteCodeJs;

// 引擎库初始化
int init_engine();

// 创建新上下文并返回
JSContext *_get_context();

int destroy_context(JSContext *ctx);

// 从 js 代码编译为字节码
ByteCodeJs *genderByteCodeJs(JSContext *ctx, const char *code, char *filename);

// 执行js字节码
int load_js_code(JSContext *ctx, const ByteCodeJs *existingByteCode);

// 执行js代码,用于初始化全局对象
int eval_js(JSContext *ctx, char *code, char *filename);

// 执行js代码,获取返回值
char *eval_js_with_result(JSContext *ctx, char *code, char *filename);

// 执行js代码,等待获取Promise返回值
char *eval_js_with_promise_result(JSContext *ctx, char *code, char *filename);

// 引擎库销毁
int destroy_engine();

// 释放 response字符串 内存
void response_free(void *ptr);

// 调用对象的异步方法，等待结果并返回字符串
char *_request(JSContext *ctx,
               const char *apiName,
               const char *funcName,
               const char *argv[],
               int argc);