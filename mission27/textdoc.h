#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // 最大行数
#define MAX_LEN  1000     // 每行最大字数

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef struct tagTextDocument {
    TCHAR* text;
    long lineptr[MAX_LINE];
    int lineCount;

    TCHAR linebuff[MAX_LEN + 1];
} TTextDoc;

// 文本文件编码格式分类两类
// 1.文件头包含BOM
// （1）Unicode(little endian)：前两个字节“FF FE”，表示小头方式存储
// （2）Unicode big endian：前两个字节“FE FF ”，表示大头方式存储
// （3）UTF-8：前三个字节“EF BB BF”
// 上述包含了BOM的文件，在打开文件时加上ccs=UNICODE即可正确读取
// 2.文件头不包含BOM
// （1）GBK：汉字编码通过设定区域setlocale(LC_ALL, "zh-CN")确保正确读取
// （2）UTF-8：打开文件时需要使用ccs=UTF-8来保证正确读取
// （3）UTF-16：打开文件时需要使用ccs=UTF-16来保证正确读取
#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define GBK       5