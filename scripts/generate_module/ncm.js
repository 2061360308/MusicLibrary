const fs = require("fs");
const path = require("path");
const { getModules, generateModulesDefinitions } = require("./tool");

const NcmModulesPath = path.join(
  __dirname,
  "..",
  "..",
  "NeteaseCloudMusicApi",
  "module"
);

const OUTPUR_JS_MODULE_DIR = path.join(__dirname, "..", "..", "src", "js", "ncm");
const OUTPUT_C_HEADER_DIR = path.join(__dirname, "..", "..", "src", "include");
const OUTPUT_C_DIR = path.join(__dirname, "..", "..", "src", "c", "ncm");
const OUTPUT_C_DEF_DIR = path.join(__dirname, "..", "..", "src", "def");

const ncmSpecificRoute = {
  "daily_signin.js": "/daily_signin",
  "fm_trash.js": "/fm_trash",
  "personal_fm.js": "/personal_fm",
};

ncmSkipModules = ["cloud.js", "voice_upload.js"]

function generateNcmHeader(modules) {
  const lines = [
    "#ifndef NCM_API_H",
    "#define NCM_API_H",
    "",
    "#include <stdio.h>",
    "#include <stdlib.h>",
    "",
    "typedef struct NcmProcessEnv {",
    "  char *cnIp;",
    "  char *ANONYMOUS_TOKEN;",
    "} NcmProcessEnv;",
    "",
    "typedef struct JSContext JSContext;",
    "char *generate_random_cnIp(JSContext *ctx);",
    "char *generate_anonimous_token(JSContext *ctx);",
    "JSContext *ncm_init(NcmProcessEnv *env);",
    "int ncm_destroy();",
    "JSContext *get_ncm_context();",
    "",
    "char *ncm_request(JSContext *ctx, const char *route, const char *cookies, const char *params, const NcmProcessEnv *env);",
    // ...modules.flatMap((m) => [
    //   `char *${m.identifier}(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);`,
    //   `char *${m.identifier}_simple(JSContext *ctx, const char *cookies, const char *params);`,
    // ]),

    "typedef struct NCMAPI",
    "{",
    // "  char *(*top_song)(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);",
    ...modules.flatMap((m) => [
      `  char *(*${m.identifier})(JSContext *ctx, const char *cookies, const char *params, const NcmProcessEnv *env);`,
    ]),
    "} NCMAPI;",
    "",
    "NCMAPI *create_ncm_api();",
    "",
    "#endif",
  ];

  return lines.join("\n");
}

function generateNcmImpl(modules) {
  const lines = [
    '#include "quickjs.h"',
    '#include "ncm_music_api.h"',
    "",
    "#define API_FUNC(name, route)  \\",
    "    static char *name(JSContext *ctx, const char *cookies, const char *params, const NcmProcessEnv *env) {  \\",
    "        return ncm_request(ctx, route, cookies, params, env);  \\",
    "    }",
    "",
    ...modules.flatMap((m) => [
      `API_FUNC(${m.identifier}, "${m.route}")`,
    ]),
    "",
    "NCMAPI *create_ncm_api(){",
    "    NCMAPI *api = malloc(sizeof(NCMAPI));",
    ...modules.flatMap((m) => [
      `    api->${m.identifier} = ${m.identifier};`,
    ]),
    "    return api;",
    "};",
  ];

  return lines.join("\n");
}

function generateNcmDef(modules) {
  const lines = [
    "LIBRARY ncm_music_api",
    "EXPORTS",
    "    generate_random_cnIp",
    "    generate_anonimous_token",
    "    get_ncm_context",
    "    ncm_init",
    "    ncm_destroy",
    "    ncm_request",
    "    create_ncm_api",
    // ...modules.flatMap((m) => [
    //   `    ${m.identifier}`,
    //   `    ${m.identifier}_simple`,
    // ]),
    "",
  ];

  return lines.join("\n");
}

function main() {
  console.log("Generating NCM functions by modules...");

  const ncmModules = getModules(NcmModulesPath, "@NeteaseCloudMusicApi/", ncmSpecificRoute, ncmSkipModules);
  console.log(`Found ${ncmModules.length} modules`);

  console.log("Generating Js definitions...");

  const modulesDefinitions = generateModulesDefinitions(ncmModules);
  fs.writeFileSync(
    path.join(OUTPUR_JS_MODULE_DIR, "modulesDefinitions.js"),
    modulesDefinitions
  );
  console.log(`Written: ${OUTPUR_JS_MODULE_DIR}/modulesDefinitions.js`);

  console.log("Generating C API...");

  const header = generateNcmHeader(ncmModules);
  fs.writeFileSync(path.join(OUTPUT_C_HEADER_DIR, "ncm_music_api.h"), header);
  console.log(`Written: ${OUTPUT_C_HEADER_DIR}/ncm_music_api.h`);

  const impl = generateNcmImpl(ncmModules);
  fs.writeFileSync(path.join(OUTPUT_C_DIR, "extension.c"), impl);
  console.log(`Written: ${OUTPUT_C_DIR}/extension.c`);

  const def = generateNcmDef(ncmModules);
  fs.writeFileSync(path.join(OUTPUT_C_DEF_DIR, "ncm_music_api.def"), def);
  console.log(`Written: ${OUTPUT_C_DEF_DIR}/ncm_music_api.def`);

  console.log("Done!");
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
