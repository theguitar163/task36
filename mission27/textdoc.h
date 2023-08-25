#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // �������
#define MAX_LEN  1000     // ÿ���������

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef struct tagTextDocument {
    TCHAR* text;
    long lineptr[MAX_LINE];
    int lineCount;

    TCHAR linebuff[MAX_LEN + 1];
} TTextDoc;

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
#define UTF8BOM   1
#define UNICODELE 2
#define UNICODEBE 3
#define UTF8      4
#define GBK       5