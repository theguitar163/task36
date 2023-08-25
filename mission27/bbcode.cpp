#define _CRT_SECURE_NO_WARNINGS

#include <easyx.h>
#include "bbcode.h"

// 去除前后空白字符
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

// 从字符串中取得token
void gettoken(TCHAR* text, long* pp, TToken* ptoken)
{
    // 初始化偏移量指针
    long ptr = *pp;

    // 记录起始位置
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

// 解析BBCode
// !隐患，token是全局变量。在处理过程中还会gettoken，引起token的变化
// 因此再次处理必须重新声明token，但是当前指针尤为重要
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
