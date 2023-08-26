#pragma once

#include <easyx.h>
#include "bbcode.h"
#include "textdoc.h"

typedef struct tagContext {
    BBCodeType bbcodetype;
    COLORREF color;
    LOGFONT font;
} TContext, ITEM;

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
    TTextDoc* pdoc;
   TListp list;
} TTextView;

typedef void (TProcBBCode)(TTextView* pview, TCHAR* value, int tagState);

typedef struct tagHandler {
    BBCodeType bbcodetype;
    TProcBBCode* proc;
} THandler;

void initView(TTextView* pview, TTextDoc* pdoc);
void displayText(TTextView* pview);
void displayRichText(TTextView* pview);
