const fs = require("fs");
const path = require("path");

const MODULES_FILE = path.join(__dirname, "..", "KuGouMusicApi", "module");
const OUTPUR_JS_MODULE_DIR = path.join(__dirname, "..", "src", "js");
const OUTPUT_C_HEADER_DIR = path.join(__dirname, "..", "src", "include");
const OUTPUT_C_DIR = path.join(__dirname, "..", "src", "c");
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
    "typedef struct ProcessEnv {",
    "  char *platform;",
    "  char *KUGOU_API_GUID;",
    "  char *KUGOU_API_DEV;",
    "  char *KUGOU_API_MAC;",
    "} ProcessEnv;",
    "typedef struct JSContext JSContext;",
    "JSContext *init(ProcessEnv *env);",
    "int destroy();",
    "JSContext *get_context();",
    "int destroy_context(JSContext *ctx);",
    "",
    "char *request(JSContext *ctx, const char *route, const char *cookies, const char *params, ProcessEnv *env);",
    ...modules.map(
      (m) =>
        `char *${m.identifier}(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);`
    ),
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
    "    char *name(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {  \\",
    "        return request(ctx, route, cookies, params, env);  \\",
    "    }",
    "",
    ...modules.map((m) => `API_FUNC(${m.identifier}, "${m.route}")`),
  ];

  return lines.join("\n");
}

function generateDef(modules) {
  const lines = [
    "LIBRARY kugou_music_api",
    "EXPORTS",
    "    init",
    "    destroy",
    "    get_context",
    "    destroy_context",
    "    request",
    ...modules.map((m) => `    ${m.identifier}`),
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
  fs.writeFileSync(path.join(OUTPUT_C_DIR, "kugou_music_api.c"), impl);
  console.log(`Written: ${OUTPUT_C_DIR}/kugou_music_api.c`);

  const def = generateDef(modules);
  fs.writeFileSync(path.join(OUTPUT_C_DIR, "kugou_music_api.def"), def);
  console.log(`Written: ${OUTPUT_C_DIR}/kugou_music_api.def`);

  console.log("Done!");
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
