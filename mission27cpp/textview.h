#pragma once

#include <easyx.h>
#include "bbcode.h"
#include "textdoc.h"
#include "list.h"

class CTextView;

typedef void (TProcBBCode)(CTextView* pview, BBCodeType type, TCHAR* value, int tagState);

typedef struct tagHandler {
    BBCodeType bbcodetype;
    TProcBBCode* proc;
} THandler;

class CTextView 
{
public:
    RECT m_r;
    LOGFONT m_font;
    int m_linespace;
    int m_defaultheight;
    int m_allheight;
    int m_allwidth;
    int m_offset = 0;
    CTextDoc* m_pdoc;
    TListp m_list;
    TProcBBCode* m_handlers[eBBCode_MAX];

    void initView(CTextDoc* pdoc);
    void displayPlainText();
    void displayRichText();
    void setViewport(RECT r);
    void scrollPage(int dist);
    void freeView();
};


