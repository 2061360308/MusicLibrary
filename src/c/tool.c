#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tool.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define PATH_BUFFER_SIZE MAX_PATH
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096 // 如果未定义 PATH_MAX，手动定义一个合理值
#endif
#define PATH_BUFFER_SIZE PATH_MAX
#else
#define PATH_BUFFER_SIZE 4096 // 默认值
#endif

static void getCurrentLibPath(char *pathBuffer, size_t bufferSize);

BinData readBinFile(const char *filename)
{
    char binPath[PATH_BUFFER_SIZE];

    getCurrentLibPath(binPath, PATH_BUFFER_SIZE);

    // 构建完整路径
    strncat(binPath, "/", PATH_BUFFER_SIZE - strlen(binPath) - 1);
    strncat(binPath, filename, PATH_BUFFER_SIZE - strlen(binPath) - 1);

    BinData result = {NULL, 0};

    FILE *f = fopen(binPath, "rb");
    if (f == NULL)
    {
        return result;
    }

    fseek(f, 0, SEEK_END);
    result.size = ftell(f);
    fseek(f, 0, SEEK_SET);

    result.data = (uint8_t *)malloc(result.size);
    fread(result.data, 1, result.size, f);
    fclose(f);

    return result;
}

#if defined(_WIN32)
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
static void getCurrentLibPath(char *pathBuffer, size_t bufferSize) {
    HMODULE hModule = NULL;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          (LPCSTR)&getCurrentLibPath, &hModule)) {
        GetModuleFileNameA(hModule, pathBuffer, (DWORD)bufferSize);
        PathRemoveFileSpecA(pathBuffer);
    } else {
        pathBuffer[0] = '\0';
    }
}
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
void getCurrentLibPath(char *pathBuffer, size_t bufferSize) {
    printf("Compiling Linux version of getCurrentLibPath\n");
    ssize_t len = readlink("/proc/self/exe", pathBuffer, bufferSize - 1);
    if (len != -1) {
        pathBuffer[len] = '\0';
        char *lastSlash = strrchr(pathBuffer, '/');
        if (lastSlash) {
            *lastSlash = '\0';
        }
    } else {
        pathBuffer[0] = '\0';
    }
}
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
void getCurrentLibPath(char *pathBuffer, size_t bufferSize) {
    printf("Compiling macOS version of getCurrentLibPath\n");
    uint32_t size = (uint32_t)bufferSize;
    if (_NSGetExecutablePath(pathBuffer, &size) == 0) {
        char *lastSlash = strrchr(pathBuffer, '/');
        if (lastSlash) {
            *lastSlash = '\0';
        }
    } else {
        pathBuffer[0] = '\0';
    }
}
#else
void getCurrentLibPath(char *pathBuffer, size_t bufferSize) {
    printf("Compiling unsupported platform version of getCurrentLibPath\n");
    pathBuffer[0] = '\0';
}
#endif