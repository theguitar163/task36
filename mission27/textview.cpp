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

void procTag_FONT(TTextView* pview, BBCodeType type, TCHAR* value, int tagState)
{
    LOGFONT font;
    TContext* ctx;
    if (tagState == tagOPEN) {
        // 取得当前字体
        gettextstyle(&font);
        // 创建ctx
        ctx = createContext();
        ctx->font = font;
        ctx->color = gettextcolor();
        ctx->bbcodetype = eBBCode_B;
        push(&pview->list, ctx);
        switch (type) {
        case eBBCode_B:
            // 更改当前字体
            font.lfWeight = FW_BOLD;
            settextstyle(&font);
            break;
        case eBBCode_I:
            // 更改当前字体
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
        // 出栈
        if (pop_stack(&pview->list, &ctx)) {
            // 类型匹配
            if (ctx->bbcodetype == eBBCode_B) {
                settextcolor(ctx->color);
                settextstyle(&ctx->font);
                freeContext(ctx);
            }
            else {
                push(&pview->list, ctx);
            }
        }
    }
}


void initView(TTextView* pview, TTextDoc* pdoc)
{
    pview->r.left = 10;
    pview->r.top = 10;
    pview->r.right = getwidth() - 10;
    pview->r.bottom = getheight() - 10;
    pview->linespace = 2;
    pview->defaultheight = textheight(' ');
    gettextstyle(&pview->font);
    _tcscpy(pview->font.lfFaceName, L"微软雅黑");
    //   pview->font.lfQuality = ANTIALIASED_QUALITY;
    pview->pdoc = pdoc;
    initList(&pview->list, 100);
    
    pview->handlers[eBBCode_B] = &procTag_FONT;
    pview->handlers[eBBCode_I] = &procTag_FONT;
    pview->handlers[eBBCode_S] = &procTag_FONT;
    pview->handlers[eBBCode_U] = &procTag_FONT;
    pview->handlers[eBBCode_FONT] = &procTag_FONT;
    pview->handlers[eBBCode_COLOR] = &procTag_FONT;
    pview->handlers[eBBCode_SIZE] = &procTag_FONT;
}

void setViewport(TTextView* pview, RECT r)
{
    pview->r = r;
}

void freeView(TTextView* pview)
{
    TContext* ctx;
    while (pview->list.size > 0) {
        pop_stack(&pview->list, &ctx);
        free(ctx);
    }
    freeList(&pview->list);
}

void displayPlainText(TTextView* pview)
{
    pview->font.lfHeight = 20;
    settextstyle(&pview->font);
    int th = 0;
    int starty = pview->r.top + pview->offset;
    int y = starty;
    for (int i = 0; i < pview->pdoc->lineCount; i++) {
        TCHAR* p = getLine(pview->pdoc, i);
        outtextxy(pview->r.left, y, p);
        th = textheight(p);
        if (th == 0) th = textheight(L" ");
        y = y + th + pview->linespace;
    }
    pview->allheight = y + th - starty;
}


void displayRichText(TTextView* pview)
{
    pview->font.lfHeight = 20;
    settextstyle(&pview->font);
    int x = pview->r.left;
    int starty = pview->r.top + pview->offset;
    int y = starty;
    int th = 0;

    TToken token;
    long ptr = 0;
    getToken(pview->pdoc->text, &ptr, &token);
    while (token.tokentype != TOKEN_END) {
        switch (token.tokentype) {
        case TOKEN_TEXT:
            outtextxy(x, y, token.content);
            th = max(th, textheight(token.content));
            x = x + textwidth(token.content);
            break;
        case TOKEN_CRLF:
            x = pview->r.left;
            y = y + th + pview->linespace;
            th = pview->defaultheight;
            break;
        case TOKEN_BBCODE:
            BBCodeType type;
            TCHAR* value = NULL;
            int state;
            parseBBCode(&token, &type, &value, &state);
            if (type < eBBCode_MAX)
                pview->handlers[type](pview, type, value, state);
            break;
        }

        getToken(pview->pdoc->text, &ptr, &token);
    }
    pview->allheight = y + th - starty;
}

void scrollPage(TTextView* pview, int dist)
{
    pview->offset += dist;
    if (pview->allheight + pview->offset < pview->defaultheight)
        pview->offset = (pview->defaultheight - pview->allheight);
    if (pview->offset > 0)
        pview->offset = 0;
}