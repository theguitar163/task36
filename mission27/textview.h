#pragma once

#include <easyx.h>
#include "textdoc.h"

typedef struct tagTextView {
    RECT r;
    LOGFONT font;
    int linespace;
    TTextDoc* pdoc;
} TTextView;

typedef struct tagContext {
    COLORREF color;
    LOGFONT font;
} TViewContext;

void initView(TTextView* pview, TTextDoc* pdoc);
void paintView(TTextView* pview);
void paintTokenView(TTextView* pview);
