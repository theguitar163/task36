// 任务：文本文件渲染器。
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
#include "textdoc.h"

void procTag_B(TCHAR* key, TCHAR* value, int tagState)
{
    LOGFONT font;
    if (tagState == tagOPEN) {
        gettextstyle(&font);
        font.lfWeight = FW_BOLD;
        settextstyle(&font);
    }
    else {

    }
}

void procTag_I(TCHAR* key, TCHAR* value, int tagState)
{
    LOGFONT font;
    if (tagState == tagCLOSE) {
        gettextstyle(&font);
        font.lfItalic = 1;
        settextstyle(&font);
    }
    else {

    }
}

THandler handlers[] = {
    {L"B", L"/B", procTag_B},
    {L"I", L"/I", procTag_I},
};

void gettoken(TTextDoc* pdoc, long* pp, TToken* ptoken)
{
     // 初始化偏移量指针
    long ptr = *pp;

    // 记录起始位置
    long start = ptr;

    if (pdoc->text[ptr] == L'\0') {
        ptoken->type = BBCODE_END;
    }
    else if (pdoc->text[ptr] == L'\n') {
        ptoken->type = BBCODE_CRLF;
        ptr++;
    }
    else if (pdoc->text[ptr] == L'[') {
        while (true) {
            TCHAR c = pdoc->text[ptr];
            if (c == L'\0' || c == L'\n') {
                ptoken->type = BBCODE_TEXT;
                wcsncpy(ptoken->content, pdoc->text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                break;
            }
            else if (c == L']') {
                ptr++;
                ptoken->type = BBCODE_TAG;
                wcsncpy(ptoken->content, pdoc->text + start + 1, ptr - start - 2);
                ptoken->content[ptr - start - 2] = L'\0';
                break;
            }
            ptr++;
        }
    }
    else {
        while (true) {
            TCHAR c = pdoc->text[ptr];
            if (c == L'\0' || c == L'[' || c == L'\n') {
                ptoken->type = BBCODE_TEXT;
                wcsncpy(ptoken->content, pdoc->text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                *pp = ptr;
                break;
            }
            ptr++;
        }
    }
    *pp = ptr;
}

TCHAR* trim(TCHAR* str)
{
    if (str == NULL || wcslen(str) == 0)
        return NULL;
    TCHAR* p;
    // 剔除尾部空格
    p = str + wcslen(str) - 1;
    while (iswspace(*p)) {
        *p = L'\0';
        p--;
    }
    // 剔除开头空格
    p = str;
    while (iswspace(*p)) p++;
    return p;
}

// !隐患，token是全局变量。在处理过程中还会gettoken，引起token的变化
// 因此再次处理必须重新声明token，但是当前指针尤为重要
void parseTag(TToken* ptoken)
{
    TCHAR* key;
    TCHAR* value;
    TCHAR* ptr;
    key = trim(wcstok(ptoken->content, L"=", &ptr));
    value = trim(wcstok(NULL, L"=", &ptr));

    for (int i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        if (_wcsicmp(key, handlers[i].keyopen)) {
            handlers[i].proc(key, value, tagOPEN);
        }
        else if (_wcsicmp(key, handlers[i].keyclose)) {
            handlers[i].proc(key, value, tagCLOSE);
        }
    }
}

int main()
{
    initgraph(800, 800);
    setbkcolor(WHITE);
    cleardevice();
    settextcolor(BLACK);

    TTextDoc doc;
    loadTextFile(&doc);

    TTextView view;
    initView(&view, &doc);

    paintView(&view);
    _getch();
    cleardevice();
    paintTokenView(&view);

    _getch();
    freeText(&doc);
    closegraph();
}


