const fs = require("fs");
const path = require("path");
const webpack = require("webpack");

/**
 * 自动生成静态导入和批量导出的 JS 文件
 * @param {string} modulesPath 目录
 * @param {string} outputPath 输出文件
 * @param {Record<string, string>} specificRoute 路由映射
 */
async function generateStaticModules(
  modulesPath,
  outputPath,
  specificRoute = {}
) {
  const files = await fs.promises.readdir(modulesPath);
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
        path.join(path.basename(modulesPath), fileName).replace(/\\/g, "/");
      return { identifier, route, modulePath };
    });

  // 生成代码
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

  const cHeaderLines = [
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

  const cImplLines = [
    "#include \"quickjs.h\"",
    "#include \"kugou_music_api.h\"",
    "",
    ...modules.map(
      (m) =>
        `char *${m.identifier}(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {\n` +
        `    return request(ctx, "${m.route}", cookies, params, env);\n}\n`
    ),
  ];

  fs.writeFileSync(outputPath[0], lines.join("\n"), "utf8");
  fs.writeFileSync(outputPath[1], cHeaderLines.join("\n"), "utf8");
  fs.writeFileSync(outputPath[2], cImplLines.join("\n"), "utf8");
}

(async () => {
  const modulesPath = path.join(__dirname, "KuGouMusicApi/module");
  const outputPath = [
    path.join(__dirname, "src/js/modulesDefinitions.js"),
    path.join(__dirname, "src/include/kugou_music_api.h"),
    path.join(__dirname, "src/c/kugou_music_api.c")
  ];
  await generateStaticModules(modulesPath, outputPath, {});
  console.log(`modulesDefinitions.js generated at ${outputPath[0]}`);
  console.log(`kugou_music_api.h generated at ${outputPath[1]}`);
  console.log(`kugou_music_api.c generated at ${outputPath[2]}`);
})();

module.exports = {
  mode: "production",
  entry: "./src/js/index.js", // 入口文件
  output: {
    path: path.resolve(__dirname, "bundle/js"),
    filename: "app.bundle.js",
    library: "KuGouMusicApi", // 导出为全局变量
    libraryTarget: "umd", // 支持多种模块规范
    globalObject: "globalThis",
  },
  resolve: {
    alias: {
      // 劫持 axios
      axios: path.resolve(__dirname, "src/js/axios_bridge.js"),
      "@kuGouMusicApi": path.resolve(__dirname, "KuGouMusicApi"),
    },
    modules: [
      path.resolve(__dirname, "src/js"),
      path.resolve(__dirname, "KuGouMusicApi"),
      "node_modules",
    ],
  },
  plugins: [
    new webpack.BannerPlugin({
      banner: `
'undefined' != typeof globalThis &&
  (void 0 === globalThis.window && (globalThis.window = globalThis),
  'object' !== typeof globalThis.process && (globalThis.process = {}),
  'object' !== typeof globalThis.process.env && (globalThis.process.env = {}),
  void 0 === globalThis.process.env.platform && (globalThis.process.env.platform = ''),
  void 0 === globalThis.process.env.KUGOU_API_GUID && (globalThis.process.env.KUGOU_API_GUID = ''),
  void 0 === globalThis.process.env.KUGOU_API_MAC && (globalThis.process.env.KUGOU_API_MAC = ''));
      `,
      raw: true,
      entryOnly: true,
    }),
  ],
  module: {
    rules: [
      {
        test: /\.js$/,
        use: {
          loader: "babel-loader", // 使用 babel
          options: {
            presets: [
              [
                "@babel/preset-env",
                {
                  targets: { ie: "11" },
                  useBuiltIns: false,
                  forceAllTransforms: true,
                },
              ],
            ],
            plugins: [],
          },
        },
      },
    ],
  },
  target: ["web", "es5"], // 兼容浏览器和 ES5
};
