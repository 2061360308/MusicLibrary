# KouGouMusicLibrary

[`KuGouMusicApi`](https://github.com/MakcRe/KuGouMusicApi)项目的 C 语言包装库，使用[`quilkjs-ng`](https://github.com/quickjs-ng/quickjs)引擎调用 Javascript。

## 功能概述

KouGouMusicLibrary 提供了一个 C 语言接口，用于调用 KuGouMusicApi 的功能。通过 QuickJS 引擎，开发者可以轻松地在 C 程序中运行 JavaScript 代码，完成诸如歌单管理、音乐搜索、推荐等功能。

## 免责声明

> 1. 本项目仅供学习使用，请尊重版权，请勿利用此项目从事商业行为及非法用途!
> 2. 使用本项目的过程中可能会产生版权数据。对于这些版权数据，本项目不拥有它们的所有权。为了避免侵权，使用者务必在 24 小时内清除使用本项目的过程中所产 生的版权数据。
> 3. 由于使用本项目产生的包括由于本协议或由于使用或无法使用本项目而引起的任何性质的任何直接、间接、特殊、偶然或结果性损害（包括但不限于因商誉损失、停 工、计算机故障或故障引起的损害赔偿，或任何及所有其他商业损害或损失）由使用者负责。
> 4. 禁止在违反当地法律法规的情况下使用本项目。 对于使用者在明知或不知当地法律法规不允许的情况下使用本项目所造成的任何违法违规行为由使用者承担，本 项目不承担由此造成的任何直接、间接、特殊、偶然或结果性责任。
> 5. 音乐平台不易，请尊重版权，支持正版。
> 6. 本项目仅用于对技术可行性的探索及研究，不接受任何商业（包括但不限于广告等）合作及捐赠。
> 7. 如果官方音乐平台觉得本项目不妥，可联系本项目更改或移除。

## 使用说明

### 关键方法介绍

> **注意**: 你需要包含 `kugou_music_api.h`，并确保 `JSContext` 不能跨线程使用。如果需要多线程支持，请为每个线程创建独立的 `JSContext`。

| 方法原型                               | 说明           |
| -------------------------------------- | -------------- |
| `JSContext *init(ProcessEnv *env);`    | 初始化         |
| `int destroy();`                       | 销毁退出       |
| `JSContext *get_context();`            | 创建运行上下文 |
| `int destroy_context(JSContext *ctx);` | 销毁运行上下文 |

### 示例代码

以下是一个简单的示例，展示了如何使用 `top_song` 方法获取热门歌曲数据：

```c
#include <stdio.h>
#include <stdlib.h>
#include "kugou_music_api.h"

int main(int argc, char **argv)
{

  
  ProcessEnv env = {
      .platform = "lite", //lite: 酷狗概念版, 默认为手机版留空字符串
      // 设备id(可选，建议固定 )
      // KUGOU_API_GUID 建议为 uuidv4
      // KUGOU_API_DEV 建议长度为 10 位的字符串
      .KUGOU_API_GUID = "45593bdf-a805-4c68-80fb-c526733fa49b",
      .KUGOU_API_DEV = "iz6v67tm56",
      .KUGOU_API_MAC = "42:69:CB:E2:0D:DE",  // MAC地址
  };

  JSContext *ctx = init(&env);
  // JSContext *ctx2 = get_context(); 创建第二个上下文

  const char *cookies = "";
  const char *params = "{}";

  const char *response = top_song(ctx, cookies, params, NULL);
  if (response)
  {
    FILE *fp = fopen("response.txt", "w");
    if (fp)
    {
      fprintf(fp, "%s", response);
      fclose(fp);
    }
    printf("Response: %s\n", response);
    free((void *)response);
  }
  else
  {
    printf("No response or error occurred.\n");
  }
}
```

### 可用方法列表

以下是库中提供的所有方法：

```c
char *request(JSContext *ctx, const char *route, const char *cookies, const char *params, ProcessEnv *env);
char *yueku_fm(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *yueku_banner(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *yueku(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_user_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_union_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_month_vip_record(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_listen_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_dynamic_recent(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_dynamic(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_day_vip_upgrade(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_day_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_sub(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_song_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_similar(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_amway(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *youth_channel_all(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *video_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *video_privilege(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *video_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_vip_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_video_love(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_video_collect(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_listen(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_history(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_follow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_cloud_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *user_cloud(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_ip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_card_youth(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_card(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *top_album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *theme_playlist_track(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *theme_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *theme_music_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *theme_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *song_url_new(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *song_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *song_ranking_filter(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *song_ranking(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *song_climax(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *singer_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *sheet_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *sheet_hot(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *sheet_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *sheet_collection_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *sheet_collection(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *server_now(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_suggest(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_mixed(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_lyric(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_hot(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_default(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search_complex(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *search(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_video_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_module_info(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_module(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_lists_v2(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_lists(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_collection_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *scene_audio_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *register_dev(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *recommend_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *rank_vol(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *rank_top(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *rank_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *rank_info(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *rank_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *privilege_lite(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_track_all_new(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_track_all(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_tracks_del(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_tracks_add(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_tags(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_similar(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_effect(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_del(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playlist_add(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *playhistory_upload(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *personal_fm(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *pc_diantai(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *lyric(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_week_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_vip_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_rank_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_daily_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_album_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *longaudio_album_audios(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_wx_create(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_wx_check(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_token(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_qr_key(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_qr_create(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_qr_check(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_openplat(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_device(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login_cellphone(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *login(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *lastest_songs_listen(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *krm_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *kmr_audio_mv(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ip_zone_home(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ip_zone(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ip_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ip_dateil(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *images_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *images(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *fm_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *fm_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *fm_image(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *fm_class(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *favorite_count(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *everyday_style_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *everyday_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *everyday_history(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *everyday_friend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_music_hotword(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_music_classify(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_floor(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_count(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *comment_album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *captcha_sent(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *brush(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *audio_related(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *audio_ktv_total(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *audio_accompany_matching(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_videos(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_unfollow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_lists(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_honour(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_follow_newsongs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_follow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_audios(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *artist_albums(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *album_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *album_shop(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *album_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
char *ai_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env);
```

## 项目结构

```bash
KouGouMusicLibrary/
│
├─ node_modules/
├─ package.json
├─ vcpkg-configuration.json
├─ vcpkg.json
├─ webpack.config.js       # Webpack打包配置
├─ src/
│   ├─ js/
│   │   ├─ index.js        # JS入口文件
│   │   ├─ axios_bridge.js # Axios桥接工具
│   │   └─ modulesDefinitions.js # 静态模块导出定义文件
│   ├─ c/
│   │   ├─ http.c          # HTTP支持代码
│   │   ├─ kugou_music_api.c # 主要方法定义模块
│   │   ├─ main.c          # 主程序入口
│   │   ├─ test.c          # 测试代码
│   │   └─ tinythread.c    # 线程支持（第三方）
│   └─ include/
│       ├─ app.bundle.h
│       ├─ kugou_music_api.h # API头文件
│       └─ tinycthread.h
│
├─ quickjs/                # QuickJS子模块（源码或预编译）
│
├─ build/                  # 构建输出目录
│
├─ bundle/                 # 打包产物（中间产物）
│   ├─ js/
│   │   └─ app.bundle.js   # webpack 打包js产物
│   └─ c/
│       └─ app.bundle.c    # qjsc 编译 js 生成的 c模块
│
├─ KuGouMucisApi/          # 酷狗音乐API node 服务 子模块
│
├─ vcpkg_installed/
│
├─ CMakeLists.txt          # CMake主配置文件
│
└─ CMakePresets.json       # CMake预设配置
```

## 构建指南

### 1. JavaScript 打包

使用 Webpack 打包 JavaScript 文件：

```bash
pnpm i
npx webpack
```

### 2. C 库构建

使用 CMake 构建 C 库：

```bash
cmake --preset x64-windows-vcpkg
cmake --build --preset x64-windows-vcpkg-Release --target qjsc  # 手动构建qjsc依赖
cmake --build --preset x64-windows-vcpkg-Release
```

## 注意事项

- **线程安全性**: `JSContext` 不能跨线程使用。
- **内存管理**: 所有返回的字符串需要使用 `free` 释放内存。
- **依赖管理**: 确保正确安装 `vcpkg` 和 `pnpm`。

## 贡献

欢迎提交 Issue 和 Pull Request！
