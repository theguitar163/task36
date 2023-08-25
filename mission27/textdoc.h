#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // 最大行数
#define MAX_LEN  1000     // 每行最大字数

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef struct tagTextDocument {
    TCHAR* text;                      // 文本文件全部数据，以字符串形式保存
    long lineptr[MAX_LINE];           // 行偏移量指针
    int lineCount;                    // 文本文件总行数
    TCHAR linebuff[MAX_LEN + 1];      // 用于获取行数据的内部缓冲区
} TTextDoc;

#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define GBK       5

// 初始化doc结构
void initDoc(TTextDoc* pdoc, TCHAR* fname);
// 获取指定行的内容
TCHAR* getLine(TTextDoc* pdoc, int lineno);
// 释放文本内存
void freeText(TTextDoc* pdoc);
// 加载文本文件
void loadTextFile(TTextDoc* pdoc);
