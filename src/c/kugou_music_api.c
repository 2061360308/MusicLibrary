#include "quickjs.h"
#include "kugou_music_api.h"

char *yueku_fm(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/yueku/fm", cookies, params, env);
}

char *yueku_banner(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/yueku/banner", cookies, params, env);
}

char *yueku(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/yueku", cookies, params, env);
}

char *youth_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/vip", cookies, params, env);
}

char *youth_user_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/user/song", cookies, params, env);
}

char *youth_union_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/union/vip", cookies, params, env);
}

char *youth_month_vip_record(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/month/vip/record", cookies, params, env);
}

char *youth_listen_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/listen/song", cookies, params, env);
}

char *youth_dynamic_recent(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/dynamic/recent", cookies, params, env);
}

char *youth_dynamic(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/dynamic", cookies, params, env);
}

char *youth_day_vip_upgrade(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/day/vip/upgrade", cookies, params, env);
}

char *youth_day_vip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/day/vip", cookies, params, env);
}

char *youth_channel_sub(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/sub", cookies, params, env);
}

char *youth_channel_song_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/song/detail", cookies, params, env);
}

char *youth_channel_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/song", cookies, params, env);
}

char *youth_channel_similar(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/similar", cookies, params, env);
}

char *youth_channel_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/detail", cookies, params, env);
}

char *youth_channel_amway(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/amway", cookies, params, env);
}

char *youth_channel_all(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/youth/channel/all", cookies, params, env);
}

char *video_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/video/url", cookies, params, env);
}

char *video_privilege(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/video/privilege", cookies, params, env);
}

char *video_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/video/detail", cookies, params, env);
}

char *user_vip_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/vip/detail", cookies, params, env);
}

char *user_video_love(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/video/love", cookies, params, env);
}

char *user_video_collect(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/video/collect", cookies, params, env);
}

char *user_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/playlist", cookies, params, env);
}

char *user_listen(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/listen", cookies, params, env);
}

char *user_history(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/history", cookies, params, env);
}

char *user_follow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/follow", cookies, params, env);
}

char *user_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/detail", cookies, params, env);
}

char *user_cloud_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/cloud/url", cookies, params, env);
}

char *user_cloud(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/user/cloud", cookies, params, env);
}

char *top_song(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/song", cookies, params, env);
}

char *top_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/playlist", cookies, params, env);
}

char *top_ip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/ip", cookies, params, env);
}

char *top_card_youth(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/card/youth", cookies, params, env);
}

char *top_card(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/card", cookies, params, env);
}

char *top_album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/top/album", cookies, params, env);
}

char *theme_playlist_track(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/theme/playlist/track", cookies, params, env);
}

char *theme_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/theme/playlist", cookies, params, env);
}

char *theme_music_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/theme/music/detail", cookies, params, env);
}

char *theme_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/theme/music", cookies, params, env);
}

char *song_url_new(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/song/url/new", cookies, params, env);
}

char *song_url(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/song/url", cookies, params, env);
}

char *song_ranking_filter(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/song/ranking/filter", cookies, params, env);
}

char *song_ranking(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/song/ranking", cookies, params, env);
}

char *song_climax(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/song/climax", cookies, params, env);
}

char *singer_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/singer/list", cookies, params, env);
}

char *sheet_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/sheet/list", cookies, params, env);
}

char *sheet_hot(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/sheet/hot", cookies, params, env);
}

char *sheet_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/sheet/detail", cookies, params, env);
}

char *sheet_collection_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/sheet/collection/detail", cookies, params, env);
}

char *sheet_collection(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/sheet/collection", cookies, params, env);
}

char *server_now(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/server/now", cookies, params, env);
}

char *search_suggest(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/suggest", cookies, params, env);
}

char *search_mixed(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/mixed", cookies, params, env);
}

char *search_lyric(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/lyric", cookies, params, env);
}

char *search_hot(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/hot", cookies, params, env);
}

char *search_default(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/default", cookies, params, env);
}

char *search_complex(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search/complex", cookies, params, env);
}

char *search(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/search", cookies, params, env);
}

char *scene_video_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/video/list", cookies, params, env);
}

char *scene_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/music", cookies, params, env);
}

char *scene_module_info(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/module/info", cookies, params, env);
}

char *scene_module(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/module", cookies, params, env);
}

char *scene_lists_v2(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/lists/v2", cookies, params, env);
}

char *scene_lists(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/lists", cookies, params, env);
}

char *scene_collection_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/collection/list", cookies, params, env);
}

char *scene_audio_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/scene/audio/list", cookies, params, env);
}

char *register_dev(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/register/dev", cookies, params, env);
}

char *recommend_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/recommend/songs", cookies, params, env);
}

char *rank_vol(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/rank/vol", cookies, params, env);
}

char *rank_top(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/rank/top", cookies, params, env);
}

char *rank_list(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/rank/list", cookies, params, env);
}

char *rank_info(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/rank/info", cookies, params, env);
}

char *rank_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/rank/audio", cookies, params, env);
}

char *privilege_lite(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/privilege/lite", cookies, params, env);
}

char *playlist_track_all_new(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/track/all/new", cookies, params, env);
}

char *playlist_track_all(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/track/all", cookies, params, env);
}

char *playlist_tracks_del(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/tracks/del", cookies, params, env);
}

char *playlist_tracks_add(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/tracks/add", cookies, params, env);
}

char *playlist_tags(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/tags", cookies, params, env);
}

char *playlist_similar(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/similar", cookies, params, env);
}

char *playlist_effect(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/effect", cookies, params, env);
}

char *playlist_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/detail", cookies, params, env);
}

char *playlist_del(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/del", cookies, params, env);
}

char *playlist_add(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playlist/add", cookies, params, env);
}

char *playhistory_upload(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/playhistory/upload", cookies, params, env);
}

char *personal_fm(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/personal/fm", cookies, params, env);
}

char *pc_diantai(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/pc/diantai", cookies, params, env);
}

char *lyric(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/lyric", cookies, params, env);
}

char *longaudio_week_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/week/recommend", cookies, params, env);
}

char *longaudio_vip_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/vip/recommend", cookies, params, env);
}

char *longaudio_rank_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/rank/recommend", cookies, params, env);
}

char *longaudio_daily_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/daily/recommend", cookies, params, env);
}

char *longaudio_album_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/album/detail", cookies, params, env);
}

char *longaudio_album_audios(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/longaudio/album/audios", cookies, params, env);
}

char *login_wx_create(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/wx/create", cookies, params, env);
}

char *login_wx_check(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/wx/check", cookies, params, env);
}

char *login_token(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/token", cookies, params, env);
}

char *login_qr_key(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/qr/key", cookies, params, env);
}

char *login_qr_create(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/qr/create", cookies, params, env);
}

char *login_qr_check(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/qr/check", cookies, params, env);
}

char *login_openplat(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/openplat", cookies, params, env);
}

char *login_device(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/device", cookies, params, env);
}

char *login_cellphone(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login/cellphone", cookies, params, env);
}

char *login(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/login", cookies, params, env);
}

char *lastest_songs_listen(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/lastest/songs/listen", cookies, params, env);
}

char *krm_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/krm/audio", cookies, params, env);
}

char *kmr_audio_mv(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/kmr/audio/mv", cookies, params, env);
}

char *ip_zone_home(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ip/zone/home", cookies, params, env);
}

char *ip_zone(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ip/zone", cookies, params, env);
}

char *ip_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ip/playlist", cookies, params, env);
}

char *ip_dateil(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ip/dateil", cookies, params, env);
}

char *ip(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ip", cookies, params, env);
}

char *images_audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/images/audio", cookies, params, env);
}

char *images(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/images", cookies, params, env);
}

char *fm_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/fm/songs", cookies, params, env);
}

char *fm_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/fm/recommend", cookies, params, env);
}

char *fm_image(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/fm/image", cookies, params, env);
}

char *fm_class(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/fm/class", cookies, params, env);
}

char *favorite_count(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/favorite/count", cookies, params, env);
}

char *everyday_style_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/everyday/style/recommend", cookies, params, env);
}

char *everyday_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/everyday/recommend", cookies, params, env);
}

char *everyday_history(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/everyday/history", cookies, params, env);
}

char *everyday_friend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/everyday/friend", cookies, params, env);
}

char *comment_playlist(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/playlist", cookies, params, env);
}

char *comment_music_hotword(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/music/hotword", cookies, params, env);
}

char *comment_music_classify(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/music/classify", cookies, params, env);
}

char *comment_music(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/music", cookies, params, env);
}

char *comment_floor(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/floor", cookies, params, env);
}

char *comment_count(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/count", cookies, params, env);
}

char *comment_album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/comment/album", cookies, params, env);
}

char *captcha_sent(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/captcha/sent", cookies, params, env);
}

char *brush(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/brush", cookies, params, env);
}

char *audio_related(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/audio/related", cookies, params, env);
}

char *audio_ktv_total(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/audio/ktv/total", cookies, params, env);
}

char *audio_accompany_matching(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/audio/accompany/matching", cookies, params, env);
}

char *audio(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/audio", cookies, params, env);
}

char *artist_videos(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/videos", cookies, params, env);
}

char *artist_unfollow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/unfollow", cookies, params, env);
}

char *artist_lists(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/lists", cookies, params, env);
}

char *artist_honour(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/honour", cookies, params, env);
}

char *artist_follow_newsongs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/follow/newsongs", cookies, params, env);
}

char *artist_follow(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/follow", cookies, params, env);
}

char *artist_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/detail", cookies, params, env);
}

char *artist_audios(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/audios", cookies, params, env);
}

char *artist_albums(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/artist/albums", cookies, params, env);
}

char *album_songs(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/album/songs", cookies, params, env);
}

char *album_shop(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/album/shop", cookies, params, env);
}

char *album_detail(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/album/detail", cookies, params, env);
}

char *album(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/album", cookies, params, env);
}

char *ai_recommend(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {
    return request(ctx, "/ai/recommend", cookies, params, env);
}
