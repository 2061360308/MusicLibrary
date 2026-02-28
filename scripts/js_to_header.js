const fs = require("fs");
const path = require("path");

const jsDir = path.join(__dirname, "..", "bundle", "js");
const outputDir = path.join(__dirname, "..", "bundle", "c");
const headerOutputDir = path.join(__dirname, "..", "src", "include");

function main() {
  console.log("Generating C headers from JavaScript files...");
  if (!fs.existsSync(jsDir)) {
    console.error(`Error: ${jsDir} not found`);
    process.exit(1);
  }

  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  const files = fs.readdirSync(jsDir);

  const headerLines = [];

  headerLines.push("#ifndef JS_BUNDLE_H");
  headerLines.push("#define JS_BUNDLE_H");
  headerLines.push("");

  for (const file of files) {
    if (!file.endsWith(".js")) continue;

    console.log(`Processing: ${file}`);

    const jsPath = path.join(jsDir, file);
    const baseName = path.basename(file, ".js");
    const cPath = path.join(outputDir, `${baseName}.c`);

    const jsContent = fs.readFileSync(jsPath, "utf-8");

    const chunkSize = 1024; // 每块的最大大小
    const chunks = [];
    for (let i = 0; i < jsContent.length; i += chunkSize) {
      let chunk = jsContent.slice(i, i + chunkSize);
      // 对每个块进行转义
      chunk = chunk
        .replace(/\\/g, "\\\\") // 转义反斜杠
        .replace(/"/g, '\\"') // 转义双引号
        .replace(/\r?\n/g, "\\n"); // 转义换行符
      chunks.push(chunk);
    }

    const header =
      `const char ${baseName}_code[] = \n` +
      chunks.map((chunk) => `    "${chunk}"`).join("\n") +
      `;\n`;

    fs.writeFileSync(cPath, header, "utf-8");

    // headerLines.push(`#include "${baseName}.c"`);
    const size = Buffer.byteLength(jsContent, "utf-8"); // 计算文件内容的字节大小
    headerLines.push(`extern const char ${baseName}_code[${size}];`);

    console.log(`Generated: ${cPath}`);
  }

  headerLines.push("");
  headerLines.push("#endif // JS_BUNDLE_H");

  const headerPath = path.join(headerOutputDir, "js_bundle.h");
  fs.writeFileSync(headerPath, headerLines.join("\n"), "utf-8");
  console.log(`Generated: ${headerPath}`);
}

module.exports = main;

// 如果命令行直接调用执行
if (require.main === module) {
  main();
}
