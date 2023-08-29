#define _CRT_SECURE_NO_WARNINGS

#include <easyx.h>
#include <stdio.h>
#include "textdoc.h"
#include "textview.h"
#include "bbcode.h"
#include "list.h"

TContext* createContext()
{
    return (TContext*)malloc(sizeof(TContext));
}

void freeContext(TContext* ctx)
{
    free(ctx);
}

typedef struct tagColorName {
    const TCHAR* name;
    const COLORREF color;
} TColorName;

TColorName colorNames[] = {
    {L"BLACK", BLACK},
    {L"BLUE", BLUE},
    {L"GREEN", GREEN},
    {L"CYAN", CYAN},
    {L"RED", RED},
    {L"MAGENTA", MAGENTA},
    {L"BROWN", BROWN},
    {L"LIGHTGRAY", LIGHTGRAY},
    {L"DARKGRAY", DARKGRAY},
    {L"LIGHTBLUE", LIGHTBLUE},
    {L"LIGHTGREEN", LIGHTGREEN},
    {L"LIGHTCYAN", LIGHTCYAN},
    {L"LIGHTRED", LIGHTRED},
    {L"LIGHTMAGENTA",LIGHTMAGENTA},
    {L"YELLOW", YELLOW},
    {L"WHITE", WHITE},
    {NULL}
};

COLORREF string2color(TCHAR* str, COLORREF defaultColor)
{
    int i = 0;
    while (colorNames[i].name != NULL) {
        if (_wcsicmp(colorNames[i].name, str) == 0) {
            return colorNames[i].color;
        }
        i++;
    }
    return defaultColor;
}

void procTag_FONT(CTextView* pview, BBCodeType type, TCHAR* value, int tagState)
{
    LOGFONT font;
    TContext* ctx;
    if (tagState == tagOPEN) {
        // ȡ�õ�ǰ����
        gettextstyle(&font);
        // ����ctx
        ctx = createContext();
        ctx->font = font;
        ctx->color = gettextcolor();
        ctx->bbcodetype = eBBCode_B;
        // ��ǰctxѹջ
        push(&pview->m_list, ctx);
        // ����BBCodeTypeִ����Ӧ�Ĳ���
        switch (type) {
        case eBBCode_B:
            // ���ĵ�ǰ����
            font.lfWeight = FW_BOLD;
            settextstyle(&font);
            break;
        case eBBCode_I:
            // ���ĵ�ǰ����
            font.lfItalic = 1;
            settextstyle(&font);
            break;
        case eBBCode_S:
            font.lfStrikeOut = 1;
            settextstyle(&font);
            break;
        case eBBCode_U:
            font.lfUnderline = 1;
            settextstyle(&font);
            break;
        case eBBCode_FONT:
            wcscpy(font.lfFaceName, value);
            settextstyle(&font);
            break;
        case eBBCode_COLOR:
            int r, g, b;
            if (swscanf(value, L"#%2x%2x%2x", &r, &g, &b) == 3) {
                settextcolor(RGB(r, g, b));
            }
            else {
                settextcolor(string2color(value, gettextcolor()));
            }
            break;
        case eBBCode_SIZE:
            int size;
            if (swscanf(value, L"%d", &size) == 1) {
                font.lfHeight = size;
                settextstyle(&font);
            }
            break;
        }
    }
    else {
        // ctx��ջ
        if (pop_stack(&pview->m_list, &ctx)) {
            // ����ƥ�䣬�ָ�ctx
            if (ctx->bbcodetype == eBBCode_B) {
                settextcolor(ctx->color);
                settextstyle(&ctx->font);
                freeContext(ctx);
            }
            else {
                // ��ƥ��������ѹջ
                push(&pview->m_list, ctx);
            }
        }
    }
}


void CTextView::initView(CTextDoc* pdoc)
{
    m_r.left = 10;
    m_r.top = 10;
    m_r.right = getwidth() - 10;
    m_r.bottom = getheight() - 10;
    m_linespace = 2;
    m_defaultheight = textheight(' ');
    gettextstyle(&m_font);
    _tcscpy(m_font.lfFaceName, L"΢���ź�");
    //   m_font.lfQuality = ANTIALIASED_QUALITY;
    m_pdoc = pdoc;
    initList(&m_list, 100);
    
    m_handlers[eBBCode_B] = &procTag_FONT;
    m_handlers[eBBCode_I] = &procTag_FONT;
    m_handlers[eBBCode_S] = &procTag_FONT;
    m_handlers[eBBCode_U] = &procTag_FONT;
    m_handlers[eBBCode_FONT] = &procTag_FONT;
    m_handlers[eBBCode_COLOR] = &procTag_FONT;
    m_handlers[eBBCode_SIZE] = &procTag_FONT;
}

void CTextView::setViewport(RECT r)
{
    m_r = r;
}

void CTextView::freeView()
{
    TContext* ctx;
    while (m_list.size > 0) {
        pop_stack(&m_list, &ctx);
        free(ctx);
    }
    freeList(&m_list);
}

void CTextView::displayPlainText()
{
    m_font.lfHeight = 20;
    settextstyle(&m_font);
    int th = 0;
    int starty = m_r.top + m_offset;
    int y = starty;
    for (int i = 0; i < m_pdoc->m_lineCount; i++) {
        TCHAR* p = m_pdoc->getLine(i);
        outtextxy(m_r.left, y, p);
        th = textheight(p);
        if (th == 0) th = textheight(L" ");
        y = y + th + m_linespace;
    }
    m_allheight = y + th - starty;
}


void CTextView::displayRichText()
{
    m_font.lfHeight = 20;
    settextstyle(&m_font);
    int x = m_r.left;
    int starty = m_r.top + m_offset;
    int y = starty;
    int th = 0;

    TToken token;
    long ptr = 0;
    getToken(m_pdoc->m_text, &ptr, &token);
    while (token.tokentype != TOKEN_END) {
        switch (token.tokentype) {
        case TOKEN_TEXT:
            outtextxy(x, y, token.content);
            th = max(th, textheight(token.content));
            x = x + textwidth(token.content);
            break;
        case TOKEN_CRLF:
            x = m_r.left;
            y = y + th + m_linespace;
            th = m_defaultheight;
            break;
        case TOKEN_BBCODE:
            BBCodeType type;
            TCHAR* value = NULL;
            int state;
            parseBBCode(&token, &type, &value, &state);
            if (type < eBBCode_MAX)
                m_handlers[type](this, type, value, state);
            break;
        }

        getToken(m_pdoc->m_text, &ptr, &token);
    }
    m_allheight = y + th - starty;
}

void CTextView::scrollPage(int dist)
{
    m_offset += dist;
    if (m_allheight + m_offset < m_defaultheight)
        m_offset = (m_defaultheight - m_allheight);
    if (m_offset > 0)
        m_offset = 0;
}