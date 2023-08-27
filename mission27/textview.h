#pragma once

#include <easyx.h>
#include "bbcode.h"
#include "textdoc.h"
#include "list.h"


typedef void (TProcBBCode)(struct tagTextView* pview, BBCodeType type, TCHAR* value, int tagState);

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
    TTextDoc* pdoc;
    TListp list;
    TProcBBCode* handlers[eBBCode_MAX];
} TTextView;

typedef struct tagHandler {
    BBCodeType bbcodetype;
    TProcBBCode* proc;
} THandler;

void initView(TTextView* pview, TTextDoc* pdoc);
void displayText(TTextView* pview);
void displayRichText(TTextView* pview);
