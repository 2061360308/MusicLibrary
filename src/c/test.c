#include <stdio.h>
#include <stdlib.h>
#include <engine.h>
#include "kugou_music_api.h"
#include "ncm_music_api.h"

int main(int argc, char **argv)
{
  init_engine();

  printf("Engine initialized.\n");

  // KugouProcessEnv env = {
  //     .platform = "lite", //lite: 酷狗概念版, 默认为手机版留空字符串
  //     // 设备id(可选，建议固定 )
  //     // KUGOU_API_GUID 建议为 uuidv4
  //     // KUGOU_API_DEV 建议长度为 10 位的字符串
  //     .KUGOU_API_GUID = "45593bdf-a805-4c68-80fb-c526733fa49b",
  //     .KUGOU_API_DEV = "iz6v67tm56",
  //     .KUGOU_API_MAC = "42:69:CB:E2:0D:DE",  // MAC地址
  // };

  // JSContext *ctx = kugou_init(&env);
  // // // JSContext *ctx2 = get_context(); 创建第二个上下文

  // KugouAPI *api = create_kugou_api();

  // if (!ctx)
  // {
  //   printf("Failed to initialize kugou context\n");
  //   return 1;
  // }

  // const char *cookies = "";
  // const char *params = "{}";

  // const char *response = api->top_song(ctx, cookies, params, NULL);
  // if (response)
  // {
  //   FILE *fp = fopen("response.txt", "w");
  //   if (fp)
  //   {
  //     fprintf(fp, "%s", response);
  //     fclose(fp);
  //   }
  //   printf("Response: %s\n", response);
  //   free((void *)response);
  // }
  // else
  // {
  //   printf("No response or error occurred.\n");
  // }

  // const char *params2 = "{\"ids\":\"collection_3_1863870844_4_0,collection_3_2093906551_8_0\"}";
  // const char *response2 = api->playlist_detail(ctx, cookies, params2, NULL);
  // if (response2)
  // {
  //   FILE *fp = fopen("response2.txt", "w");
  //   if (fp)
  //   {
  //     fprintf(fp, "%s", response2);
  //     fclose(fp);
  //   }
  //   printf("Response2: %s\n", response2);
  //   free((void *)response2);
  // }
  // else
  // {
  //   printf("No response2 or error occurred.\n");
  // }

  // kugou_destroy();

  JSContext *ncmCtx = ncm_init(NULL);

  // const char *route = "/top/song";

  // const char *response2 = ncm_request(ncmCtx, route, "", "{\"type\": \"96\"}");
  // printf("NCM Response: %s\n", response2);

  NCMAPI *ncmApi = create_ncm_api();

  const char *response3 = ncmApi->top_song(ncmCtx, "", "{\"id\": \"96\"}", NULL);\

  printf("NCM API Response: %s\n", response3);

  ncm_destroy();

  destroy_engine();
}