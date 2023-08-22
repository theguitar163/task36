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
#include <locale.h>

#define MAX_LINE 10000
#define MAX_LEN  1000

typedef struct tagText {
    TCHAR* lines[MAX_LINE];
    int lineCnt;
} TText;

#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define ANSI      5

int fileEncodeType(TCHAR* fname)
{
    unsigned char buff[10] = { 0 };
    FILE* fp;
    if (_wfopen_s(&fp, fname, L"rb") != 0) return 0;
    fread(buff, 1, 10, fp);
    fclose(fp);

    if ((0xFF & buff[0]) == 0xEF && (0xFF & buff[1]) == 0xBB && (0xFF & buff[2]) == 0xBF) {
        return UTF8BOM;
    }
    else if ((0xFF & buff[0]) == 0xFF && (0xFF & buff[1]) == 0xFE) {
        return UNICODELE;
    }
    else if ((0xFF & buff[0]) == 0xFE && (0xFF & buff[1]) == 0xFF) {
        return UNICODEBE;
    }
    else if ((((0xFF & buff[0]) & 0x80) == 0)
        || (((0xFF & buff[0]) & 0xE0) == 0xC0 && ((0xFF & buff[1]) & 0xC0) == 0x80)
        || (((0xFF & buff[0]) & 0xF0) == 0xE0 && ((0xFF & buff[1]) & 0xC0) == 0x80 && ((0xFF & buff[2]) & 0xC0) == 0x80)) {
        return UTF8;
    }
    else if (((0xFF & buff[0]) & 0xFF) >= 0x81 && ((0xFF & buff[1]) & 0xFF) >= 0x40) {
        return ANSI;
    }
    return 0;
}
void initText(TText* ptext, TCHAR* fname)
{
    FILE* fp;
    int codetype = fileEncodeType(fname);

    if (codetype == UTF8BOM || codetype == UNICODELE || codetype == UNICODEBE) {
        if (_wfopen_s(&fp, fname, L"rt,ccs=UNICODE") != 0) return;
    }
    else if (codetype == UTF8) {
        if (_wfopen_s(&fp, fname, L"rt,ccs=UTF-8") != 0) return;
    }
    else {
        //setlocale(LC_CTYPE,"Chinese-simplified"); // zh_CN.UTF-8
        setlocale(LC_ALL, "zh-CN");
        if (_wfopen_s(&fp, fname, L"r") != 0) return;
    }

    TCHAR* line;
    ptext->lineCnt = 0;
    while (true) {
        line = (TCHAR*)calloc(MAX_LEN, sizeof(TCHAR));
        TCHAR* p = fgetws(line, MAX_LEN+1, fp);
        if (p != NULL) {
            ptext->lines[ptext->lineCnt] = p;
            ptext->lineCnt++;
        }
        else
            break;
    }

/*    ptext->lineCnt = 0;
    for (int i = 0; i < ptext->buffsize; i++) {
        if (ptext->buff[i] == '\n')
            ptext->lineCnt++;
    }

    TCHAR* line = NULL;
    TCHAR* ptr = NULL;
    //相较于strtok()函数，strtok_s函数需要用户传入一个指针，用于函数内部判断从哪里开始处理字符串
    line = wcstok_s((TCHAR*)ptext->buff, L"\n", &ptr);
    int idx = 0;
    ptext->lines[idx] = line;
    while (line != NULL) {
        line = wcstok_s(NULL, "\n", &ptr);  //其他的使用与strtok()函数相同
        ptext->lines[++idx] = line;
    }
    */
    fclose(fp);
}

void freeText(TText* ptext)
{
    for (int i = 0; i < ptext->lineCnt; i++) {
        free(ptext->lines[i]);
    }
    //free(ptext->lines);
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
    ofn.lpstrFilter = TEXT("文本文件(txt)\0*.txt; *.cpp; *.html;\0\0"); // 文件类型
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

/*根据msdn描述，如果参数为ccs = UNICODE，则
如果文件没有BOM头，则编码为ANSI，相当于没有写css =
如果文件BOM为UTF - 8，则编码为UTF - 8，相当于写了css = UTF - 8
如果文件BOM为UTF - 16LE，则编码为UTF - 16LE，相当于写了css = UTF - 16LE

Windows自带的Notepad(记事本)程序，可以保存的编码格式有
ANSI
Unicode:特指Unicode little endian，即第一段中的UTF - 16LE)
Unicode big endian(注：此编码在第一段中没有对应的ccs)
UTF - 8

根据实际测试
代码中必须使用_wfopen / fgetws，如果使用fopen则fgets时报错
如果文件编码为ANSI，则需要使用setlocale(LC_CTYPE, "Chinese-simplified")，
如果是UNICODE(UTF - 8、UTF - 16LE)则不需要。
（注：locale问题和本文无关，只是顺便提一下，免得写代码时不小心遗漏了）

总结：
1. 如果文件编码为UNICODE(UTF - 8、UTF - 16LE)，不使用ccs = UNICODE则取出来的格式不正确。（当然，可以以二进制方式"rb"打开，自己转化）
2. 如果文件编码为UNICODE(UTF - 8、UTF - 16LE)，但没有BOM头，则需要用css = UTF - 8、css = UTF - 16LE来指明编码，不可直接用ccs = UNICODE，否则就当成了ANSI编码。
3. 记事本中如果保存编码格式为Unicode big endian，则无法使用 ccs = UNICODE 这个功能
4. 如果用了ccs = UNICODE，则必须使用宽字符格式的相应函数，即便文件本身编码是ANSI格式
5. 如果文件本身编码是ANSI格式，别忘了setlocale
*/
// 打”记事本“程序Notepad.exe依次采用ANSI，Unicode，Unicode big endian 和 UTF - 8编码方式保存。
// 然后，用文本编辑软件UltraEdit中的”十六进制功能“，观察该文件的内部编码方式。
//1）ANSI：GB2312编码，是采用大头方式存储的。
//2）Unicode：编码是四个字节“FF FE 25 4E”，其中“FF FE”表明是小头方式存储，
//3）Unicode big endian：编码是四个字节“FE FF ”，其中“FE FF”表明是大头方式存储。
//4）UTF - 8：编码是六个字节“EF BB BF”，前三个字节“EF BB BF”表示这是UTF - 8编码，
// 它的存储顺序与编码顺序是一致的。

/*int main1(int argc, char** argv)
{
    unsigned char* buffer;
    char* fpath = argv[1];
    FILE* pFile = fopen(fpath, "rb");
    //FILE *pFile=fopen("277.txt", "rb+");// "rw,ccs=UTF-8");
    fseek(pFile, 0, SEEK_END);
    int lsize = ftell(pFile);
    rewind(pFile);
    buffer = (unsigned char*)malloc(sizeof(unsigned char) * lsize);
    memset(buffer, 0, sizeof(unsigned char) * lsize);
    int result = fread(buffer, 1, lsize, pFile);
    printf("sizw=%d   %d\n %s\n", lsize, strlen((const char*)buffer), buffer);
    fclose(pFile);
    int i = 0;
    for (i = 0; i < lsize; i++) {
        printf("%02X ", (unsigned  char)buffer[i]);
    }

    printf("\n");

    if (lsize > 2 && (0xFF & buffer[0]) == 0xEF && (0xFF & buffer[1]) == 0xBB && (0xFF & buffer[2]) == 0xBF) {
        printf("utf-8 bom\n");
        memmove(buffer, buffer + 3, strlen((const char*)buffer) - 2);
        lsize = strlen((const char*)buffer) - 2;

    }
    else if (lsize > 1 && (0xFF & buffer[0]) == 0xFF && (0xFF & buffer[1]) == 0xFE) {
        printf("litter unicode\n");
        memmove(buffer, buffer + 2, lsize - 1);
        lsize = lsize - 1;
        i = 0;
        for (i = 0; i < lsize; i++) {
            int tmp = buffer[i];
            buffer[i] = buffer[i + 1];
            buffer[i + 1] = tmp;
            i++;

        }
    }
    else if (lsize > 1 && (0xFF & buffer[0]) == 0xFE && (0xFF & buffer[1]) == 0xFF) {
        printf("default big unicode\n");
        memmove(buffer, buffer + 2, lsize - 1);
        lsize = lsize - 1;

    }
    else if ((((0xFF & buffer[0]) & 0x80) == 0) 
        || (lsize > 1 && ((0xFF & buffer[0]) & 0xE0) == 0xC0 && ((0xFF & buffer[1]) & 0xC0) == 0x80) 
        || (lsize > 2 && ((0xFF & buffer[0]) & 0xF0) == 0xE0 && ((0xFF & buffer[1]) & 0xC0) == 0x80 && ((0xFF & buffer[2]) & 0xC0) == 0x80)) {
        printf("utf-8 no bom\n");
    }
    else if (((0xFF & buffer[0]) & 0xFF) >= 0x81 && ((0xFF & buffer[1]) & 0xFF) >= 0x40) {
        printf("ansi\n");
    }


    i = 0;
    for (i = 0; i < lsize; i++) {
        printf("%02X ", (unsigned  char)buffer[i]);
    }
    printf("\n");
    return 0;
}
*/