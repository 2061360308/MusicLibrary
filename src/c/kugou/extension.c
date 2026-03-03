#include "quickjs.h"
#include "kugou_music_api.h"

#define API_FUNC(name, route)  \
    static char *name(JSContext *ctx, const char *cookies, const char *params, ProcessEnv *env) {  \
        return kugou_request(ctx, route, cookies, params, env);  \
    }

#define API_FUNC_NOENV(name, route)  \
    static char *name(JSContext *ctx, const char *cookies, const char *params) {  \
        return kugou_request_simple(ctx, route, cookies, params);  \
    }

API_FUNC(yueku_fm, "/yueku/fm")
API_FUNC_NOENV(yueku_fm_simple, "/yueku/fm")
API_FUNC(yueku_banner, "/yueku/banner")
API_FUNC_NOENV(yueku_banner_simple, "/yueku/banner")
API_FUNC(yueku, "/yueku")
API_FUNC_NOENV(yueku_simple, "/yueku")
API_FUNC(youth_vip, "/youth/vip")
API_FUNC_NOENV(youth_vip_simple, "/youth/vip")
API_FUNC(youth_user_song, "/youth/user/song")
API_FUNC_NOENV(youth_user_song_simple, "/youth/user/song")
API_FUNC(youth_union_vip, "/youth/union/vip")
API_FUNC_NOENV(youth_union_vip_simple, "/youth/union/vip")
API_FUNC(youth_month_vip_record, "/youth/month/vip/record")
API_FUNC_NOENV(youth_month_vip_record_simple, "/youth/month/vip/record")
API_FUNC(youth_listen_song, "/youth/listen/song")
API_FUNC_NOENV(youth_listen_song_simple, "/youth/listen/song")
API_FUNC(youth_dynamic_recent, "/youth/dynamic/recent")
API_FUNC_NOENV(youth_dynamic_recent_simple, "/youth/dynamic/recent")
API_FUNC(youth_dynamic, "/youth/dynamic")
API_FUNC_NOENV(youth_dynamic_simple, "/youth/dynamic")
API_FUNC(youth_day_vip_upgrade, "/youth/day/vip/upgrade")
API_FUNC_NOENV(youth_day_vip_upgrade_simple, "/youth/day/vip/upgrade")
API_FUNC(youth_day_vip, "/youth/day/vip")
API_FUNC_NOENV(youth_day_vip_simple, "/youth/day/vip")
API_FUNC(youth_channel_sub, "/youth/channel/sub")
API_FUNC_NOENV(youth_channel_sub_simple, "/youth/channel/sub")
API_FUNC(youth_channel_song_detail, "/youth/channel/song/detail")
API_FUNC_NOENV(youth_channel_song_detail_simple, "/youth/channel/song/detail")
API_FUNC(youth_channel_song, "/youth/channel/song")
API_FUNC_NOENV(youth_channel_song_simple, "/youth/channel/song")
API_FUNC(youth_channel_similar, "/youth/channel/similar")
API_FUNC_NOENV(youth_channel_similar_simple, "/youth/channel/similar")
API_FUNC(youth_channel_detail, "/youth/channel/detail")
API_FUNC_NOENV(youth_channel_detail_simple, "/youth/channel/detail")
API_FUNC(youth_channel_amway, "/youth/channel/amway")
API_FUNC_NOENV(youth_channel_amway_simple, "/youth/channel/amway")
API_FUNC(youth_channel_all, "/youth/channel/all")
API_FUNC_NOENV(youth_channel_all_simple, "/youth/channel/all")
API_FUNC(video_url, "/video/url")
API_FUNC_NOENV(video_url_simple, "/video/url")
API_FUNC(video_privilege, "/video/privilege")
API_FUNC_NOENV(video_privilege_simple, "/video/privilege")
API_FUNC(video_detail, "/video/detail")
API_FUNC_NOENV(video_detail_simple, "/video/detail")
API_FUNC(user_vip_detail, "/user/vip/detail")
API_FUNC_NOENV(user_vip_detail_simple, "/user/vip/detail")
API_FUNC(user_video_love, "/user/video/love")
API_FUNC_NOENV(user_video_love_simple, "/user/video/love")
API_FUNC(user_video_collect, "/user/video/collect")
API_FUNC_NOENV(user_video_collect_simple, "/user/video/collect")
API_FUNC(user_playlist, "/user/playlist")
API_FUNC_NOENV(user_playlist_simple, "/user/playlist")
API_FUNC(user_listen, "/user/listen")
API_FUNC_NOENV(user_listen_simple, "/user/listen")
API_FUNC(user_history, "/user/history")
API_FUNC_NOENV(user_history_simple, "/user/history")
API_FUNC(user_follow, "/user/follow")
API_FUNC_NOENV(user_follow_simple, "/user/follow")
API_FUNC(user_detail, "/user/detail")
API_FUNC_NOENV(user_detail_simple, "/user/detail")
API_FUNC(user_cloud_url, "/user/cloud/url")
API_FUNC_NOENV(user_cloud_url_simple, "/user/cloud/url")
API_FUNC(user_cloud, "/user/cloud")
API_FUNC_NOENV(user_cloud_simple, "/user/cloud")
API_FUNC(top_song, "/top/song")
API_FUNC_NOENV(top_song_simple, "/top/song")
API_FUNC(top_playlist, "/top/playlist")
API_FUNC_NOENV(top_playlist_simple, "/top/playlist")
API_FUNC(top_ip, "/top/ip")
API_FUNC_NOENV(top_ip_simple, "/top/ip")
API_FUNC(top_card_youth, "/top/card/youth")
API_FUNC_NOENV(top_card_youth_simple, "/top/card/youth")
API_FUNC(top_card, "/top/card")
API_FUNC_NOENV(top_card_simple, "/top/card")
API_FUNC(top_album, "/top/album")
API_FUNC_NOENV(top_album_simple, "/top/album")
API_FUNC(theme_playlist_track, "/theme/playlist/track")
API_FUNC_NOENV(theme_playlist_track_simple, "/theme/playlist/track")
API_FUNC(theme_playlist, "/theme/playlist")
API_FUNC_NOENV(theme_playlist_simple, "/theme/playlist")
API_FUNC(theme_music_detail, "/theme/music/detail")
API_FUNC_NOENV(theme_music_detail_simple, "/theme/music/detail")
API_FUNC(theme_music, "/theme/music")
API_FUNC_NOENV(theme_music_simple, "/theme/music")
API_FUNC(song_url_new, "/song/url/new")
API_FUNC_NOENV(song_url_new_simple, "/song/url/new")
API_FUNC(song_url, "/song/url")
API_FUNC_NOENV(song_url_simple, "/song/url")
API_FUNC(song_ranking_filter, "/song/ranking/filter")
API_FUNC_NOENV(song_ranking_filter_simple, "/song/ranking/filter")
API_FUNC(song_ranking, "/song/ranking")
API_FUNC_NOENV(song_ranking_simple, "/song/ranking")
API_FUNC(song_climax, "/song/climax")
API_FUNC_NOENV(song_climax_simple, "/song/climax")
API_FUNC(singer_list, "/singer/list")
API_FUNC_NOENV(singer_list_simple, "/singer/list")
API_FUNC(sheet_list, "/sheet/list")
API_FUNC_NOENV(sheet_list_simple, "/sheet/list")
API_FUNC(sheet_hot, "/sheet/hot")
API_FUNC_NOENV(sheet_hot_simple, "/sheet/hot")
API_FUNC(sheet_detail, "/sheet/detail")
API_FUNC_NOENV(sheet_detail_simple, "/sheet/detail")
API_FUNC(sheet_collection_detail, "/sheet/collection/detail")
API_FUNC_NOENV(sheet_collection_detail_simple, "/sheet/collection/detail")
API_FUNC(sheet_collection, "/sheet/collection")
API_FUNC_NOENV(sheet_collection_simple, "/sheet/collection")
API_FUNC(server_now, "/server/now")
API_FUNC_NOENV(server_now_simple, "/server/now")
API_FUNC(search_suggest, "/search/suggest")
API_FUNC_NOENV(search_suggest_simple, "/search/suggest")
API_FUNC(search_mixed, "/search/mixed")
API_FUNC_NOENV(search_mixed_simple, "/search/mixed")
API_FUNC(search_lyric, "/search/lyric")
API_FUNC_NOENV(search_lyric_simple, "/search/lyric")
API_FUNC(search_hot, "/search/hot")
API_FUNC_NOENV(search_hot_simple, "/search/hot")
API_FUNC(search_default, "/search/default")
API_FUNC_NOENV(search_default_simple, "/search/default")
API_FUNC(search_complex, "/search/complex")
API_FUNC_NOENV(search_complex_simple, "/search/complex")
API_FUNC(search, "/search")
API_FUNC_NOENV(search_simple, "/search")
API_FUNC(scene_video_list, "/scene/video/list")
API_FUNC_NOENV(scene_video_list_simple, "/scene/video/list")
API_FUNC(scene_music, "/scene/music")
API_FUNC_NOENV(scene_music_simple, "/scene/music")
API_FUNC(scene_module_info, "/scene/module/info")
API_FUNC_NOENV(scene_module_info_simple, "/scene/module/info")
API_FUNC(scene_module, "/scene/module")
API_FUNC_NOENV(scene_module_simple, "/scene/module")
API_FUNC(scene_lists_v2, "/scene/lists/v2")
API_FUNC_NOENV(scene_lists_v2_simple, "/scene/lists/v2")
API_FUNC(scene_lists, "/scene/lists")
API_FUNC_NOENV(scene_lists_simple, "/scene/lists")
API_FUNC(scene_collection_list, "/scene/collection/list")
API_FUNC_NOENV(scene_collection_list_simple, "/scene/collection/list")
API_FUNC(scene_audio_list, "/scene/audio/list")
API_FUNC_NOENV(scene_audio_list_simple, "/scene/audio/list")
API_FUNC(register_dev, "/register/dev")
API_FUNC_NOENV(register_dev_simple, "/register/dev")
API_FUNC(recommend_songs, "/recommend/songs")
API_FUNC_NOENV(recommend_songs_simple, "/recommend/songs")
API_FUNC(rank_vol, "/rank/vol")
API_FUNC_NOENV(rank_vol_simple, "/rank/vol")
API_FUNC(rank_top, "/rank/top")
API_FUNC_NOENV(rank_top_simple, "/rank/top")
API_FUNC(rank_list, "/rank/list")
API_FUNC_NOENV(rank_list_simple, "/rank/list")
API_FUNC(rank_info, "/rank/info")
API_FUNC_NOENV(rank_info_simple, "/rank/info")
API_FUNC(rank_audio, "/rank/audio")
API_FUNC_NOENV(rank_audio_simple, "/rank/audio")
API_FUNC(privilege_lite, "/privilege/lite")
API_FUNC_NOENV(privilege_lite_simple, "/privilege/lite")
API_FUNC(playlist_track_all_new, "/playlist/track/all/new")
API_FUNC_NOENV(playlist_track_all_new_simple, "/playlist/track/all/new")
API_FUNC(playlist_track_all, "/playlist/track/all")
API_FUNC_NOENV(playlist_track_all_simple, "/playlist/track/all")
API_FUNC(playlist_tracks_del, "/playlist/tracks/del")
API_FUNC_NOENV(playlist_tracks_del_simple, "/playlist/tracks/del")
API_FUNC(playlist_tracks_add, "/playlist/tracks/add")
API_FUNC_NOENV(playlist_tracks_add_simple, "/playlist/tracks/add")
API_FUNC(playlist_tags, "/playlist/tags")
API_FUNC_NOENV(playlist_tags_simple, "/playlist/tags")
API_FUNC(playlist_similar, "/playlist/similar")
API_FUNC_NOENV(playlist_similar_simple, "/playlist/similar")
API_FUNC(playlist_effect, "/playlist/effect")
API_FUNC_NOENV(playlist_effect_simple, "/playlist/effect")
API_FUNC(playlist_detail, "/playlist/detail")
API_FUNC_NOENV(playlist_detail_simple, "/playlist/detail")
API_FUNC(playlist_del, "/playlist/del")
API_FUNC_NOENV(playlist_del_simple, "/playlist/del")
API_FUNC(playlist_add, "/playlist/add")
API_FUNC_NOENV(playlist_add_simple, "/playlist/add")
API_FUNC(playhistory_upload, "/playhistory/upload")
API_FUNC_NOENV(playhistory_upload_simple, "/playhistory/upload")
API_FUNC(personal_fm, "/personal/fm")
API_FUNC_NOENV(personal_fm_simple, "/personal/fm")
API_FUNC(pc_diantai, "/pc/diantai")
API_FUNC_NOENV(pc_diantai_simple, "/pc/diantai")
API_FUNC(lyric, "/lyric")
API_FUNC_NOENV(lyric_simple, "/lyric")
API_FUNC(longaudio_week_recommend, "/longaudio/week/recommend")
API_FUNC_NOENV(longaudio_week_recommend_simple, "/longaudio/week/recommend")
API_FUNC(longaudio_vip_recommend, "/longaudio/vip/recommend")
API_FUNC_NOENV(longaudio_vip_recommend_simple, "/longaudio/vip/recommend")
API_FUNC(longaudio_rank_recommend, "/longaudio/rank/recommend")
API_FUNC_NOENV(longaudio_rank_recommend_simple, "/longaudio/rank/recommend")
API_FUNC(longaudio_daily_recommend, "/longaudio/daily/recommend")
API_FUNC_NOENV(longaudio_daily_recommend_simple, "/longaudio/daily/recommend")
API_FUNC(longaudio_album_detail, "/longaudio/album/detail")
API_FUNC_NOENV(longaudio_album_detail_simple, "/longaudio/album/detail")
API_FUNC(longaudio_album_audios, "/longaudio/album/audios")
API_FUNC_NOENV(longaudio_album_audios_simple, "/longaudio/album/audios")
API_FUNC(login_wx_create, "/login/wx/create")
API_FUNC_NOENV(login_wx_create_simple, "/login/wx/create")
API_FUNC(login_wx_check, "/login/wx/check")
API_FUNC_NOENV(login_wx_check_simple, "/login/wx/check")
API_FUNC(login_token, "/login/token")
API_FUNC_NOENV(login_token_simple, "/login/token")
API_FUNC(login_qr_key, "/login/qr/key")
API_FUNC_NOENV(login_qr_key_simple, "/login/qr/key")
API_FUNC(login_qr_create, "/login/qr/create")
API_FUNC_NOENV(login_qr_create_simple, "/login/qr/create")
API_FUNC(login_qr_check, "/login/qr/check")
API_FUNC_NOENV(login_qr_check_simple, "/login/qr/check")
API_FUNC(login_openplat, "/login/openplat")
API_FUNC_NOENV(login_openplat_simple, "/login/openplat")
API_FUNC(login_device, "/login/device")
API_FUNC_NOENV(login_device_simple, "/login/device")
API_FUNC(login_cellphone, "/login/cellphone")
API_FUNC_NOENV(login_cellphone_simple, "/login/cellphone")
API_FUNC(login, "/login")
API_FUNC_NOENV(login_simple, "/login")
API_FUNC(lastest_songs_listen, "/lastest/songs/listen")
API_FUNC_NOENV(lastest_songs_listen_simple, "/lastest/songs/listen")
API_FUNC(krm_audio, "/krm/audio")
API_FUNC_NOENV(krm_audio_simple, "/krm/audio")
API_FUNC(kmr_audio_mv, "/kmr/audio/mv")
API_FUNC_NOENV(kmr_audio_mv_simple, "/kmr/audio/mv")
API_FUNC(ip_zone_home, "/ip/zone/home")
API_FUNC_NOENV(ip_zone_home_simple, "/ip/zone/home")
API_FUNC(ip_zone, "/ip/zone")
API_FUNC_NOENV(ip_zone_simple, "/ip/zone")
API_FUNC(ip_playlist, "/ip/playlist")
API_FUNC_NOENV(ip_playlist_simple, "/ip/playlist")
API_FUNC(ip_dateil, "/ip/dateil")
API_FUNC_NOENV(ip_dateil_simple, "/ip/dateil")
API_FUNC(ip, "/ip")
API_FUNC_NOENV(ip_simple, "/ip")
API_FUNC(images_audio, "/images/audio")
API_FUNC_NOENV(images_audio_simple, "/images/audio")
API_FUNC(images, "/images")
API_FUNC_NOENV(images_simple, "/images")
API_FUNC(fm_songs, "/fm/songs")
API_FUNC_NOENV(fm_songs_simple, "/fm/songs")
API_FUNC(fm_recommend, "/fm/recommend")
API_FUNC_NOENV(fm_recommend_simple, "/fm/recommend")
API_FUNC(fm_image, "/fm/image")
API_FUNC_NOENV(fm_image_simple, "/fm/image")
API_FUNC(fm_class, "/fm/class")
API_FUNC_NOENV(fm_class_simple, "/fm/class")
API_FUNC(favorite_count, "/favorite/count")
API_FUNC_NOENV(favorite_count_simple, "/favorite/count")
API_FUNC(everyday_style_recommend, "/everyday/style/recommend")
API_FUNC_NOENV(everyday_style_recommend_simple, "/everyday/style/recommend")
API_FUNC(everyday_recommend, "/everyday/recommend")
API_FUNC_NOENV(everyday_recommend_simple, "/everyday/recommend")
API_FUNC(everyday_history, "/everyday/history")
API_FUNC_NOENV(everyday_history_simple, "/everyday/history")
API_FUNC(everyday_friend, "/everyday/friend")
API_FUNC_NOENV(everyday_friend_simple, "/everyday/friend")
API_FUNC(comment_playlist, "/comment/playlist")
API_FUNC_NOENV(comment_playlist_simple, "/comment/playlist")
API_FUNC(comment_music_hotword, "/comment/music/hotword")
API_FUNC_NOENV(comment_music_hotword_simple, "/comment/music/hotword")
API_FUNC(comment_music_classify, "/comment/music/classify")
API_FUNC_NOENV(comment_music_classify_simple, "/comment/music/classify")
API_FUNC(comment_music, "/comment/music")
API_FUNC_NOENV(comment_music_simple, "/comment/music")
API_FUNC(comment_floor, "/comment/floor")
API_FUNC_NOENV(comment_floor_simple, "/comment/floor")
API_FUNC(comment_count, "/comment/count")
API_FUNC_NOENV(comment_count_simple, "/comment/count")
API_FUNC(comment_album, "/comment/album")
API_FUNC_NOENV(comment_album_simple, "/comment/album")
API_FUNC(captcha_sent, "/captcha/sent")
API_FUNC_NOENV(captcha_sent_simple, "/captcha/sent")
API_FUNC(brush, "/brush")
API_FUNC_NOENV(brush_simple, "/brush")
API_FUNC(audio_related, "/audio/related")
API_FUNC_NOENV(audio_related_simple, "/audio/related")
API_FUNC(audio_ktv_total, "/audio/ktv/total")
API_FUNC_NOENV(audio_ktv_total_simple, "/audio/ktv/total")
API_FUNC(audio_accompany_matching, "/audio/accompany/matching")
API_FUNC_NOENV(audio_accompany_matching_simple, "/audio/accompany/matching")
API_FUNC(audio, "/audio")
API_FUNC_NOENV(audio_simple, "/audio")
API_FUNC(artist_videos, "/artist/videos")
API_FUNC_NOENV(artist_videos_simple, "/artist/videos")
API_FUNC(artist_unfollow, "/artist/unfollow")
API_FUNC_NOENV(artist_unfollow_simple, "/artist/unfollow")
API_FUNC(artist_lists, "/artist/lists")
API_FUNC_NOENV(artist_lists_simple, "/artist/lists")
API_FUNC(artist_honour, "/artist/honour")
API_FUNC_NOENV(artist_honour_simple, "/artist/honour")
API_FUNC(artist_follow_newsongs, "/artist/follow/newsongs")
API_FUNC_NOENV(artist_follow_newsongs_simple, "/artist/follow/newsongs")
API_FUNC(artist_follow, "/artist/follow")
API_FUNC_NOENV(artist_follow_simple, "/artist/follow")
API_FUNC(artist_detail, "/artist/detail")
API_FUNC_NOENV(artist_detail_simple, "/artist/detail")
API_FUNC(artist_audios, "/artist/audios")
API_FUNC_NOENV(artist_audios_simple, "/artist/audios")
API_FUNC(artist_albums, "/artist/albums")
API_FUNC_NOENV(artist_albums_simple, "/artist/albums")
API_FUNC(album_songs, "/album/songs")
API_FUNC_NOENV(album_songs_simple, "/album/songs")
API_FUNC(album_shop, "/album/shop")
API_FUNC_NOENV(album_shop_simple, "/album/shop")
API_FUNC(album_detail, "/album/detail")
API_FUNC_NOENV(album_detail_simple, "/album/detail")
API_FUNC(album, "/album")
API_FUNC_NOENV(album_simple, "/album")
API_FUNC(ai_recommend, "/ai/recommend")
API_FUNC_NOENV(ai_recommend_simple, "/ai/recommend")

KugouAPI *create_kugou_api(){
    KugouAPI *api = malloc(sizeof(KugouAPI));
    api->yueku_fm = yueku_fm;
    api->yueku_fm_simple = yueku_fm_simple;
    api->yueku_banner = yueku_banner;
    api->yueku_banner_simple = yueku_banner_simple;
    api->yueku = yueku;
    api->yueku_simple = yueku_simple;
    api->youth_vip = youth_vip;
    api->youth_vip_simple = youth_vip_simple;
    api->youth_user_song = youth_user_song;
    api->youth_user_song_simple = youth_user_song_simple;
    api->youth_union_vip = youth_union_vip;
    api->youth_union_vip_simple = youth_union_vip_simple;
    api->youth_month_vip_record = youth_month_vip_record;
    api->youth_month_vip_record_simple = youth_month_vip_record_simple;
    api->youth_listen_song = youth_listen_song;
    api->youth_listen_song_simple = youth_listen_song_simple;
    api->youth_dynamic_recent = youth_dynamic_recent;
    api->youth_dynamic_recent_simple = youth_dynamic_recent_simple;
    api->youth_dynamic = youth_dynamic;
    api->youth_dynamic_simple = youth_dynamic_simple;
    api->youth_day_vip_upgrade = youth_day_vip_upgrade;
    api->youth_day_vip_upgrade_simple = youth_day_vip_upgrade_simple;
    api->youth_day_vip = youth_day_vip;
    api->youth_day_vip_simple = youth_day_vip_simple;
    api->youth_channel_sub = youth_channel_sub;
    api->youth_channel_sub_simple = youth_channel_sub_simple;
    api->youth_channel_song_detail = youth_channel_song_detail;
    api->youth_channel_song_detail_simple = youth_channel_song_detail_simple;
    api->youth_channel_song = youth_channel_song;
    api->youth_channel_song_simple = youth_channel_song_simple;
    api->youth_channel_similar = youth_channel_similar;
    api->youth_channel_similar_simple = youth_channel_similar_simple;
    api->youth_channel_detail = youth_channel_detail;
    api->youth_channel_detail_simple = youth_channel_detail_simple;
    api->youth_channel_amway = youth_channel_amway;
    api->youth_channel_amway_simple = youth_channel_amway_simple;
    api->youth_channel_all = youth_channel_all;
    api->youth_channel_all_simple = youth_channel_all_simple;
    api->video_url = video_url;
    api->video_url_simple = video_url_simple;
    api->video_privilege = video_privilege;
    api->video_privilege_simple = video_privilege_simple;
    api->video_detail = video_detail;
    api->video_detail_simple = video_detail_simple;
    api->user_vip_detail = user_vip_detail;
    api->user_vip_detail_simple = user_vip_detail_simple;
    api->user_video_love = user_video_love;
    api->user_video_love_simple = user_video_love_simple;
    api->user_video_collect = user_video_collect;
    api->user_video_collect_simple = user_video_collect_simple;
    api->user_playlist = user_playlist;
    api->user_playlist_simple = user_playlist_simple;
    api->user_listen = user_listen;
    api->user_listen_simple = user_listen_simple;
    api->user_history = user_history;
    api->user_history_simple = user_history_simple;
    api->user_follow = user_follow;
    api->user_follow_simple = user_follow_simple;
    api->user_detail = user_detail;
    api->user_detail_simple = user_detail_simple;
    api->user_cloud_url = user_cloud_url;
    api->user_cloud_url_simple = user_cloud_url_simple;
    api->user_cloud = user_cloud;
    api->user_cloud_simple = user_cloud_simple;
    api->top_song = top_song;
    api->top_song_simple = top_song_simple;
    api->top_playlist = top_playlist;
    api->top_playlist_simple = top_playlist_simple;
    api->top_ip = top_ip;
    api->top_ip_simple = top_ip_simple;
    api->top_card_youth = top_card_youth;
    api->top_card_youth_simple = top_card_youth_simple;
    api->top_card = top_card;
    api->top_card_simple = top_card_simple;
    api->top_album = top_album;
    api->top_album_simple = top_album_simple;
    api->theme_playlist_track = theme_playlist_track;
    api->theme_playlist_track_simple = theme_playlist_track_simple;
    api->theme_playlist = theme_playlist;
    api->theme_playlist_simple = theme_playlist_simple;
    api->theme_music_detail = theme_music_detail;
    api->theme_music_detail_simple = theme_music_detail_simple;
    api->theme_music = theme_music;
    api->theme_music_simple = theme_music_simple;
    api->song_url_new = song_url_new;
    api->song_url_new_simple = song_url_new_simple;
    api->song_url = song_url;
    api->song_url_simple = song_url_simple;
    api->song_ranking_filter = song_ranking_filter;
    api->song_ranking_filter_simple = song_ranking_filter_simple;
    api->song_ranking = song_ranking;
    api->song_ranking_simple = song_ranking_simple;
    api->song_climax = song_climax;
    api->song_climax_simple = song_climax_simple;
    api->singer_list = singer_list;
    api->singer_list_simple = singer_list_simple;
    api->sheet_list = sheet_list;
    api->sheet_list_simple = sheet_list_simple;
    api->sheet_hot = sheet_hot;
    api->sheet_hot_simple = sheet_hot_simple;
    api->sheet_detail = sheet_detail;
    api->sheet_detail_simple = sheet_detail_simple;
    api->sheet_collection_detail = sheet_collection_detail;
    api->sheet_collection_detail_simple = sheet_collection_detail_simple;
    api->sheet_collection = sheet_collection;
    api->sheet_collection_simple = sheet_collection_simple;
    api->server_now = server_now;
    api->server_now_simple = server_now_simple;
    api->search_suggest = search_suggest;
    api->search_suggest_simple = search_suggest_simple;
    api->search_mixed = search_mixed;
    api->search_mixed_simple = search_mixed_simple;
    api->search_lyric = search_lyric;
    api->search_lyric_simple = search_lyric_simple;
    api->search_hot = search_hot;
    api->search_hot_simple = search_hot_simple;
    api->search_default = search_default;
    api->search_default_simple = search_default_simple;
    api->search_complex = search_complex;
    api->search_complex_simple = search_complex_simple;
    api->search = search;
    api->search_simple = search_simple;
    api->scene_video_list = scene_video_list;
    api->scene_video_list_simple = scene_video_list_simple;
    api->scene_music = scene_music;
    api->scene_music_simple = scene_music_simple;
    api->scene_module_info = scene_module_info;
    api->scene_module_info_simple = scene_module_info_simple;
    api->scene_module = scene_module;
    api->scene_module_simple = scene_module_simple;
    api->scene_lists_v2 = scene_lists_v2;
    api->scene_lists_v2_simple = scene_lists_v2_simple;
    api->scene_lists = scene_lists;
    api->scene_lists_simple = scene_lists_simple;
    api->scene_collection_list = scene_collection_list;
    api->scene_collection_list_simple = scene_collection_list_simple;
    api->scene_audio_list = scene_audio_list;
    api->scene_audio_list_simple = scene_audio_list_simple;
    api->register_dev = register_dev;
    api->register_dev_simple = register_dev_simple;
    api->recommend_songs = recommend_songs;
    api->recommend_songs_simple = recommend_songs_simple;
    api->rank_vol = rank_vol;
    api->rank_vol_simple = rank_vol_simple;
    api->rank_top = rank_top;
    api->rank_top_simple = rank_top_simple;
    api->rank_list = rank_list;
    api->rank_list_simple = rank_list_simple;
    api->rank_info = rank_info;
    api->rank_info_simple = rank_info_simple;
    api->rank_audio = rank_audio;
    api->rank_audio_simple = rank_audio_simple;
    api->privilege_lite = privilege_lite;
    api->privilege_lite_simple = privilege_lite_simple;
    api->playlist_track_all_new = playlist_track_all_new;
    api->playlist_track_all_new_simple = playlist_track_all_new_simple;
    api->playlist_track_all = playlist_track_all;
    api->playlist_track_all_simple = playlist_track_all_simple;
    api->playlist_tracks_del = playlist_tracks_del;
    api->playlist_tracks_del_simple = playlist_tracks_del_simple;
    api->playlist_tracks_add = playlist_tracks_add;
    api->playlist_tracks_add_simple = playlist_tracks_add_simple;
    api->playlist_tags = playlist_tags;
    api->playlist_tags_simple = playlist_tags_simple;
    api->playlist_similar = playlist_similar;
    api->playlist_similar_simple = playlist_similar_simple;
    api->playlist_effect = playlist_effect;
    api->playlist_effect_simple = playlist_effect_simple;
    api->playlist_detail = playlist_detail;
    api->playlist_detail_simple = playlist_detail_simple;
    api->playlist_del = playlist_del;
    api->playlist_del_simple = playlist_del_simple;
    api->playlist_add = playlist_add;
    api->playlist_add_simple = playlist_add_simple;
    api->playhistory_upload = playhistory_upload;
    api->playhistory_upload_simple = playhistory_upload_simple;
    api->personal_fm = personal_fm;
    api->personal_fm_simple = personal_fm_simple;
    api->pc_diantai = pc_diantai;
    api->pc_diantai_simple = pc_diantai_simple;
    api->lyric = lyric;
    api->lyric_simple = lyric_simple;
    api->longaudio_week_recommend = longaudio_week_recommend;
    api->longaudio_week_recommend_simple = longaudio_week_recommend_simple;
    api->longaudio_vip_recommend = longaudio_vip_recommend;
    api->longaudio_vip_recommend_simple = longaudio_vip_recommend_simple;
    api->longaudio_rank_recommend = longaudio_rank_recommend;
    api->longaudio_rank_recommend_simple = longaudio_rank_recommend_simple;
    api->longaudio_daily_recommend = longaudio_daily_recommend;
    api->longaudio_daily_recommend_simple = longaudio_daily_recommend_simple;
    api->longaudio_album_detail = longaudio_album_detail;
    api->longaudio_album_detail_simple = longaudio_album_detail_simple;
    api->longaudio_album_audios = longaudio_album_audios;
    api->longaudio_album_audios_simple = longaudio_album_audios_simple;
    api->login_wx_create = login_wx_create;
    api->login_wx_create_simple = login_wx_create_simple;
    api->login_wx_check = login_wx_check;
    api->login_wx_check_simple = login_wx_check_simple;
    api->login_token = login_token;
    api->login_token_simple = login_token_simple;
    api->login_qr_key = login_qr_key;
    api->login_qr_key_simple = login_qr_key_simple;
    api->login_qr_create = login_qr_create;
    api->login_qr_create_simple = login_qr_create_simple;
    api->login_qr_check = login_qr_check;
    api->login_qr_check_simple = login_qr_check_simple;
    api->login_openplat = login_openplat;
    api->login_openplat_simple = login_openplat_simple;
    api->login_device = login_device;
    api->login_device_simple = login_device_simple;
    api->login_cellphone = login_cellphone;
    api->login_cellphone_simple = login_cellphone_simple;
    api->login = login;
    api->login_simple = login_simple;
    api->lastest_songs_listen = lastest_songs_listen;
    api->lastest_songs_listen_simple = lastest_songs_listen_simple;
    api->krm_audio = krm_audio;
    api->krm_audio_simple = krm_audio_simple;
    api->kmr_audio_mv = kmr_audio_mv;
    api->kmr_audio_mv_simple = kmr_audio_mv_simple;
    api->ip_zone_home = ip_zone_home;
    api->ip_zone_home_simple = ip_zone_home_simple;
    api->ip_zone = ip_zone;
    api->ip_zone_simple = ip_zone_simple;
    api->ip_playlist = ip_playlist;
    api->ip_playlist_simple = ip_playlist_simple;
    api->ip_dateil = ip_dateil;
    api->ip_dateil_simple = ip_dateil_simple;
    api->ip = ip;
    api->ip_simple = ip_simple;
    api->images_audio = images_audio;
    api->images_audio_simple = images_audio_simple;
    api->images = images;
    api->images_simple = images_simple;
    api->fm_songs = fm_songs;
    api->fm_songs_simple = fm_songs_simple;
    api->fm_recommend = fm_recommend;
    api->fm_recommend_simple = fm_recommend_simple;
    api->fm_image = fm_image;
    api->fm_image_simple = fm_image_simple;
    api->fm_class = fm_class;
    api->fm_class_simple = fm_class_simple;
    api->favorite_count = favorite_count;
    api->favorite_count_simple = favorite_count_simple;
    api->everyday_style_recommend = everyday_style_recommend;
    api->everyday_style_recommend_simple = everyday_style_recommend_simple;
    api->everyday_recommend = everyday_recommend;
    api->everyday_recommend_simple = everyday_recommend_simple;
    api->everyday_history = everyday_history;
    api->everyday_history_simple = everyday_history_simple;
    api->everyday_friend = everyday_friend;
    api->everyday_friend_simple = everyday_friend_simple;
    api->comment_playlist = comment_playlist;
    api->comment_playlist_simple = comment_playlist_simple;
    api->comment_music_hotword = comment_music_hotword;
    api->comment_music_hotword_simple = comment_music_hotword_simple;
    api->comment_music_classify = comment_music_classify;
    api->comment_music_classify_simple = comment_music_classify_simple;
    api->comment_music = comment_music;
    api->comment_music_simple = comment_music_simple;
    api->comment_floor = comment_floor;
    api->comment_floor_simple = comment_floor_simple;
    api->comment_count = comment_count;
    api->comment_count_simple = comment_count_simple;
    api->comment_album = comment_album;
    api->comment_album_simple = comment_album_simple;
    api->captcha_sent = captcha_sent;
    api->captcha_sent_simple = captcha_sent_simple;
    api->brush = brush;
    api->brush_simple = brush_simple;
    api->audio_related = audio_related;
    api->audio_related_simple = audio_related_simple;
    api->audio_ktv_total = audio_ktv_total;
    api->audio_ktv_total_simple = audio_ktv_total_simple;
    api->audio_accompany_matching = audio_accompany_matching;
    api->audio_accompany_matching_simple = audio_accompany_matching_simple;
    api->audio = audio;
    api->audio_simple = audio_simple;
    api->artist_videos = artist_videos;
    api->artist_videos_simple = artist_videos_simple;
    api->artist_unfollow = artist_unfollow;
    api->artist_unfollow_simple = artist_unfollow_simple;
    api->artist_lists = artist_lists;
    api->artist_lists_simple = artist_lists_simple;
    api->artist_honour = artist_honour;
    api->artist_honour_simple = artist_honour_simple;
    api->artist_follow_newsongs = artist_follow_newsongs;
    api->artist_follow_newsongs_simple = artist_follow_newsongs_simple;
    api->artist_follow = artist_follow;
    api->artist_follow_simple = artist_follow_simple;
    api->artist_detail = artist_detail;
    api->artist_detail_simple = artist_detail_simple;
    api->artist_audios = artist_audios;
    api->artist_audios_simple = artist_audios_simple;
    api->artist_albums = artist_albums;
    api->artist_albums_simple = artist_albums_simple;
    api->album_songs = album_songs;
    api->album_songs_simple = album_songs_simple;
    api->album_shop = album_shop;
    api->album_shop_simple = album_shop_simple;
    api->album_detail = album_detail;
    api->album_detail_simple = album_detail_simple;
    api->album = album;
    api->album_simple = album_simple;
    api->ai_recommend = ai_recommend;
    api->ai_recommend_simple = ai_recommend_simple;
    return api;
};