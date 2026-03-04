const fs = require("fs");
const path = require("path");

function getModules(modulesPath, rootDir, specificRoute = {}, skipModules=[]) {
  const files = fs.readdirSync(modulesPath).filter(file => !skipModules.includes(file));
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
        rootDir + 
        path.join(path.basename(modulesPath), fileName).replace(/\\/g, "/");
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

module.exports = {
  getModules,
  generateModulesDefinitions,
};