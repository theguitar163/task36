﻿// 任务：文本文件渲染器。
// 1.显示英文文本文件。
//   在磁盘上保存一个文本文件，内容为若干行的英文。用程序读取这个文档，并将其中的英文内容显示到屏幕上。
//   如果内容较长，无需考虑滚动条功能。
// 2.文本文件支持中文。
//   请自行了解 ANSI 编码、Unicode 编码（也叫 Utf - 16，分大小端）、Utf - 8 编码等常见编码格式，
//   使你的程序能正确的读取不同编码的中文文本文件，并正确在屏幕上显示中文。
// 3.富文本格式支持。
//   前面两步的文本文件并没有实现丰富的格式，这一步，要加上格式描述，使文档可以显示丰富的格式。
//   例如：文本文件中的 abc<b>def< / b>g，表示 def 为粗体，
//         <h1>xxx< / h1> 表示 1 号标题、
//         <i>xxx< / i> 表示斜体， 
//         << 表示 <， >> 表示 > 等。
//   你也可以 \bxxx / b 表示粗体，还可以用【asd】表示粗体。
//   格式描述自己定义，只要你的程序能按照你定义的格式正确显示即可。
//   注意：
//   1).格式定义要完整、没有歧义。例如，如果用 <b>xxx< / b> 表示粗体，那么，如何表示 < 这个符号呢？
//      当文档中含有 a < b 这样的内容，如何确保正确解析并显示？要在设计阶段就考虑清楚。
//   2).格式定义要允许嵌套。例如：<b>xxx<i>xxx< / i>xxx< / b> 表示粗体里面若干内容在粗体的基础上增加了斜体样式。
//   3).如果文档比较长，暂时可以不考虑滚动条的实现。
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <easyx.h>
#include <locale.h>


#define MAX_LINE 1000     // 最大行数
#define MAX_LEN  1000     // 每行最大字数

typedef struct tagTextDocument {
    TCHAR* lines[MAX_LINE];
    int lineCount;
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

// 通过读取少量文件头，判断文件编码格式
// TODO：对于无BOM的文件，因为采样数量太少，可能会误判
int fileEncodeType(TCHAR* fname)
{
    unsigned char buff[10] = { 0 };
    FILE* fp = _wfopen(fname, L"rb");
    if (fp == NULL) return 0;
    fread(buff, 1, 10, fp);
    fclose(fp);

    // 前三字节为EF BB BF
    if ((0xFF & buff[0]) == 0xEF && (0xFF & buff[1]) == 0xBB && (0xFF & buff[2]) == 0xBF) {
        return UTF8BOM;
    }
    // 前两字节为FF FE
    else if ((0xFF & buff[0]) == 0xFF && (0xFF & buff[1]) == 0xFE) {
        return UNICODELE;
    }
    // 前两字节为FE FF
    else if ((0xFF & buff[0]) == 0xFE && (0xFF & buff[1]) == 0xFF) {
        return UNICODEBE;
    }
    // 首字节为0xxx xxxx，表示纯英文ASCII码
    // 或前两字节为110xxxxx 10xxxxxx，表示两字节编码文字
    // 或前三字节为1110xxxx 10xxxxxx 10xxxxxx，表示三字节编码文字
    // （最长可以到六字节）
    else if ((((0xFF & buff[0]) & 0x80) == 0)   
        || (((0xFF & buff[0]) & 0xE0) == 0xC0 && ((0xFF & buff[1]) & 0xC0) == 0x80) 
        || (((0xFF & buff[0]) & 0xF0) == 0xE0 && ((0xFF & buff[1]) & 0xC0) == 0x80 && ((0xFF & buff[2]) & 0xC0) == 0x80)) { 
        return UTF8;
    }
    // GBK编码范围为，第一字节0x81–0xFE，第二个字节 0x40–0xFE
    else if (((0xFF & buff[0]) & 0xFF) >= 0x81 && ((0xFF & buff[1]) & 0xFF) >= 0x40) {
        return GBK;
    }
    return 0;
}

// 将从文件读取的内容p填充至line中，同步(扩展)分配内存
TCHAR* fillLine(TCHAR* line, TCHAR* p)
{
    // 若line为NULL，标明此行为初次读取，需分配新的内存块
    if (line == NULL) {
        line = (TCHAR*)malloc((wcslen(p) + 1) * sizeof(TCHAR));
        if (line != NULL) wcscpy(line, p);
    }
    // 否则，表明此行已存在内容，需重新扩展分配内存，并将新的内容追加在尾部
    else {
        line = (TCHAR*)realloc(line, (wcslen(line) + wcslen(p) + 1) * sizeof(TCHAR));
        if (line != NULL) wcscat(line, p);
    }
    return line;
}

// 逐行读取文件内容至TText文件结构中
void initText(TTextDoc* ptext, TCHAR* fname)
{
    FILE* fp;
    int codetype = fileEncodeType(fname);
    // 根据文件编码不同，采用不同的打开方式
    if (codetype == UTF8BOM || codetype == UNICODELE || codetype == UNICODEBE) {
        if ((fp = _wfopen(fname, L"rt,ccs=UNICODE")) == NULL) return;
    }
    else if (codetype == UTF8) {
        if ((fp = _wfopen(fname, L"rt,ccs=UTF-8")) == NULL) return;
    }
    else if (codetype == GBK) {
        setlocale(LC_ALL, "zh-CN");
        if ((fp = _wfopen(fname, L"rt")) == NULL) return;
    }
    else
        return;

    TCHAR buff[MAX_LEN] = { 0 };
    TCHAR* line = NULL;
    ptext->lineCount = 0;
    while (true) {
        // 按行读取文本，每次读取MAX_LEN，若有需要则多次读取同一行
        TCHAR* p = fgetws(buff, MAX_LEN, fp);
        // 文件读取完毕，直接退出循环
        if (p == NULL)
            break;

        // 检测回车符，替换为0
        TCHAR* crptr = wcschr(p, '\n');
        if (crptr != NULL) *crptr = '\0';
        // 换行或文件尾
        if (crptr != NULL || feof(fp)) {
            // 根据实际字符串长度重新分配每行的内存，避免浪费
            line = fillLine(line, p);
            // 行指针数组记录行内存块地址
            ptext->lines[ptext->lineCount] = line;
            ptext->lineCount++;
            // 行结束，将line初始化为NULL
            line = NULL;

            // 超过最大行数，简单处理，直接不再读取
            if (ptext->lineCount >= MAX_LINE)
                break;
        }
        // 当前fgetws读取的行未结束，需要多次读取
        else {
            // 将从文件读取的行内容，填充或追加进行内存块中，若有必要则扩充
            line = fillLine(line, p);
        }
    }
    fclose(fp);
}

// 释放为每行分配的内存
void freeText(TTextDoc* ptext)
{
    for (int i = 0; i < ptext->lineCount; i++) {
        free(ptext->lines[i]);
    }
    //free(ptext->lines);
}

void loadTextFile(TTextDoc* ptext)
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//用于接收文件名
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.hwndOwner = GetHWnd();
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = szFile;	       //接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = MAX_PATH;       // 路径大小
    ofn.lpstrFilter = TEXT("文本文件(txt)\0*.txt; *.cpp; *.html;\0\0"); // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 标志
    if (GetOpenFileName(&ofn)) {
        initText(ptext, ofn.lpstrFile);
    }
}

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
} TTextView;

void initView(TTextView* pview)
{
    pview->r.left = 10;
    pview->r.top = 10;
    pview->r.right = getwidth() - 10;
    pview->r.bottom = getheight() - 10;
    pview->linespace = 2;
    gettextstyle(&pview->font);
    _tcscpy(pview->font.lfFaceName, L"微软雅黑");
    pview->font.lfQuality = ANTIALIASED_QUALITY;
}

int main()
{
    initgraph(800, 800);
    setbkcolor(WHITE);
    cleardevice();
    settextcolor(BLACK);

    TTextDoc text;
    loadTextFile(&text);

    TTextView view;
    initView(&view);

    view.font.lfHeight = 22;
    settextstyle(&view.font);
    int y = view.r.top;
    for (int i = 0; i < text.lineCount; i++) {
        outtextxy(view.r.left, y, text.lines[i]);
        int th = textheight(text.lines[i]);
        if (th == 0) th = textheight(L" ");
        y = y + th + view.linespace;
    }

    _getch();
    freeText(&text);
    closegraph();
}


