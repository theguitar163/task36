#define _CRT_SECURE_NO_WARNINGS

#include <easyx.h>
#include "bbcode.h"

// ȥ��ǰ��հ��ַ�
TCHAR* trim(TCHAR* str)
{
    if (str == NULL || wcslen(str) == 0)
        return NULL;
    TCHAR* p;
    // �޳�β���ո�
    p = str + wcslen(str) - 1;
    while (iswspace(*p)) {
        *p = L'\0';
        p--;
    }
    // �޳���ͷ�ո�
    p = str;
    while (iswspace(*p)) p++;
    return p;
}

// ���ַ�����ȡ��token
void gettoken(TCHAR* text, long* pp, TToken* ptoken)
{
    // ��ʼ��ƫ����ָ��
    long ptr = *pp;

    // ��¼��ʼλ��
    long start = ptr;

    if (text[ptr] == L'\0') {
        ptoken->tokentype = TOKEN_END;
    }
    else if (text[ptr] == L'\n') {
        ptoken->tokentype = TOKEN_CRLF;
        ptr++;
    }
    else if (text[ptr] == L'[') {
        while (true) {
            TCHAR c = text[ptr];
            if (c == L'\0' || c == L'\n') {
                ptoken->tokentype = TOKEN_TEXT;
                wcsncpy(ptoken->content, text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                break;
            }
            else if (c == L']') {
                ptr++;
                ptoken->tokentype = TOKEN_BBCODE;
                wcsncpy(ptoken->content, text + start + 1, ptr - start - 2);
                ptoken->content[ptr - start - 2] = L'\0';
                break;
            }
            ptr++;
        }
    }
    else {
        while (true) {
            TCHAR c = text[ptr];
            if (c == L'\0' || c == L'[' || c == L'\n') {
                ptoken->tokentype = TOKEN_TEXT;
                wcsncpy(ptoken->content, text + start, ptr - start);
                ptoken->content[ptr - start] = L'\0';
                *pp = ptr;
                break;
            }
            ptr++;
        }
    }
    *pp = ptr;
}

// ����BBCode
// !������token��ȫ�ֱ������ڴ�������л���gettoken������token�ı仯
// ����ٴδ��������������token�����ǵ�ǰָ����Ϊ��Ҫ
void parseBBCode(TToken* ptoken, THandler handlers[])
{
    TCHAR* key;
    TCHAR* value;
    TCHAR* ptr;
    key = trim(wcstok(ptoken->content, L"=", &ptr));
    value = trim(wcstok(NULL, L"=", &ptr));

    int i = 0;
    while (handlers[i].bbcodetype != eBBCode_NULL) {
        if (_wcsicmp(key, handlers[i].keyopen)) {
            handlers[i].proc(key, value, tagOPEN);
        }
        else if (_wcsicmp(key, handlers[i].keyclose)) {
            handlers[i].proc(key, value, tagCLOSE);
        }
        i++;
    }
}
