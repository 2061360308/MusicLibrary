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

  console.log("Done!");
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
