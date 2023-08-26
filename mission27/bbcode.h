#pragma once

#include <easyx.h>

#define MAX_TOKEN 1000

#define TOKEN_TEXT   1
#define TOKEN_BBCODE 2
#define TOKEN_CRLF   3
#define TOKEN_END    4

#define tagOPEN  0
#define tagCLOSE 1

typedef struct tagToken {
    int tokentype;
    TCHAR content[MAX_TOKEN];
} TToken;

typedef enum tagBBCodeType{
    eBBCode_B = 0, eBBCode_I, eBBCode_S, eBBCode_U, eBBCode_FONT, eBBCode_COLOR,
    eBBCode_MAX, // 用于标志THandler数组结束
} BBCodeType;

typedef struct tagBBCodeMap {
    BBCodeType bbcodetype;
    const TCHAR* keyopen;
    const TCHAR* keyclose;
} BBCodeMap;

void gettoken(TCHAR* text, long* pp, TToken* ptoken);
void parseBBCode(TToken* ptoken, BBCodeType* ptype, TCHAR** pvalue, int* pstate);