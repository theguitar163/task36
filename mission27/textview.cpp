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

void paintTokenView(TTextView* pview)
{
    pview->font.lfHeight = 20;
    settextstyle(&pview->font);
    int x = pview->r.left;
    int y = pview->r.top;
    int th = 0;

    TToken token;
    long ptr = 0;
    gettoken(pview->pdoc, &ptr, &token);
    while (token.type != BBCODE_END) {
        switch (token.type) {
        case BBCODE_TEXT:
            outtextxy(x, y, token.content);
            th = max(th, textheight(token.content));
            x = x + textwidth(token.content);
            break;
        case BBCODE_CRLF:
            x = pview->r.left;
            y = y + th + pview->linespace;
            break;
        case BBCODE_TAG:
            parseTag(&token);
            break;
        }

        gettoken(pview->pdoc, &ptr, &token);
    }
}