// NCM_API_VERSION 指定要拉取的版本
const NCM_API_VERSION = "4.29.0";
const TGZ_NAME = `NeteaseCloudMusicApi-${NCM_API_VERSION}.tgz`;
const API_DIR = "NeteaseCloudMusicApi";
const PACKAGE_DIR = `${API_DIR}/package`;
const path = require("path");
const PROJECT_ROOT = path.resolve(__dirname, "..");
const { execSync } = require("child_process");
const fs = require("fs");

// 1. 下载tgz包
console.log(`[pull_ncm] 下载 NeteaseCloudMusicApi@${NCM_API_VERSION} ...`);
execSync(`npm pack NeteaseCloudMusicApi@${NCM_API_VERSION}`, {
  stdio: "inherit",
  cwd: PROJECT_ROOT,
});

// 2. 解压到根目录NeteaseCloudMusicApi
const tgzPath = path.join(PROJECT_ROOT, TGZ_NAME);
const apiDirPath = path.join(PROJECT_ROOT, API_DIR);
const packageDirPath = path.join(PROJECT_ROOT, PACKAGE_DIR);
console.log(`[pull_ncm] 解压 ${tgzPath} 到 ${apiDirPath} ...`);
if (fs.existsSync(apiDirPath)) {
  fs.rmSync(apiDirPath, { recursive: true, force: true });
}
fs.mkdirSync(apiDirPath);
execSync(`tar -xzf ${tgzPath} -C ${apiDirPath}`);

// 3. 移动package子目录内容到NeteaseCloudMusicApi根目录
console.log(`[pull_ncm] 移动 package/* 到 ${apiDirPath}/ ...`);
if (fs.existsSync(packageDirPath)) {
  const files = fs.readdirSync(packageDirPath);
  for (const file of files) {
    const src = path.join(packageDirPath, file);
    const dest = path.join(apiDirPath, file);
    if (fs.existsSync(dest)) {
      fs.rmSync(dest, { recursive: true, force: true });
    }
    fs.renameSync(src, dest);
  }
}

// 4. 清理tgz包
console.log(`[pull_ncm] 删除临时包 ${tgzPath} ...`);
fs.unlinkSync(tgzPath);

// 5. 删除 register_anonimous.js 中 path 和 fs 的 require
const regAnonFile = path.join(apiDirPath, "module", "register_anonimous.js");
console.log(`[pull_ncm] 处理 ${regAnonFile} ...`);
if (fs.existsSync(regAnonFile)) {
  let content = fs.readFileSync(regAnonFile, "utf8");
  // 删除 const path = require('path') 和 const fs = require('fs')
  content = content.replace(/^const\s+path\s*=\s*require\(['"]path['"]\).*$/gm, "");
  content = content.replace(/^const\s+fs\s*=\s*require\(['"]fs['"]\).*$/gm, "");
  fs.writeFileSync(regAnonFile, content);
  console.log("[pull_ncm] 已移除 register_anonimous.js 中 path 和 fs 的 require");
}

console.log("[pull_ncm] 完成！");
