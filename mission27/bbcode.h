#pragma once

typedef struct tagContext {
    COLORREF color;
    LOGFONT font;
} TViewContext;

#define BBCODE_TEXT 1
#define BBCODE_TAG  2
#define BBCODE_CRLF   3
#define BBCODE_END  4

typedef void (TProcTag)(TCHAR* key, TCHAR* value, int tagState);

typedef struct tagHandler {
    const TCHAR* keyopen;
    const TCHAR* keyclose;
    TProcTag* proc;
} THandler;

typedef struct tagToken {
    int type;
    TCHAR content[MAX_LEN];
} TToken;

#define tagOPEN  0
#define tagCLOSE 1