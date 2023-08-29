#pragma once

#include <easyx.h>

#define MAX_LINE 1000     // �������
#define MAX_LEN  1000     // ÿ���������

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
    TCHAR* m_text;                      // �ı��ļ�ȫ�����ݣ����ַ�����ʽ����
    long m_lineptr[MAX_LINE];           // ��ƫ����ָ��
    int m_lineCount;                    // �ı��ļ�������
    TCHAR m_linebuff[MAX_LEN + 1];      // ���ڻ�ȡ�����ݵ��ڲ�������

    int fileEncodeType(TCHAR* fname);
public:
    // ��ʼ��doc�ṹ
    void initDoc(TCHAR* fname);
    // ��ȡָ���е�����
    TCHAR* getLine(int lineno);
    // �ͷ��ı��ڴ�
    void freeDoc();
};
