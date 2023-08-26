#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // �������
#define MAX_LEN  1000     // ÿ���������

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef struct tagTextDocument {
    TCHAR* text;                      // �ı��ļ�ȫ�����ݣ����ַ�����ʽ����
    long lineptr[MAX_LINE];           // ��ƫ����ָ��
    int lineCount;                    // �ı��ļ�������
    TCHAR linebuff[MAX_LEN + 1];      // ���ڻ�ȡ�����ݵ��ڲ�������
} TTextDoc;

#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define GBK       5

// ��ʼ��doc�ṹ
void initDoc(TTextDoc* pdoc, TCHAR* fname);
// ��ȡָ���е�����
TCHAR* getLine(TTextDoc* pdoc, int lineno);
// �ͷ��ı��ڴ�
void freeText(TTextDoc* pdoc);
// �����ı��ļ�
void loadTextFile(TTextDoc* pdoc);
