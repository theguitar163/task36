#define _CRT_SECURE_NO_WARNINGS

#include "textdoc.h"
#include "textview.h"
#include "bbcode.h"

void initView(TTextView* pview, TTextDoc* pdoc)
{
    pview->r.left = 10;
    pview->r.top = 10;
    pview->r.right = getwidth() - 10;
    pview->r.bottom = getheight() - 10;
    pview->linespace = 2;
    gettextstyle(&pview->font);
    _tcscpy(pview->font.lfFaceName, L"Î¢ÈíÑÅºÚ");
    //   pview->font.lfQuality = ANTIALIASED_QUALITY;
    pview->pdoc = pdoc;
}

void paintView(TTextView* pview)
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

void procTag_B(TCHAR* key, TCHAR* value, int tagState)
{
    LOGFONT font;
    if (tagState == tagOPEN) {
        gettextstyle(&font);
        font.lfWeight = FW_BOLD;
        settextstyle(&font);
    }
    else {

    }
}

void procTag_I(TCHAR* key, TCHAR* value, int tagState)
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
    {eBBCode_B, L"B", L"/B", procTag_B},
    {eBBCode_I, L"I", L"/I", procTag_I},
    {eBBCode_NULL},
};

void paintTokenView(TTextView* pview)
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
            parseBBCode(&token, handlers);
            break;
        }

        gettoken(pview->pdoc->text, &ptr, &token);
    }
}