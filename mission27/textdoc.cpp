#define _CRT_SECURE_NO_WARNINGS

#include <easyx.h>
#include <stdio.h>
#include <locale.h>
#include "textdoc.h"

// ͨ����ȡ�����ļ�ͷ���ж��ļ������ʽ
// �ı��ļ������ʽ��������
// 1.�ļ�ͷ����BOM
// ��1��Unicode(little endian)��ǰ�����ֽڡ�FF FE������ʾСͷ��ʽ�洢
// ��2��Unicode big endian��ǰ�����ֽڡ�FE FF ������ʾ��ͷ��ʽ�洢
// ��3��UTF-8��ǰ�����ֽڡ�EF BB BF��
// ����������BOM���ļ����ڴ��ļ�ʱ����ccs=UNICODE������ȷ��ȡ
// 2.�ļ�ͷ������BOM
// ��1��GBK�����ֱ���ͨ���趨����setlocale(LC_ALL, "zh-CN")ȷ����ȷ��ȡ
// ��2��UTF-8�����ļ�ʱ��Ҫʹ��ccs=UTF-8����֤��ȷ��ȡ
// ��3��UTF-16�����ļ�ʱ��Ҫʹ��ccs=UTF-16����֤��ȷ��ȡ
// TODO��������BOM���ļ�����Ϊ��������̫�٣����ܻ�����
int fileEncodeType(TCHAR* fname)
{
    unsigned char buff[10] = { 0 };
    FILE* fp = _wfopen(fname, L"rb");
    if (fp == NULL) return 0;
    fread(buff, 1, 10, fp);
    fclose(fp);

    // ǰ���ֽ�ΪEF BB BF
    if ((0xFF & buff[0]) == 0xEF && (0xFF & buff[1]) == 0xBB && (0xFF & buff[2]) == 0xBF) {
        return UTF8BOM;
    }
    // ǰ���ֽ�ΪFF FE
    else if ((0xFF & buff[0]) == 0xFF && (0xFF & buff[1]) == 0xFE) {
        return UNICODELE;
    }
    // ǰ���ֽ�ΪFE FF
    else if ((0xFF & buff[0]) == 0xFE && (0xFF & buff[1]) == 0xFF) {
        return UNICODEBE;
    }
    // ���ֽ�Ϊ0xxx xxxx����ʾ��Ӣ��ASCII��
    // ��ǰ���ֽ�Ϊ110xxxxx 10xxxxxx����ʾ���ֽڱ�������
    // ��ǰ���ֽ�Ϊ1110xxxx 10xxxxxx 10xxxxxx����ʾ���ֽڱ�������
    // ������Ե����ֽڣ�
    else if ((((0xFF & buff[0]) & 0x80) == 0)
        || (((0xFF & buff[0]) & 0xE0) == 0xC0 && ((0xFF & buff[1]) & 0xC0) == 0x80)
        || (((0xFF & buff[0]) & 0xF0) == 0xE0 && ((0xFF & buff[1]) & 0xC0) == 0x80 && ((0xFF & buff[2]) & 0xC0) == 0x80)) {
        return UTF8;
    }
    // GBK���뷶ΧΪ����һ�ֽ�0x81�C0xFE���ڶ����ֽ� 0x40�C0xFE
    else if (((0xFF & buff[0]) & 0xFF) >= 0x81 && ((0xFF & buff[1]) & 0xFF) >= 0x40) {
        return GBK;
    }
    return 0;
}

// ���ж�ȡ�ļ�������TText�ļ��ṹ��
// ��������֤��fgetws�ܹ���ȷ���룬��fread������ȷ����
// ʵ�����������ݽṹ��
// 1.���ж�ȡ��Ϊÿ�з����ַ���(�ڴ�)����ÿ�е�ָ���¼�������У�
// 2.�������ж�ȡȫ���ļ����������ڴ���У���¼ÿ�п�ʼ��ƫ��λ��
// ���е����洢��������ļ�����������������ʤ�ڷ��з��ʱ��
// �����洢Ϊ���ڿ����������ļ����ݲ�������������Ե����еķ���
// ���ۣ����÷�ʽ2.
void initDoc(TTextDoc* pdoc, TCHAR* fname)
{
    FILE* fp;
    // ����ļ�����
    int codetype = fileEncodeType(fname);
    // �����ļ����벻ͬ�����ò�ͬ�Ĵ򿪷�ʽ
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

    // �������������洢
    // ���ļ�����
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    // �����ڴ棬ע��ʹ��sizeof(TCHAR)Ϊ��С��λ
    pdoc->text = (TCHAR*)malloc(sizeof(TCHAR) * (filesize + 1));

    TCHAR* line = pdoc->text;
    TCHAR buff[MAX_LEN] = { 0 };
    fseek(fp, 0, SEEK_SET);
    while (true) {
        // ���ж�ȡ�ı���ÿ�ζ�ȡMAX_LEN��������Ҫ���ζ�ȡͬһ��
        TCHAR* p = fgetws(buff, MAX_LEN, fp);
        // �ļ���ȡ��ϣ�ֱ���˳�ѭ��
        if (p == NULL)
            break;
        wcsncpy(line, p, wcslen(p));
        line = line + wcslen(p);
    }
    *line = L'\0';

    // ���ּ��س�������¼ÿ�е�ƫ�Ƶ�ַ
    int lineno = 0;
    pdoc->lineptr[0] = 0;
    for (int i = 0; i < wcslen(pdoc->text); i++) {
        if (pdoc->text[i] == '\n') {
            lineno++;
            pdoc->lineptr[lineno] = i + 1;
        }
    }
    pdoc->lineCount = lineno + 1;
    fclose(fp);
}

/*
// �����ļ���ȡ������p�����line�У�ͬ��(��չ)�����ڴ�
TCHAR* fillLine(TCHAR* line, TCHAR* p)
{
    // ��lineΪNULL����������Ϊ���ζ�ȡ��������µ��ڴ��
    if (line == NULL) {
        line = (TCHAR*)malloc((wcslen(p) + 1) * sizeof(TCHAR));
        if (line != NULL) wcscpy(line, p);
    }
    // ���򣬱��������Ѵ������ݣ���������չ�����ڴ棬�����µ�����׷����β��
    else {
        line = (TCHAR*)realloc(line, (wcslen(line) + wcslen(p) + 1) * sizeof(TCHAR));
        if (line != NULL) wcscat(line, p);
    }
    return line;
}

//  ����һ�����е����洢
void initDoc(TTextDoc* pdoc, TCHAR* fname)
{
    FILE* fp;
    // ����ļ�����
    int codetype = fileEncodeType(fname);
    // �����ļ����벻ͬ�����ò�ͬ�Ĵ򿪷�ʽ
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
        // ���ж�ȡ�ı���ÿ�ζ�ȡMAX_LEN��������Ҫ���ζ�ȡͬһ��
        TCHAR* p = fgetws(buff, MAX_LEN, fp);
        // �ļ���ȡ��ϣ�ֱ���˳�ѭ��
        if (p == NULL)
            break;

        // ���س������滻Ϊ0
        TCHAR* crptr = wcschr(p, '\n');
        if (crptr != NULL) *crptr = '\0';
        // ���л��ļ�β
        if (crptr != NULL || feof(fp)) {
            // ����ʵ���ַ����������·���ÿ�е��ڴ棬�����˷�
            line = fillLine(line, p);
            // ��ָ�������¼���ڴ���ַ
            pdoc->lines[pdoc->lineCount] = line;
            pdoc->lineCount++;
            // �н�������line��ʼ��ΪNULL
            line = NULL;

            // ��������������򵥴���ֱ�Ӳ��ٶ�ȡ
            if (pdoc->lineCount >= MAX_LINE)
                break;
        }
        // ��ǰfgetws��ȡ����δ��������Ҫ��ζ�ȡ
        else {
            // �����ļ���ȡ�������ݣ�����׷�ӽ����ڴ���У����б�Ҫ������
            line = fillLine(line, p);
        }
    }
    fclose(fp);
}
*/

// �ͷ��ı��ڴ�
void freeText(TTextDoc* pdoc)
{
    free(pdoc->text);
}

// ��ȡָ���е�����
TCHAR* getLine(TTextDoc* pdoc, int lineno)
{
    // ������������ĺϷ���
    if (lineno >= pdoc->lineCount) return NULL;

    // ����ʼλ��ָ��
    TCHAR* ptr = (TCHAR*)pdoc->text + pdoc->lineptr[lineno];
    long len;
    if (lineno == pdoc->lineCount - 1)
        len = wcslen(ptr);
    else
        len = pdoc->lineptr[lineno + 1] - pdoc->lineptr[lineno];

    // ȷ�������������
    len = min(len, MAX_LEN);

    wcsncpy(pdoc->linebuff, ptr, len);
    pdoc->linebuff[len] = '\0';
    return pdoc->linebuff;
}

void loadTextFile(TTextDoc* pdoc)
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH] = { 0 };	//���ڽ����ļ���
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.hwndOwner = GetHWnd();
    ofn.lStructSize = sizeof(ofn); // �ṹ��С
    ofn.lpstrFile = szFile;	       //���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
    ofn.nMaxFile = MAX_PATH;       // ·����С
    ofn.lpstrFilter = TEXT("�ı��ļ�(txt)\0*.txt; *.cpp; *.html;\0\0"); // �ļ�����
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // ��־
    if (GetOpenFileName(&ofn)) {
        initDoc(pdoc, ofn.lpstrFile);
    }
}