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
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <easyx.h>

typedef struct tagText {
    char* buff;
    int buffsize;
    TCHAR** lines;
    int lineCnt;
} TText;

void initText(TText* ptext, TCHAR* fname)
{
    FILE* fp;
    if (_wfopen_s(&fp, fname, L"rt,ccs=UNICODE") != 0) return;

    fseek(fp, 0, SEEK_END);
    ptext->buffsize = ftell(fp);
    ptext->buff = (char*)malloc(ptext->buffsize);
    if (ptext->buff != NULL) {
        fseek(fp, 0, SEEK_SET);
        fread(ptext->buff, sizeof(char), ptext->buffsize, fp);

        ptext->lineCnt = 0;
        for (int i = 0; i < ptext->buffsize; i++) {
            if (ptext->buff[i] == '\n')
                ptext->lineCnt++;
        }
        ptext->lines = (TCHAR**)malloc(sizeof(TCHAR*) * ptext->lineCnt);

        TCHAR* line = NULL;
        TCHAR* ptr = NULL;
        //相较于strtok()函数，strtok_s函数需要用户传入一个指针，用于函数内部判断从哪里开始处理字符串
        line = wcstok_s((wchar_t*)ptext->buff, L"\n", &ptr);
        int idx = 0;
        ptext->lines[idx] = line;
        while (line != NULL) {
            line = wcstok_s(NULL, L"\n", &ptr);  //其他的使用与strtok()函数相同
            ptext->lines[++idx] = line;
        }
    }
    fclose(fp);
}

void freeText(TText* ptext)
{
    free(ptext->buff);
    free(ptext->lines);
}

void onLoadText(TText* ptext)
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//用于接收文件名
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.hwndOwner = GetHWnd();
    ofn.lStructSize = sizeof(ofn); // 结构大小
    ofn.lpstrFile = szFile;	       //接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = MAX_PATH;       // 路径大小
    ofn.lpstrFilter = TEXT("文本文件(txt)\0*.txt; *.html;\0\0"); // 文件类型
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 标志
    if (GetOpenFileName(&ofn)) {
        initText(ptext, ofn.lpstrFile);
    }
}

int main()
{
    initgraph(800, 800);
    setbkcolor(WHITE);
    cleardevice();
    settextcolor(BLACK);

    TText text;
    onLoadText(&text);

    for (int i = 0; i < text.lineCnt; i++) {
        outtextxy(0, i * 20, text.lines[i]);
    }

    _getch();
    freeText(&text);
    closegraph();
}
