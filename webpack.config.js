const path = require("path");
const webpack = require("webpack");
const TerserPlugin = require("terser-webpack-plugin");
const generate_module_funs = require("./scripts/generate_module_funs");
const js_to_header = require("./scripts/js_to_header");

// 生成模块函数定义
// modulesDefinitions.js、kugou_music_api.h、kugou_music_api.c
generate_module_funs();

module.exports = {
  mode: "production",
  entry: "./src/js/index.js", // 入口文件
  output: {
    path: path.resolve(__dirname, "bundle/js"),
    filename: "kugou_music_api_bundle.js",
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
  optimization: {
    minimize: true, // 先关闭压缩，方便调试。后续可以开启。
    minimizer: [
      new TerserPlugin({
        extractComments: false, // 不将注释提取到单独的文件中
        terserOptions: {
          format: {
            comments: false, // 从输出中删除所有注释
          },
        },
      }),
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
    {
      apply: (compiler) => {
        compiler.hooks.done.tap("BuildCompleteCallback", (stats) => {
          if (stats.hasErrors()) {
            console.error(
              "Build completed with errors. Skipping js_to_header."
            );
          } else {
            js_to_header();
          }
        });
      },
    },
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
  target: ["web", "es6"], // 兼容浏览器和 ES6
};
