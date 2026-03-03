const fs = require("fs");
const path = require("path");

const MODULES_FILE = path.join(__dirname, "..", "KuGouMusicApi", "module");
const OUTPUR_JS_MODULE_DIR = path.join(__dirname, "..", "src", "js");
const OUTPUT_C_HEADER_DIR = path.join(__dirname, "..", "src", "include");
const OUTPUT_C_DIR = path.join(__dirname, "..", "src", "c");
const OUTPUT_C_DEF_DIR = path.join(__dirname, "..", "src", "def");
const specificRoute = {};

function getModules() {
  const files = fs.readdirSync(MODULES_FILE);
  const parseRoute = (fileName) =>
    specificRoute && fileName in specificRoute
      ? specificRoute[fileName]
      : `/${fileName.replace(/\.(js)$/i, "").replace(/_/g, "/")}`;

  // 过滤、排序、生成 require 语句和导出对象
  const modules = files
    .reverse()
    .filter((fileName) => fileName.endsWith(".js") && !fileName.startsWith("_"))
    .map((fileName) => {
      const identifier = fileName.split(".").shift();
      const route = parseRoute(fileName);
      const modulePath =
        "@kuGouMusicApi/" +
        path.join(path.basename(MODULES_FILE), fileName).replace(/\\/g, "/");
      return { identifier, route, modulePath };
    });
  return modules;
}

function generateModulesDefinitions(modules) {
  const lines = [
    "// 自动生成，请勿手动修改",
    ...modules.map(
      (m) => `const ${m.identifier} = require('${m.modulePath}');`
    ),
    "",
    "module.exports = [",
    ...modules.map(
      (m) =>
        `  { identifier: '${m.identifier}', route: '${m.route}', module: ${m.identifier} },`
    ),
    "];",
    "",
  ];
  return lines.join("\n");
}

function generateHeader(modules) {
  const lines = [
    "#ifndef KUGOU_API_H",
    "#define KUGOU_API_H",
    "",
    "#include <stdio.h>",
    "#include <stdlib.h>",
    "",
    "typedef struct ProcessEnv {",
    "  char *platform;",
    "  char *KUGOU_API_GUID;",
    "  char *KUGOU_API_DEV;",
    "  char *KUGOU_API_MAC;",
    "} ProcessEnv;",
    "typedef struct JSContext JSContext;",
    "JSContext *kugou_init(ProcessEnv *env);",
    "JSContext *kugou_init_simple();",
    "int kugou_destroy();",
    "JSContext *get_kugou_context();",
    "",
    "char *kugou_request(JSContext *ctx, const char *route, const char *cookies, const char *params, ProcessEnv *env);",
    "char *kugou_request_simple(JSContext *ctx, const char *route, const char *cookies, const char *params);",
    // ...modules.flatMap((m) => [
    //   `char *${m.identifier}(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);`,
    //   `char *${m.identifier}_simple(JSContext *ctx, const char *cookies, const char *params);`,
    // ]),

    "typedef struct KugouAPI",
    "{",
    // "  char *(*top_song)(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);",
    ...modules.flatMap((m) => [
      `  char *(*${m.identifier})(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);`,
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

function generateImpl(modules) {
  const lines = [
    '#include "quickjs.h"',
    '#include "kugou_music_api.h"',
    "",
    "#define API_FUNC(name, route)  \\",
    "    static char *name(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {  \\",
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

function generateDef(modules) {
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
  console.log("Generating functions by modules...");

  const modules = getModules();
  console.log(`Found ${modules.length} modules`);

  console.log("Generating modules Js definitions...");

  const modulesDefinitions = generateModulesDefinitions(modules);
  fs.writeFileSync(
    path.join(OUTPUR_JS_MODULE_DIR, "modulesDefinitions.js"),
    modulesDefinitions
  );
  console.log(`Written: ${OUTPUR_JS_MODULE_DIR}/modulesDefinitions.js`);

  console.log("Generating C API...");

  const header = generateHeader(modules);
  fs.writeFileSync(path.join(OUTPUT_C_HEADER_DIR, "kugou_music_api.h"), header);
  console.log(`Written: ${OUTPUT_C_HEADER_DIR}/kugou_music_api.h`);

  const impl = generateImpl(modules);
  fs.writeFileSync(path.join(OUTPUT_C_DIR, "kugou/extension.c"), impl);
  console.log(`Written: ${OUTPUT_C_DIR}/kugou/extension.c`);

  const def = generateDef(modules);
  fs.writeFileSync(path.join(OUTPUT_C_DEF_DIR, "kugou_music_api.def"), def);
  console.log(`Written: ${OUTPUT_C_DEF_DIR}/kugou_music_api.def`);

  console.log("Done!");
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
