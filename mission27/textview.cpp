#define _CRT_SECURE_NO_WARNINGS

#include <easyx.h>
#include <stdio.h>
#include "textdoc.h"
#include "textview.h"
#include "bbcode.h"
#include "list.h"

void initView(TTextView* pview, TTextDoc* pdoc)
{
    pview->r.left = 10;
    pview->r.top = 10;
    pview->r.right = getwidth() - 10;
    pview->r.bottom = getheight() - 10;
    pview->linespace = 2;
    gettextstyle(&pview->font);
    _tcscpy(pview->font.lfFaceName, L"΢���ź�");
    //   pview->font.lfQuality = ANTIALIASED_QUALITY;
    pview->pdoc = pdoc;
    initList(&pview->list, 100);
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

void displayText(TTextView* pview)
{
    pview->font.lfHeight = 20;
    settextstyle(&pview->font);
    int y = pview->r.top;
    for (int i = 0; i < pview->pdoc->lineCount; i++) {
        TCHAR* p = getLine(pview->pdoc, i);
        outtextxy(pview->r.left, y, p);
        int th = textheight(p);
        if (th == 0) th = textheight(L" ");
        y = y + th + pview->linespace;
    }
}

TContext* createContext()
{
    return (TContext*)malloc(sizeof(TContext));
}

void freeContext(TContext* ctx)
{
    free(ctx);
}

void procTag_B(TTextView* pview, TCHAR* value, int tagState)
{
    LOGFONT font;
    TContext* ctx;
    if (tagState == tagOPEN) {
        // ���浱ǰctx
        TContext* ctx = createContext();
        gettextstyle(&ctx->font);
        ctx->color = gettextcolor();
        ctx->bbcodetype = eBBCode_B;
        push(&pview->list, ctx);
        // ���ĵ�ǰ����
        font.lfWeight = FW_BOLD;
        settextstyle(&font);
    }
    else {
        // ��ջ
        pop_stack(&pview->list, &ctx);
        // ����ƥ��
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

void procTag_I(TTextView* pview, TCHAR* value, int tagState)
{
    LOGFONT font;
    if (tagState == tagCLOSE) {
        gettextstyle(&font);
        font.lfItalic = 1;
        settextstyle(&font);
    }
    else {

    }
}

THandler handlers[] = {
    {eBBCode_B, procTag_B},
    {eBBCode_I, procTag_I},
    {eBBCode_NULL},
};

void displayRichText(TTextView* pview)
{
    pview->font.lfHeight = 20;
    settextstyle(&pview->font);
    int x = pview->r.left;
    int y = pview->r.top;
    int th = 0;

    TToken token;
    long ptr = 0;
    gettoken(pview->pdoc->text, &ptr, &token);
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
            break;
        case TOKEN_BBCODE:
            BBCodeType type;
            TCHAR* value;
            int state;
            parseBBCode(&token, &type, value, &state);

            break;
        }

        gettoken(pview->pdoc->text, &ptr, &token);
    }
}