cmake_minimum_required(VERSION 3.21)
project(kugouC VERSION 1.0.0 LANGUAGES C)

# ========== VCPKG 集成 ==========
if(DEFINED ENV{VCPKG_ROOT} OR EXISTS("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"))
    include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
elseif(EXISTS("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"))
    include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

# ========== 编译选项 ==========
if(CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

# 文件输出目录绑定
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/include) # 头文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

# ========== QuickJS ==========
# set(QJS_BUILD_LIBC ON)
# set(BUILD_SHARED_LIBS ON)
add_subdirectory(quickjs)

# ========== 公共源文件 ==========
file(GLOB BUNDLE_C_FILES "${CMAKE_SOURCE_DIR}/bundle/c/*.c")

set(LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/src/c/main.c
    ${CMAKE_SOURCE_DIR}/src/c/http.c
    ${CMAKE_SOURCE_DIR}/src/c/tinycthread.c
    ${CMAKE_SOURCE_DIR}/src/c/kugou_music_api.c
    # ${CMAKE_SOURCE_DIR}/src/c/tool.c
    ${BUNDLE_C_FILES}
)

# ========== INTERFACE 库 - 存放公共配置 ==========
add_library(kugou_music_api_config INTERFACE)
target_include_directories(kugou_music_api_config INTERFACE
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/quickjs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>
    # $<INSTALL_INTERFACE:include>
)
target_link_libraries(kugou_music_api_config INTERFACE qjs qjs-libc)

# ========== 平台相关依赖 ==========
if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURL REQUIRED libcurl)
    target_include_directories(kugou_music_api_config INTERFACE ${CURL_INCLUDE_DIRS})
    target_link_libraries(kugou_music_api_config INTERFACE ${CURL_LIBRARIES})
else()
    find_package(CURL CONFIG REQUIRED)
    target_link_libraries(kugou_music_api_config INTERFACE CURL::libcurl)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

# ========== STATIC 库 ==========
add_library(kugou_music_api_static STATIC EXCLUDE_FROM_ALL ${LIB_SOURCES})
target_link_libraries(kugou_music_api_static PUBLIC kugou_music_api_config)

# ========== SHARED 库 ==========
add_library(kugou_music_api SHARED ${LIB_SOURCES})
set_target_properties(kugou_music_api PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

if(WIN32)
    # 设置 Windows 平台特有的属性
    set_target_properties(kugou_music_api PROPERTIES
        LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/src/c/kugou_music_api.def"
    )
    target_compile_definitions(kugou_music_api PUBLIC KUGOU_MUSIC_API_EXPORTS)
endif()

target_link_libraries(kugou_music_api PUBLIC kugou_music_api_config)

# ========== 测试可执行文件 ==========
add_executable(test ${CMAKE_SOURCE_DIR}/src/c/test.c)
# if(WIN32)
#     set_target_properties(test PROPERTIES
#         ENABLE_EXPORTS OFF
#         LINK_FLAGS "/NOENTRY /NODEFAULTLIB /NOIMPLIB"
#     )
# endif()
target_link_libraries(test PRIVATE kugou_music_api_static)


# ========== 生成配置头文件 ==========
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/kugou_music_api_version.h.in
    ${CMAKE_BINARY_DIR}/kugou_music_api_version.h
)

# 复制相关文件到lib目录
add_custom_command(TARGET kugou_music_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/kugou_music_api_version.h
        ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/kugou_music_api_version.h
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/include/kugou_music_api.h
        ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/kugou_music_api.h
    # COMMAND ${CMAKE_COMMAND} -E copy
    #     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qjs.dll
    #     ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/qjs.dll
    COMMENT "Copying kugou_music_api.h qjs.lib qjs.dll to include directory"
)

# 删除exp文件
add_custom_command(TARGET kugou_music_api POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E sleep 1
    COMMAND if exist *.exp (del /q *.exp)
    COMMENT "Clean up .exp files"
    WORKING_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

# ========== 安装规则 ==========
include(GNUInstallDirs)

install(TARGETS kugou_music_api kugou_music_api_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/src/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

