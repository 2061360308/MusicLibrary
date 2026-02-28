#ifndef TOOL_H
#define TOOL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// 定义 BinData 结构体
typedef struct
{
    uint8_t *data;
    uint32_t size;
} BinData;

// 函数声明
BinData readBinFile(const char *filename);

#endif // TOOL_H