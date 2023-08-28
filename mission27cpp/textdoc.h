#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // 最大行数
#define MAX_LEN  1000     // 每行最大字数

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define GBK       5

class CTextDoc 
{
public:
    TCHAR* m_text;                      // 文本文件全部数据，以字符串形式保存
    long m_lineptr[MAX_LINE];           // 行偏移量指针
    int m_lineCount;                    // 文本文件总行数
    TCHAR m_linebuff[MAX_LEN + 1];      // 用于获取行数据的内部缓冲区

    int fileEncodeType(TCHAR* fname);
public:
    // 初始化doc结构
    void initDoc(TCHAR* fname);
    // 获取指定行的内容
    TCHAR* getLine(int lineno);
    // 释放文本内存
    void freeDoc();
};
