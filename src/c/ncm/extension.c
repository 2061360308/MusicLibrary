#include "quickjs.h"
#include "ncm_music_api.h"

#define API_FUNC(name, route)  \
    static char *name(JSContext *ctx, const char *cookies, const char *params, const NcmProcessEnv *env) {  \
        return ncm_request(ctx, route, cookies, params, env);  \
    }

API_FUNC(yunbei_today, "/yunbei/today")
API_FUNC(yunbei_task_finish, "/yunbei/task/finish")
API_FUNC(yunbei_tasks_todo, "/yunbei/tasks/todo")
API_FUNC(yunbei_tasks, "/yunbei/tasks")
API_FUNC(yunbei_sign, "/yunbei/sign")
API_FUNC(yunbei_receipt, "/yunbei/receipt")
API_FUNC(yunbei_rcmd_song_history, "/yunbei/rcmd/song/history")
API_FUNC(yunbei_rcmd_song, "/yunbei/rcmd/song")
API_FUNC(yunbei_info, "/yunbei/info")
API_FUNC(yunbei_expense, "/yunbei/expense")
API_FUNC(yunbei, "/yunbei")
API_FUNC(weblog, "/weblog")
API_FUNC(voice_lyric, "/voice/lyric")
API_FUNC(voice_detail, "/voice/detail")
API_FUNC(voice_delete, "/voice/delete")
API_FUNC(voicelist_trans, "/voicelist/trans")
API_FUNC(voicelist_search, "/voicelist/search")
API_FUNC(voicelist_list_search, "/voicelist/list/search")
API_FUNC(voicelist_list, "/voicelist/list")
API_FUNC(voicelist_detail, "/voicelist/detail")
API_FUNC(vip_timemachine, "/vip/timemachine")
API_FUNC(vip_tasks, "/vip/tasks")
API_FUNC(vip_info_v2, "/vip/info/v2")
API_FUNC(vip_info, "/vip/info")
API_FUNC(vip_growthpoint_get, "/vip/growthpoint/get")
API_FUNC(vip_growthpoint_details, "/vip/growthpoint/details")
API_FUNC(vip_growthpoint, "/vip/growthpoint")
API_FUNC(video_url, "/video/url")
API_FUNC(video_timeline_recommend, "/video/timeline/recommend")
API_FUNC(video_timeline_all, "/video/timeline/all")
API_FUNC(video_sub, "/video/sub")
API_FUNC(video_group_list, "/video/group/list")
API_FUNC(video_group, "/video/group")
API_FUNC(video_detail_info, "/video/detail/info")
API_FUNC(video_detail, "/video/detail")
API_FUNC(video_category_list, "/video/category/list")
API_FUNC(verify_qrcodestatus, "/verify/qrcodestatus")
API_FUNC(verify_getQr, "/verify/getQr")
API_FUNC(user_update, "/user/update")
API_FUNC(user_subcount, "/user/subcount")
API_FUNC(user_social_status_support, "/user/social/status/support")
API_FUNC(user_social_status_rcmd, "/user/social/status/rcmd")
API_FUNC(user_social_status_edit, "/user/social/status/edit")
API_FUNC(user_social_status, "/user/social/status")
API_FUNC(user_replacephone, "/user/replacephone")
API_FUNC(user_record, "/user/record")
API_FUNC(user_playlist_create, "/user/playlist/create")
API_FUNC(user_playlist_collect, "/user/playlist/collect")
API_FUNC(user_playlist, "/user/playlist")
API_FUNC(user_mutualfollow_get, "/user/mutualfollow/get")
API_FUNC(user_medal, "/user/medal")
API_FUNC(user_level, "/user/level")
API_FUNC(user_follow_mixed, "/user/follow/mixed")
API_FUNC(user_follows, "/user/follows")
API_FUNC(user_followeds, "/user/followeds")
API_FUNC(user_event, "/user/event")
API_FUNC(user_dj, "/user/dj")
API_FUNC(user_detail_new, "/user/detail/new")
API_FUNC(user_detail, "/user/detail")
API_FUNC(user_comment_history, "/user/comment/history")
API_FUNC(user_cloud_detail, "/user/cloud/detail")
API_FUNC(user_cloud_del, "/user/cloud/del")
API_FUNC(user_cloud, "/user/cloud")
API_FUNC(user_bindingcellphone, "/user/bindingcellphone")
API_FUNC(user_binding, "/user/binding")
API_FUNC(user_audio, "/user/audio")
API_FUNC(user_account, "/user/account")
API_FUNC(ugc_user_devote, "/ugc/user/devote")
API_FUNC(ugc_song_get, "/ugc/song/get")
API_FUNC(ugc_mv_get, "/ugc/mv/get")
API_FUNC(ugc_detail, "/ugc/detail")
API_FUNC(ugc_artist_search, "/ugc/artist/search")
API_FUNC(ugc_artist_get, "/ugc/artist/get")
API_FUNC(ugc_album_get, "/ugc/album/get")
API_FUNC(top_song, "/top/song")
API_FUNC(top_playlist_highquality, "/top/playlist/highquality")
API_FUNC(top_playlist, "/top/playlist")
API_FUNC(top_mv, "/top/mv")
API_FUNC(top_list, "/top/list")
API_FUNC(top_artists, "/top/artists")
API_FUNC(top_album, "/top/album")
API_FUNC(toplist_detail_v2, "/toplist/detail/v2")
API_FUNC(toplist_detail, "/toplist/detail")
API_FUNC(toplist_artist, "/toplist/artist")
API_FUNC(toplist, "/toplist")
API_FUNC(topic_sublist, "/topic/sublist")
API_FUNC(topic_detail_event_hot, "/topic/detail/event/hot")
API_FUNC(topic_detail, "/topic/detail")
API_FUNC(threshold_detail_get, "/threshold/detail/get")
API_FUNC(summary_annual, "/summary/annual")
API_FUNC(style_song, "/style/song")
API_FUNC(style_preference, "/style/preference")
API_FUNC(style_playlist, "/style/playlist")
API_FUNC(style_list, "/style/list")
API_FUNC(style_detail, "/style/detail")
API_FUNC(style_artist, "/style/artist")
API_FUNC(style_album, "/style/album")
API_FUNC(starpick_comments_summary, "/starpick/comments/summary")
API_FUNC(song_wiki_summary, "/song/wiki/summary")
API_FUNC(song_url_v1, "/song/url/v1")
API_FUNC(song_url, "/song/url")
API_FUNC(song_singledownlist, "/song/singledownlist")
API_FUNC(song_red_count, "/song/red/count")
API_FUNC(song_purchased, "/song/purchased")
API_FUNC(song_order_update, "/song/order/update")
API_FUNC(song_music_detail, "/song/music/detail")
API_FUNC(song_monthdownlist, "/song/monthdownlist")
API_FUNC(song_lyrics_mark_user_page, "/song/lyrics/mark/user/page")
API_FUNC(song_lyrics_mark_del, "/song/lyrics/mark/del")
API_FUNC(song_lyrics_mark_add, "/song/lyrics/mark/add")
API_FUNC(song_lyrics_mark, "/song/lyrics/mark")
API_FUNC(song_like_check, "/song/like/check")
API_FUNC(song_dynamic_cover, "/song/dynamic/cover")
API_FUNC(song_download_url_v1, "/song/download/url/v1")
API_FUNC(song_download_url, "/song/download/url")
API_FUNC(song_downlist, "/song/downlist")
API_FUNC(song_detail, "/song/detail")
API_FUNC(song_chorus, "/song/chorus")
API_FUNC(simi_user, "/simi/user")
API_FUNC(simi_song, "/simi/song")
API_FUNC(simi_playlist, "/simi/playlist")
API_FUNC(simi_mv, "/simi/mv")
API_FUNC(simi_artist, "/simi/artist")
API_FUNC(sign_happy_info, "/sign/happy/info")
API_FUNC(signin_progress, "/signin/progress")
API_FUNC(sheet_preview, "/sheet/preview")
API_FUNC(sheet_list, "/sheet/list")
API_FUNC(share_resource, "/share/resource")
API_FUNC(setting, "/setting")
API_FUNC(send_text, "/send/text")
API_FUNC(send_song, "/send/song")
API_FUNC(send_playlist, "/send/playlist")
API_FUNC(send_album, "/send/album")
API_FUNC(search_suggest, "/search/suggest")
API_FUNC(search_multimatch, "/search/multimatch")
API_FUNC(search_match, "/search/match")
API_FUNC(search_hot_detail, "/search/hot/detail")
API_FUNC(search_hot, "/search/hot")
API_FUNC(search_default, "/search/default")
API_FUNC(search, "/search")
API_FUNC(scrobble, "/scrobble")
API_FUNC(resource_like, "/resource/like")
API_FUNC(related_playlist, "/related/playlist")
API_FUNC(related_allvideo, "/related/allvideo")
API_FUNC(register_cellphone, "/register/cellphone")
API_FUNC(register_anonimous, "/register/anonimous")
API_FUNC(record_recent_voice, "/record/recent/voice")
API_FUNC(record_recent_video, "/record/recent/video")
API_FUNC(record_recent_song, "/record/recent/song")
API_FUNC(record_recent_playlist, "/record/recent/playlist")
API_FUNC(record_recent_dj, "/record/recent/dj")
API_FUNC(record_recent_album, "/record/recent/album")
API_FUNC(recommend_songs_dislike, "/recommend/songs/dislike")
API_FUNC(recommend_songs, "/recommend/songs")
API_FUNC(recommend_resource, "/recommend/resource")
API_FUNC(recent_listen_list, "/recent/listen/list")
API_FUNC(rebind, "/rebind")
API_FUNC(program_recommend, "/program/recommend")
API_FUNC(pl_count, "/pl/count")
API_FUNC(playmode_song_vector, "/playmode/song/vector")
API_FUNC(playmode_intelligence_list, "/playmode/intelligence/list")
API_FUNC(playlist_video_recent, "/playlist/video/recent")
API_FUNC(playlist_update_playcount, "/playlist/update/playcount")
API_FUNC(playlist_update, "/playlist/update")
API_FUNC(playlist_track_delete, "/playlist/track/delete")
API_FUNC(playlist_track_all, "/playlist/track/all")
API_FUNC(playlist_track_add, "/playlist/track/add")
API_FUNC(playlist_tracks, "/playlist/tracks")
API_FUNC(playlist_tags_update, "/playlist/tags/update")
API_FUNC(playlist_subscribers, "/playlist/subscribers")
API_FUNC(playlist_subscribe, "/playlist/subscribe")
API_FUNC(playlist_privacy, "/playlist/privacy")
API_FUNC(playlist_order_update, "/playlist/order/update")
API_FUNC(playlist_name_update, "/playlist/name/update")
API_FUNC(playlist_mylike, "/playlist/mylike")
API_FUNC(playlist_import_task_status, "/playlist/import/task/status")
API_FUNC(playlist_import_name_task_create, "/playlist/import/name/task/create")
API_FUNC(playlist_hot, "/playlist/hot")
API_FUNC(playlist_highquality_tags, "/playlist/highquality/tags")
API_FUNC(playlist_detail_rcmd_get, "/playlist/detail/rcmd/get")
API_FUNC(playlist_detail_dynamic, "/playlist/detail/dynamic")
API_FUNC(playlist_detail, "/playlist/detail")
API_FUNC(playlist_desc_update, "/playlist/desc/update")
API_FUNC(playlist_delete, "/playlist/delete")
API_FUNC(playlist_create, "/playlist/create")
API_FUNC(playlist_cover_update, "/playlist/cover/update")
API_FUNC(playlist_catlist, "/playlist/catlist")
API_FUNC(playlist_category_list, "/playlist/category/list")
API_FUNC(personal_fm_mode, "/personal/fm/mode")
API_FUNC(personal_fm, "/personal_fm")
API_FUNC(personalized_privatecontent_list, "/personalized/privatecontent/list")
API_FUNC(personalized_privatecontent, "/personalized/privatecontent")
API_FUNC(personalized_newsong, "/personalized/newsong")
API_FUNC(personalized_mv, "/personalized/mv")
API_FUNC(personalized_djprogram, "/personalized/djprogram")
API_FUNC(personalized, "/personalized")
API_FUNC(nickname_check, "/nickname/check")
API_FUNC(mv_url, "/mv/url")
API_FUNC(mv_sublist, "/mv/sublist")
API_FUNC(mv_sub, "/mv/sub")
API_FUNC(mv_first, "/mv/first")
API_FUNC(mv_exclusive_rcmd, "/mv/exclusive/rcmd")
API_FUNC(mv_detail_info, "/mv/detail/info")
API_FUNC(mv_detail, "/mv/detail")
API_FUNC(mv_all, "/mv/all")
API_FUNC(music_first_listen_info, "/music/first/listen/info")
API_FUNC(musician_tasks_new, "/musician/tasks/new")
API_FUNC(musician_tasks, "/musician/tasks")
API_FUNC(musician_sign, "/musician/sign")
API_FUNC(musician_play_trend, "/musician/play/trend")
API_FUNC(musician_data_overview, "/musician/data/overview")
API_FUNC(musician_cloudbean_obtain, "/musician/cloudbean/obtain")
API_FUNC(musician_cloudbean, "/musician/cloudbean")
API_FUNC(msg_recentcontact, "/msg/recentcontact")
API_FUNC(msg_private_history, "/msg/private/history")
API_FUNC(msg_private, "/msg/private")
API_FUNC(msg_notices, "/msg/notices")
API_FUNC(msg_forwards, "/msg/forwards")
API_FUNC(msg_comments, "/msg/comments")
API_FUNC(mlog_url, "/mlog/url")
API_FUNC(mlog_to_video, "/mlog/to/video")
API_FUNC(mlog_music_rcmd, "/mlog/music/rcmd")
API_FUNC(lyric_new, "/lyric/new")
API_FUNC(lyric, "/lyric")
API_FUNC(logout, "/logout")
API_FUNC(login_status, "/login/status")
API_FUNC(login_refresh, "/login/refresh")
API_FUNC(login_qr_key, "/login/qr/key")
API_FUNC(login_qr_create, "/login/qr/create")
API_FUNC(login_qr_check, "/login/qr/check")
API_FUNC(login_cellphone, "/login/cellphone")
API_FUNC(login, "/login")
API_FUNC(listen_data_year_report, "/listen/data/year/report")
API_FUNC(listen_data_total, "/listen/data/total")
API_FUNC(listen_data_today_song, "/listen/data/today/song")
API_FUNC(listen_data_report, "/listen/data/report")
API_FUNC(listen_data_realtime_report, "/listen/data/realtime/report")
API_FUNC(listentogether_sync_playlist_get, "/listentogether/sync/playlist/get")
API_FUNC(listentogether_sync_list_command, "/listentogether/sync/list/command")
API_FUNC(listentogether_status, "/listentogether/status")
API_FUNC(listentogether_room_create, "/listentogether/room/create")
API_FUNC(listentogether_room_check, "/listentogether/room/check")
API_FUNC(listentogether_play_command, "/listentogether/play/command")
API_FUNC(listentogether_heatbeat, "/listentogether/heatbeat")
API_FUNC(listentogether_end, "/listentogether/end")
API_FUNC(listentogether_accept, "/listentogether/accept")
API_FUNC(likelist, "/likelist")
API_FUNC(like, "/like")
API_FUNC(inner_version, "/inner/version")
API_FUNC(hug_comment, "/hug/comment")
API_FUNC(hot_topic, "/hot/topic")
API_FUNC(homepage_dragon_ball, "/homepage/dragon/ball")
API_FUNC(homepage_block_page, "/homepage/block/page")
API_FUNC(history_recommend_songs_detail, "/history/recommend/songs/detail")
API_FUNC(history_recommend_songs, "/history/recommend/songs")
API_FUNC(get_userids, "/get/userids")
API_FUNC(follow, "/follow")
API_FUNC(fm_trash, "/fm_trash")
API_FUNC(fanscenter_trend_list, "/fanscenter/trend/list")
API_FUNC(fanscenter_overview_get, "/fanscenter/overview/get")
API_FUNC(fanscenter_basicinfo_province_get, "/fanscenter/basicinfo/province/get")
API_FUNC(fanscenter_basicinfo_gender_get, "/fanscenter/basicinfo/gender/get")
API_FUNC(fanscenter_basicinfo_age_get, "/fanscenter/basicinfo/age/get")
API_FUNC(event_forward, "/event/forward")
API_FUNC(event_del, "/event/del")
API_FUNC(event, "/event")
API_FUNC(eapi_decrypt, "/eapi/decrypt")
API_FUNC(dj_toplist_popular, "/dj/toplist/popular")
API_FUNC(dj_toplist_pay, "/dj/toplist/pay")
API_FUNC(dj_toplist_newcomer, "/dj/toplist/newcomer")
API_FUNC(dj_toplist_hours, "/dj/toplist/hours")
API_FUNC(dj_toplist, "/dj/toplist")
API_FUNC(dj_today_perfered, "/dj/today/perfered")
API_FUNC(dj_subscriber, "/dj/subscriber")
API_FUNC(dj_sublist, "/dj/sublist")
API_FUNC(dj_sub, "/dj/sub")
API_FUNC(dj_recommend_type, "/dj/recommend/type")
API_FUNC(dj_recommend, "/dj/recommend")
API_FUNC(dj_radio_hot, "/dj/radio/hot")
API_FUNC(dj_program_toplist_hours, "/dj/program/toplist/hours")
API_FUNC(dj_program_toplist, "/dj/program/toplist")
API_FUNC(dj_program_detail, "/dj/program/detail")
API_FUNC(dj_program, "/dj/program")
API_FUNC(dj_personalize_recommend, "/dj/personalize/recommend")
API_FUNC(dj_paygift, "/dj/paygift")
API_FUNC(dj_hot, "/dj/hot")
API_FUNC(dj_detail, "/dj/detail")
API_FUNC(dj_catelist, "/dj/catelist")
API_FUNC(dj_category_recommend, "/dj/category/recommend")
API_FUNC(dj_category_excludehot, "/dj/category/excludehot")
API_FUNC(dj_banner, "/dj/banner")
API_FUNC(djRadio_top, "/djRadio/top")
API_FUNC(digitalAlbum_sales, "/digitalAlbum/sales")
API_FUNC(digitalAlbum_purchased, "/digitalAlbum/purchased")
API_FUNC(digitalAlbum_ordering, "/digitalAlbum/ordering")
API_FUNC(digitalAlbum_detail, "/digitalAlbum/detail")
API_FUNC(daily_signin, "/daily_signin")
API_FUNC(creator_authinfo_get, "/creator/authinfo/get")
API_FUNC(countries_code_list, "/countries/code/list")
API_FUNC(comment_video, "/comment/video")
API_FUNC(comment_playlist, "/comment/playlist")
API_FUNC(comment_new, "/comment/new")
API_FUNC(comment_mv, "/comment/mv")
API_FUNC(comment_music, "/comment/music")
API_FUNC(comment_like, "/comment/like")
API_FUNC(comment_hug_list, "/comment/hug/list")
API_FUNC(comment_hot, "/comment/hot")
API_FUNC(comment_floor, "/comment/floor")
API_FUNC(comment_event, "/comment/event")
API_FUNC(comment_dj, "/comment/dj")
API_FUNC(comment_album, "/comment/album")
API_FUNC(comment, "/comment")
API_FUNC(cloud_match, "/cloud/match")
API_FUNC(cloud_import, "/cloud/import")
API_FUNC(cloudsearch, "/cloudsearch")
API_FUNC(check_music, "/check/music")
API_FUNC(cellphone_existence_check, "/cellphone/existence/check")
API_FUNC(captcha_verify, "/captcha/verify")
API_FUNC(captcha_sent, "/captcha/sent")
API_FUNC(calendar, "/calendar")
API_FUNC(broadcast_sub, "/broadcast/sub")
API_FUNC(broadcast_channel_list, "/broadcast/channel/list")
API_FUNC(broadcast_channel_currentinfo, "/broadcast/channel/currentinfo")
API_FUNC(broadcast_channel_collect_list, "/broadcast/channel/collect/list")
API_FUNC(broadcast_category_region_get, "/broadcast/category/region/get")
API_FUNC(batch, "/batch")
API_FUNC(banner, "/banner")
API_FUNC(avatar_upload, "/avatar/upload")
API_FUNC(audio_match, "/audio/match")
API_FUNC(artist_video, "/artist/video")
API_FUNC(artist_top_song, "/artist/top/song")
API_FUNC(artist_sublist, "/artist/sublist")
API_FUNC(artist_sub, "/artist/sub")
API_FUNC(artist_songs, "/artist/songs")
API_FUNC(artist_new_song, "/artist/new/song")
API_FUNC(artist_new_mv, "/artist/new/mv")
API_FUNC(artist_mv, "/artist/mv")
API_FUNC(artist_list, "/artist/list")
API_FUNC(artist_follow_count, "/artist/follow/count")
API_FUNC(artist_fans, "/artist/fans")
API_FUNC(artist_detail_dynamic, "/artist/detail/dynamic")
API_FUNC(artist_detail, "/artist/detail")
API_FUNC(artist_desc, "/artist/desc")
API_FUNC(artist_album, "/artist/album")
API_FUNC(artists, "/artists")
API_FUNC(api, "/api")
API_FUNC(album_sublist, "/album/sublist")
API_FUNC(album_sub, "/album/sub")
API_FUNC(album_songsaleboard, "/album/songsaleboard")
API_FUNC(album_privilege, "/album/privilege")
API_FUNC(album_newest, "/album/newest")
API_FUNC(album_new, "/album/new")
API_FUNC(album_list_style, "/album/list/style")
API_FUNC(album_list, "/album/list")
API_FUNC(album_detail_dynamic, "/album/detail/dynamic")
API_FUNC(album_detail, "/album/detail")
API_FUNC(album, "/album")
API_FUNC(aidj_content_rcmd, "/aidj/content/rcmd")
API_FUNC(activate_init_profile, "/activate/init/profile")

NCMAPI *create_ncm_api(){
    NCMAPI *api = malloc(sizeof(NCMAPI));
    api->yunbei_today = yunbei_today;
    api->yunbei_task_finish = yunbei_task_finish;
    api->yunbei_tasks_todo = yunbei_tasks_todo;
    api->yunbei_tasks = yunbei_tasks;
    api->yunbei_sign = yunbei_sign;
    api->yunbei_receipt = yunbei_receipt;
    api->yunbei_rcmd_song_history = yunbei_rcmd_song_history;
    api->yunbei_rcmd_song = yunbei_rcmd_song;
    api->yunbei_info = yunbei_info;
    api->yunbei_expense = yunbei_expense;
    api->yunbei = yunbei;
    api->weblog = weblog;
    api->voice_lyric = voice_lyric;
    api->voice_detail = voice_detail;
    api->voice_delete = voice_delete;
    api->voicelist_trans = voicelist_trans;
    api->voicelist_search = voicelist_search;
    api->voicelist_list_search = voicelist_list_search;
    api->voicelist_list = voicelist_list;
    api->voicelist_detail = voicelist_detail;
    api->vip_timemachine = vip_timemachine;
    api->vip_tasks = vip_tasks;
    api->vip_info_v2 = vip_info_v2;
    api->vip_info = vip_info;
    api->vip_growthpoint_get = vip_growthpoint_get;
    api->vip_growthpoint_details = vip_growthpoint_details;
    api->vip_growthpoint = vip_growthpoint;
    api->video_url = video_url;
    api->video_timeline_recommend = video_timeline_recommend;
    api->video_timeline_all = video_timeline_all;
    api->video_sub = video_sub;
    api->video_group_list = video_group_list;
    api->video_group = video_group;
    api->video_detail_info = video_detail_info;
    api->video_detail = video_detail;
    api->video_category_list = video_category_list;
    api->verify_qrcodestatus = verify_qrcodestatus;
    api->verify_getQr = verify_getQr;
    api->user_update = user_update;
    api->user_subcount = user_subcount;
    api->user_social_status_support = user_social_status_support;
    api->user_social_status_rcmd = user_social_status_rcmd;
    api->user_social_status_edit = user_social_status_edit;
    api->user_social_status = user_social_status;
    api->user_replacephone = user_replacephone;
    api->user_record = user_record;
    api->user_playlist_create = user_playlist_create;
    api->user_playlist_collect = user_playlist_collect;
    api->user_playlist = user_playlist;
    api->user_mutualfollow_get = user_mutualfollow_get;
    api->user_medal = user_medal;
    api->user_level = user_level;
    api->user_follow_mixed = user_follow_mixed;
    api->user_follows = user_follows;
    api->user_followeds = user_followeds;
    api->user_event = user_event;
    api->user_dj = user_dj;
    api->user_detail_new = user_detail_new;
    api->user_detail = user_detail;
    api->user_comment_history = user_comment_history;
    api->user_cloud_detail = user_cloud_detail;
    api->user_cloud_del = user_cloud_del;
    api->user_cloud = user_cloud;
    api->user_bindingcellphone = user_bindingcellphone;
    api->user_binding = user_binding;
    api->user_audio = user_audio;
    api->user_account = user_account;
    api->ugc_user_devote = ugc_user_devote;
    api->ugc_song_get = ugc_song_get;
    api->ugc_mv_get = ugc_mv_get;
    api->ugc_detail = ugc_detail;
    api->ugc_artist_search = ugc_artist_search;
    api->ugc_artist_get = ugc_artist_get;
    api->ugc_album_get = ugc_album_get;
    api->top_song = top_song;
    api->top_playlist_highquality = top_playlist_highquality;
    api->top_playlist = top_playlist;
    api->top_mv = top_mv;
    api->top_list = top_list;
    api->top_artists = top_artists;
    api->top_album = top_album;
    api->toplist_detail_v2 = toplist_detail_v2;
    api->toplist_detail = toplist_detail;
    api->toplist_artist = toplist_artist;
    api->toplist = toplist;
    api->topic_sublist = topic_sublist;
    api->topic_detail_event_hot = topic_detail_event_hot;
    api->topic_detail = topic_detail;
    api->threshold_detail_get = threshold_detail_get;
    api->summary_annual = summary_annual;
    api->style_song = style_song;
    api->style_preference = style_preference;
    api->style_playlist = style_playlist;
    api->style_list = style_list;
    api->style_detail = style_detail;
    api->style_artist = style_artist;
    api->style_album = style_album;
    api->starpick_comments_summary = starpick_comments_summary;
    api->song_wiki_summary = song_wiki_summary;
    api->song_url_v1 = song_url_v1;
    api->song_url = song_url;
    api->song_singledownlist = song_singledownlist;
    api->song_red_count = song_red_count;
    api->song_purchased = song_purchased;
    api->song_order_update = song_order_update;
    api->song_music_detail = song_music_detail;
    api->song_monthdownlist = song_monthdownlist;
    api->song_lyrics_mark_user_page = song_lyrics_mark_user_page;
    api->song_lyrics_mark_del = song_lyrics_mark_del;
    api->song_lyrics_mark_add = song_lyrics_mark_add;
    api->song_lyrics_mark = song_lyrics_mark;
    api->song_like_check = song_like_check;
    api->song_dynamic_cover = song_dynamic_cover;
    api->song_download_url_v1 = song_download_url_v1;
    api->song_download_url = song_download_url;
    api->song_downlist = song_downlist;
    api->song_detail = song_detail;
    api->song_chorus = song_chorus;
    api->simi_user = simi_user;
    api->simi_song = simi_song;
    api->simi_playlist = simi_playlist;
    api->simi_mv = simi_mv;
    api->simi_artist = simi_artist;
    api->sign_happy_info = sign_happy_info;
    api->signin_progress = signin_progress;
    api->sheet_preview = sheet_preview;
    api->sheet_list = sheet_list;
    api->share_resource = share_resource;
    api->setting = setting;
    api->send_text = send_text;
    api->send_song = send_song;
    api->send_playlist = send_playlist;
    api->send_album = send_album;
    api->search_suggest = search_suggest;
    api->search_multimatch = search_multimatch;
    api->search_match = search_match;
    api->search_hot_detail = search_hot_detail;
    api->search_hot = search_hot;
    api->search_default = search_default;
    api->search = search;
    api->scrobble = scrobble;
    api->resource_like = resource_like;
    api->related_playlist = related_playlist;
    api->related_allvideo = related_allvideo;
    api->register_cellphone = register_cellphone;
    api->register_anonimous = register_anonimous;
    api->record_recent_voice = record_recent_voice;
    api->record_recent_video = record_recent_video;
    api->record_recent_song = record_recent_song;
    api->record_recent_playlist = record_recent_playlist;
    api->record_recent_dj = record_recent_dj;
    api->record_recent_album = record_recent_album;
    api->recommend_songs_dislike = recommend_songs_dislike;
    api->recommend_songs = recommend_songs;
    api->recommend_resource = recommend_resource;
    api->recent_listen_list = recent_listen_list;
    api->rebind = rebind;
    api->program_recommend = program_recommend;
    api->pl_count = pl_count;
    api->playmode_song_vector = playmode_song_vector;
    api->playmode_intelligence_list = playmode_intelligence_list;
    api->playlist_video_recent = playlist_video_recent;
    api->playlist_update_playcount = playlist_update_playcount;
    api->playlist_update = playlist_update;
    api->playlist_track_delete = playlist_track_delete;
    api->playlist_track_all = playlist_track_all;
    api->playlist_track_add = playlist_track_add;
    api->playlist_tracks = playlist_tracks;
    api->playlist_tags_update = playlist_tags_update;
    api->playlist_subscribers = playlist_subscribers;
    api->playlist_subscribe = playlist_subscribe;
    api->playlist_privacy = playlist_privacy;
    api->playlist_order_update = playlist_order_update;
    api->playlist_name_update = playlist_name_update;
    api->playlist_mylike = playlist_mylike;
    api->playlist_import_task_status = playlist_import_task_status;
    api->playlist_import_name_task_create = playlist_import_name_task_create;
    api->playlist_hot = playlist_hot;
    api->playlist_highquality_tags = playlist_highquality_tags;
    api->playlist_detail_rcmd_get = playlist_detail_rcmd_get;
    api->playlist_detail_dynamic = playlist_detail_dynamic;
    api->playlist_detail = playlist_detail;
    api->playlist_desc_update = playlist_desc_update;
    api->playlist_delete = playlist_delete;
    api->playlist_create = playlist_create;
    api->playlist_cover_update = playlist_cover_update;
    api->playlist_catlist = playlist_catlist;
    api->playlist_category_list = playlist_category_list;
    api->personal_fm_mode = personal_fm_mode;
    api->personal_fm = personal_fm;
    api->personalized_privatecontent_list = personalized_privatecontent_list;
    api->personalized_privatecontent = personalized_privatecontent;
    api->personalized_newsong = personalized_newsong;
    api->personalized_mv = personalized_mv;
    api->personalized_djprogram = personalized_djprogram;
    api->personalized = personalized;
    api->nickname_check = nickname_check;
    api->mv_url = mv_url;
    api->mv_sublist = mv_sublist;
    api->mv_sub = mv_sub;
    api->mv_first = mv_first;
    api->mv_exclusive_rcmd = mv_exclusive_rcmd;
    api->mv_detail_info = mv_detail_info;
    api->mv_detail = mv_detail;
    api->mv_all = mv_all;
    api->music_first_listen_info = music_first_listen_info;
    api->musician_tasks_new = musician_tasks_new;
    api->musician_tasks = musician_tasks;
    api->musician_sign = musician_sign;
    api->musician_play_trend = musician_play_trend;
    api->musician_data_overview = musician_data_overview;
    api->musician_cloudbean_obtain = musician_cloudbean_obtain;
    api->musician_cloudbean = musician_cloudbean;
    api->msg_recentcontact = msg_recentcontact;
    api->msg_private_history = msg_private_history;
    api->msg_private = msg_private;
    api->msg_notices = msg_notices;
    api->msg_forwards = msg_forwards;
    api->msg_comments = msg_comments;
    api->mlog_url = mlog_url;
    api->mlog_to_video = mlog_to_video;
    api->mlog_music_rcmd = mlog_music_rcmd;
    api->lyric_new = lyric_new;
    api->lyric = lyric;
    api->logout = logout;
    api->login_status = login_status;
    api->login_refresh = login_refresh;
    api->login_qr_key = login_qr_key;
    api->login_qr_create = login_qr_create;
    api->login_qr_check = login_qr_check;
    api->login_cellphone = login_cellphone;
    api->login = login;
    api->listen_data_year_report = listen_data_year_report;
    api->listen_data_total = listen_data_total;
    api->listen_data_today_song = listen_data_today_song;
    api->listen_data_report = listen_data_report;
    api->listen_data_realtime_report = listen_data_realtime_report;
    api->listentogether_sync_playlist_get = listentogether_sync_playlist_get;
    api->listentogether_sync_list_command = listentogether_sync_list_command;
    api->listentogether_status = listentogether_status;
    api->listentogether_room_create = listentogether_room_create;
    api->listentogether_room_check = listentogether_room_check;
    api->listentogether_play_command = listentogether_play_command;
    api->listentogether_heatbeat = listentogether_heatbeat;
    api->listentogether_end = listentogether_end;
    api->listentogether_accept = listentogether_accept;
    api->likelist = likelist;
    api->like = like;
    api->inner_version = inner_version;
    api->hug_comment = hug_comment;
    api->hot_topic = hot_topic;
    api->homepage_dragon_ball = homepage_dragon_ball;
    api->homepage_block_page = homepage_block_page;
    api->history_recommend_songs_detail = history_recommend_songs_detail;
    api->history_recommend_songs = history_recommend_songs;
    api->get_userids = get_userids;
    api->follow = follow;
    api->fm_trash = fm_trash;
    api->fanscenter_trend_list = fanscenter_trend_list;
    api->fanscenter_overview_get = fanscenter_overview_get;
    api->fanscenter_basicinfo_province_get = fanscenter_basicinfo_province_get;
    api->fanscenter_basicinfo_gender_get = fanscenter_basicinfo_gender_get;
    api->fanscenter_basicinfo_age_get = fanscenter_basicinfo_age_get;
    api->event_forward = event_forward;
    api->event_del = event_del;
    api->event = event;
    api->eapi_decrypt = eapi_decrypt;
    api->dj_toplist_popular = dj_toplist_popular;
    api->dj_toplist_pay = dj_toplist_pay;
    api->dj_toplist_newcomer = dj_toplist_newcomer;
    api->dj_toplist_hours = dj_toplist_hours;
    api->dj_toplist = dj_toplist;
    api->dj_today_perfered = dj_today_perfered;
    api->dj_subscriber = dj_subscriber;
    api->dj_sublist = dj_sublist;
    api->dj_sub = dj_sub;
    api->dj_recommend_type = dj_recommend_type;
    api->dj_recommend = dj_recommend;
    api->dj_radio_hot = dj_radio_hot;
    api->dj_program_toplist_hours = dj_program_toplist_hours;
    api->dj_program_toplist = dj_program_toplist;
    api->dj_program_detail = dj_program_detail;
    api->dj_program = dj_program;
    api->dj_personalize_recommend = dj_personalize_recommend;
    api->dj_paygift = dj_paygift;
    api->dj_hot = dj_hot;
    api->dj_detail = dj_detail;
    api->dj_catelist = dj_catelist;
    api->dj_category_recommend = dj_category_recommend;
    api->dj_category_excludehot = dj_category_excludehot;
    api->dj_banner = dj_banner;
    api->djRadio_top = djRadio_top;
    api->digitalAlbum_sales = digitalAlbum_sales;
    api->digitalAlbum_purchased = digitalAlbum_purchased;
    api->digitalAlbum_ordering = digitalAlbum_ordering;
    api->digitalAlbum_detail = digitalAlbum_detail;
    api->daily_signin = daily_signin;
    api->creator_authinfo_get = creator_authinfo_get;
    api->countries_code_list = countries_code_list;
    api->comment_video = comment_video;
    api->comment_playlist = comment_playlist;
    api->comment_new = comment_new;
    api->comment_mv = comment_mv;
    api->comment_music = comment_music;
    api->comment_like = comment_like;
    api->comment_hug_list = comment_hug_list;
    api->comment_hot = comment_hot;
    api->comment_floor = comment_floor;
    api->comment_event = comment_event;
    api->comment_dj = comment_dj;
    api->comment_album = comment_album;
    api->comment = comment;
    api->cloud_match = cloud_match;
    api->cloud_import = cloud_import;
    api->cloudsearch = cloudsearch;
    api->check_music = check_music;
    api->cellphone_existence_check = cellphone_existence_check;
    api->captcha_verify = captcha_verify;
    api->captcha_sent = captcha_sent;
    api->calendar = calendar;
    api->broadcast_sub = broadcast_sub;
    api->broadcast_channel_list = broadcast_channel_list;
    api->broadcast_channel_currentinfo = broadcast_channel_currentinfo;
    api->broadcast_channel_collect_list = broadcast_channel_collect_list;
    api->broadcast_category_region_get = broadcast_category_region_get;
    api->batch = batch;
    api->banner = banner;
    api->avatar_upload = avatar_upload;
    api->audio_match = audio_match;
    api->artist_video = artist_video;
    api->artist_top_song = artist_top_song;
    api->artist_sublist = artist_sublist;
    api->artist_sub = artist_sub;
    api->artist_songs = artist_songs;
    api->artist_new_song = artist_new_song;
    api->artist_new_mv = artist_new_mv;
    api->artist_mv = artist_mv;
    api->artist_list = artist_list;
    api->artist_follow_count = artist_follow_count;
    api->artist_fans = artist_fans;
    api->artist_detail_dynamic = artist_detail_dynamic;
    api->artist_detail = artist_detail;
    api->artist_desc = artist_desc;
    api->artist_album = artist_album;
    api->artists = artists;
    api->api = api;
    api->album_sublist = album_sublist;
    api->album_sub = album_sub;
    api->album_songsaleboard = album_songsaleboard;
    api->album_privilege = album_privilege;
    api->album_newest = album_newest;
    api->album_new = album_new;
    api->album_list_style = album_list_style;
    api->album_list = album_list;
    api->album_detail_dynamic = album_detail_dynamic;
    api->album_detail = album_detail;
    api->album = album;
    api->aidj_content_rcmd = aidj_content_rcmd;
    api->activate_init_profile = activate_init_profile;
    return api;
};