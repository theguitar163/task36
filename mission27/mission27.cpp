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
#include <locale.h>


#define MAX_LINE 1000     // 最大行数
#define MAX_LEN  1000     // 每行最大字数

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef struct tagTextDocument {
    TCHAR* text;
    long lineptr[MAX_LINE];
    int lineCount;

    TCHAR linebuff[MAX_LEN+1];
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

// 逐行读取文件内容至TText文件结构中
// 经反复验证，fgetws能够正确解码，而fread不能正确解码
// 实验了两种数据结构：
// 1.分行读取，为每行分配字符串(内存)，将每行的指针记录至数组中；
// 2.连续分行读取全部文件至连续的内存块中，记录每行开始的偏移位置
// 分行单独存储不方便对文件内容连续操作，但胜在分行访问便捷
// 连续存储为便于跨行连续对文件内容操作，但不方便对单独行的访问
// 结论：采用方式2.
void initDoc(TTextDoc* pdoc, TCHAR* fname)
{
    FILE* fp;
    // 检测文件编码
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

    // 方法二：连续存储
    // 求文件长度
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    // 分配内存，注意使用sizeof(TCHAR)为最小单位
    pdoc->text = (TCHAR*)malloc(sizeof(TCHAR) * (filesize + 1));

    TCHAR* line = pdoc->text;
    TCHAR buff[MAX_LEN] = { 0 };
    fseek(fp, 0, SEEK_SET);
    while (true) {
        // 按行读取文本，每次读取MAX_LEN，若有需要则多次读取同一行
        TCHAR* p = fgetws(buff, MAX_LEN, fp);
        // 文件读取完毕，直接退出循环
        if (p == NULL)
            break;
        wcsncpy(line, p, wcslen(p));
        line = line + wcslen(p);
    }

    // 逐字检测回车符，记录每行的偏移地址
    int lineno = 0;
    pdoc->lineptr[0] = 0;
    for (int i = 0; i < wcslen(pdoc->text); i++) {
        if (pdoc->text[i] == '\n') {
            lineno++;
            pdoc->lineptr[lineno] = i + 1;
        }
    }
    pdoc->lineCount = lineno+1;
    fclose(fp);
}

/*
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

//  方法一。分行单独存储
void initDoc(TTextDoc* pdoc, TCHAR* fname)
{
    FILE* fp;
    // 检测文件编码
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


    TCHAR buff[MAX_LEN] = {0};
    TCHAR* line = NULL;
    pdoc->lineCount = 0;
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
            pdoc->lines[pdoc->lineCount] = line;
            pdoc->lineCount++;
            // 行结束，将line初始化为NULL
            line = NULL;

            // 超过最大行数，简单处理，直接不再读取
            if (pdoc->lineCount >= MAX_LINE)
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
*/

// 释放文本内存
void freeText(TTextDoc* pdoc)
{
    free(pdoc->text);
}

// 获取指定行的内容
TCHAR* getLine(TTextDoc* pdoc, int lineno)
{
    // 检测行数参数的合法性
    if (lineno >= pdoc->lineCount) return NULL;

    // 行起始位置指针
    TCHAR* ptr = (TCHAR*)pdoc->text + pdoc->lineptr[lineno];
    long len;
    if (lineno == pdoc->lineCount - 1)
        len = wcslen(ptr);
    else
        len = pdoc->lineptr[lineno + 1] - pdoc->lineptr[lineno];

    // 确保缓冲区不溢出
    len = min(len, MAX_LEN);

    wcsncpy(pdoc->linebuff, ptr, len);
    pdoc->linebuff[len] = '\0';
    return pdoc->linebuff;
}

void loadTextFile(TTextDoc* pdoc)
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
        initDoc(pdoc, ofn.lpstrFile);
    }
}

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
    TTextDoc* pdoc;
} TTextView;

void initView(TTextView* pview, TTextDoc* pdoc)
{
    pview->r.left = 10;
    pview->r.top = 10;
    pview->r.right = getwidth() - 10;
    pview->r.bottom = getheight() - 10;
    pview->linespace = 2;
    gettextstyle(&pview->font);
    _tcscpy(pview->font.lfFaceName, L"微软雅黑");
    pview->font.lfQuality = ANTIALIASED_QUALITY;
    pview->pdoc = pdoc;
}

void paintView(TTextView* pview)
{
    pview->font.lfHeight = 22;
    settextstyle(&pview->font);
    int y = pview->r.top;
    for (int i = 0; i < pview->pdoc->lineCount; i++) {
        TCHAR* p = getLine(pview->pdoc, i);
        outtextxy(pview->r.left, y, p);
        int th = textheight(p);
        if (th == 0) th = textheight(L" ");
        y = y + th + pview->linespace;
    }
}



#define BBCODE_TEXT 1
#define BBCODE_TAG  2
#define BBCODE_CRLF   3
#define BBCODE_END  4

typedef struct tagToken {
    int type;
    TCHAR content[MAX_LEN];
} TToken;

void gettoken(TTextDoc* pdoc, long* pp, TToken* ptoken)
{
     // 初始化偏移量指针
    long ptr = *pp;

    // 记录起始位置
    long start = ptr;
    long end = ptr;

    if (ptr >= wcslen(pdoc->text)) {
        ptoken->type = BBCODE_END;
    }
    else if (pdoc->text[ptr] == L'\n') {
        ptoken->type = BBCODE_CRLF;
        ptr++;
    }
    else if (pdoc->text[ptr] == L'[') {
        while (true) {
            if (ptr >= wcslen(pdoc->text) || pdoc->text[ptr] == L'\n') {
                ptoken->type = BBCODE_TEXT;
                wcsncpy(ptoken->content, pdoc->text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                break;
            }
            else if (pdoc->text[ptr] == L']') {
                ptr++;
                ptoken->type = BBCODE_TAG;
                wcsncpy(ptoken->content, pdoc->text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                break;
            }
            ptr++;
        }
    }
    else {
        while (true) {
            if (ptr >= wcslen(pdoc->text) || pdoc->text[ptr] == L'[' || pdoc->text[ptr] == L'\n') {
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


void paintTokenView(TTextView* pview)
{
    pview->font.lfHeight = 22;
    settextstyle(&pview->font);
    int x = pview->r.left;
    int y = pview->r.top;
    int th = 0;

    TToken token;
    long ptr = 0;
    gettoken(pview->pdoc, &ptr, &token);
    while (token.type != BBCODE_END) {
        switch (token.type) {
        case BBCODE_TEXT:
            outtextxy(x, y, token.content);
            th = max(th, textheight(token.content));
            x = x + textwidth(token.content);
            break;
        case BBCODE_CRLF:
            x = pview->r.left;
            y = y + th + pview->linespace;
            break;
        }

        gettoken(pview->pdoc, &ptr, &token);
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

    paintTokenView(&view);

    _getch();
    freeText(&doc);
    closegraph();
}


