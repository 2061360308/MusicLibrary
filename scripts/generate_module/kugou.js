const fs = require("fs");
const path = require("path");
const { getModules, generateModulesDefinitions } = require("./tool");

const KugouModulesPath = path.join(__dirname, "..", "..", "KuGouMusicApi", "module");

const OUTPUR_JS_MODULE_DIR = path.join(__dirname, "..", "..", "src", "js", "kugou");
const OUTPUT_C_HEADER_DIR = path.join(__dirname, "..", "..", "src", "include");
const OUTPUT_C_DIR = path.join(__dirname, "..", "..", "src", "c", "kugou");
const OUTPUT_C_DEF_DIR = path.join(__dirname, "..", "..", "src", "def");

function generateKugouHeader(modules) {
  const lines = [
    "#ifndef KUGOU_API_H",
    "#define KUGOU_API_H",
    "",
    "#include <stdio.h>",
    "#include <stdlib.h>",
    "",
    "typedef struct KugouProcessEnv {",
    "  char *platform;",
    "  char *KUGOU_API_GUID;",
    "  char *KUGOU_API_DEV;",
    "  char *KUGOU_API_MAC;",
    "} KugouProcessEnv;",
    "typedef struct JSContext JSContext;",
    "JSContext *kugou_init(KugouProcessEnv *env);",
    "JSContext *kugou_init_simple();",
    "int kugou_destroy();",
    "JSContext *get_kugou_context();",
    "",
    "char *kugou_request(JSContext *ctx, const char *route, const char *cookies, const char *params, KugouProcessEnv *env);",
    "char *kugou_request_simple(JSContext *ctx, const char *route, const char *cookies, const char *params);",
    // ...modules.flatMap((m) => [
    //   `char *${m.identifier}(JSContext *ctx, const char *cookies, const char *params, KugouProcessEnv *env);`,
    //   `char *${m.identifier}_simple(JSContext *ctx, const char *cookies, const char *params);`,
    // ]),

    "typedef struct KugouAPI",
    "{",
    // "  char *(*top_song)(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);",
    ...modules.flatMap((m) => [
      `  char *(*${m.identifier})(JSContext *ctx, const char *cookies, const char *params, KugouProcessEnv *env);`,
      `  char *(*${m.identifier}_simple)(JSContext *ctx, const char *cookies, const char *params);`,
    ]),
    "} KugouAPI;",
    "",
    "KugouAPI *create_kugou_api();",
    "",
    "#endif",
  ];

  return lines.join("\n");
}

function generateKugouImpl(modules) {
  const lines = [
    '#include "quickjs.h"',
    '#include "kugou_music_api.h"',
    "",
    "#define API_FUNC(name, route)  \\",
    "    static char *name(JSContext *ctx, const char *cookies, const char *params, KugouProcessEnv *env) {  \\",
    "        return kugou_request(ctx, route, cookies, params, env);  \\",
    "    }",
    "",
    "#define API_FUNC_NOENV(name, route)  \\",
    "    static char *name(JSContext *ctx, const char *cookies, const char *params) {  \\",
    "        return kugou_request_simple(ctx, route, cookies, params);  \\",
    "    }",
    "",
    ...modules.flatMap((m) => [
      `API_FUNC(${m.identifier}, "${m.route}")`,
      `API_FUNC_NOENV(${m.identifier}_simple, "${m.route}")`,
    ]),
    "",
    "KugouAPI *create_kugou_api(){",
    "    KugouAPI *api = malloc(sizeof(KugouAPI));",
    ...modules.flatMap((m) => [
      `    api->${m.identifier} = ${m.identifier};`,
      `    api->${m.identifier}_simple = ${m.identifier}_simple;`,
    ]),
    "    return api;",
    "};",
  ];

  return lines.join("\n");
}

function generateKugouDef(modules) {
  const lines = [
    "LIBRARY kugou_music_api",
    "EXPORTS",
    "    get_kugou_context",
    "    kugou_init",
    "    kugou_init_simple",
    "    kugou_destroy",
    "    kugou_request",
    "    kugou_request_simple",
    "    create_kugou_api",
    // ...modules.flatMap((m) => [
    //   `    ${m.identifier}`,
    //   `    ${m.identifier}_simple`,
    // ]),
    "",
  ];

  return lines.join("\n");
}

function main() {
  console.log("Generating Kugou functions by modules...");

  const kugouModules = getModules(KugouModulesPath, "@kuGouMusicApi/");
  console.log(`Found ${kugouModules.length} modules`);

  console.log("Generating Js definitions...");

  const modulesDefinitions = generateModulesDefinitions(kugouModules);
  fs.writeFileSync(
    path.join(OUTPUR_JS_MODULE_DIR, "modulesDefinitions.js"),
    modulesDefinitions
  );
  console.log(`Written: ${OUTPUR_JS_MODULE_DIR}/modulesDefinitions.js`);

  console.log("Generating C API...");

  const header = generateKugouHeader(kugouModules);
  fs.writeFileSync(path.join(OUTPUT_C_HEADER_DIR, "kugou_music_api.h"), header);
  console.log(`Written: ${OUTPUT_C_HEADER_DIR}/kugou_music_api.h`);

  const impl = generateKugouImpl(kugouModules);
  fs.writeFileSync(path.join(OUTPUT_C_DIR, "extension.c"), impl);
  console.log(`Written: ${OUTPUT_C_DIR}/extension.c`);

  const def = generateKugouDef(kugouModules);
  fs.writeFileSync(path.join(OUTPUT_C_DEF_DIR, "kugou_music_api.def"), def);
  console.log(`Written: ${OUTPUT_C_DEF_DIR}/kugou_music_api.def`);

  console.log("Done!");
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
