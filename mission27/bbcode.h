#pragma once

#include <easyx.h>

#define MAX_TOKEN 1000

#define TOKEN_TEXT   1
#define TOKEN_BBCODE 2
#define TOKEN_CRLF   3
#define TOKEN_END    4

#define tagOPEN  0
#define tagCLOSE 1

typedef enum tagBBCodeType {
    eBBCode_NULL, // 用于标志THandler数组结束
    eBBCode_B, eBBCode_I, eBBCode_S, eBBCode_U, eBBCode_FONT, eBBCode_COLOR,
} BBCodeType;

typedef void (TProcTag)(TCHAR* key, TCHAR* value, int tagState);

typedef struct tagHandler {
    BBCodeType bbcodetype;
    const TCHAR* keyopen;
    const TCHAR* keyclose;
    TProcTag* proc;
} THandler;

typedef struct tagToken {
    int tokentype;
    TCHAR content[MAX_TOKEN];
} TToken;


void gettoken(TCHAR* text, long* pp, TToken* ptoken);
void parseBBCode(TToken* ptoken, THandler handlers[]);