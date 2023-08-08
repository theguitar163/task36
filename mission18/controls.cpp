#include <easyx.h>
#include "controls.h"

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int mod)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->x2 = x2;
    pbtn->y2 = y2;
    pbtn->w = x2 - x;
    pbtn->h = y2 - y;
    pbtn->color = color;
    pbtn->text = text;
    //   wcscpy_s(pbtn->text, text);
    pbtn->type = mod;
}

void initButton(TButton* pbtn, int x1, int y1, int radius, int mod)
{
    pbtn->x = x1;
    pbtn->y = y1;
    pbtn->radius = radius;
    pbtn->type = mod;
}

void initButton(TButton* pbtn, int x, int y, int w, int h, int mod)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->w = w;
    pbtn->h = h;
    pbtn->type = mod;
    pbtn->text = NULL;
    //    wcscpy_s(pbtn->text, L"\0");
}

void initButton(TButton* pbtn, int x, int y)
{
    pbtn->x = x;
    pbtn->y = y;
}

// ���ư�ť
void drawButton(TButton* pbtn)
{
    int ox, oy;
    // ��ȡ����ԭ�����꣬��������Ϊ����ԭ��
    if (pbtn->container == NULL) {
        ox = 0;
        oy = 0;
    }
    else {
        ox = pbtn->container->x;
        oy = pbtn->container->y;
    }

    // ���ư�ť��������Բ�Ρ�Բ�Ǿ��Ρ���������
    setfillcolor(pbtn->color);
    setlinecolor(BLACK);
    settextcolor(BLACK);
    RECT r = { ox + pbtn->x, oy + pbtn->y, ox + pbtn->x + pbtn->w, oy + pbtn->y + pbtn->h };
    if (pbtn->type == btCIRCLE) {
        fillellipse(r.left, r.top, r.right, r.bottom);
    }
    else if (pbtn->type == btRDRECT) {
        int esize = ((pbtn->w > pbtn->h) ? pbtn->h : pbtn->w) / 4;
        fillroundrect(r.left, r.top, r.right, r.bottom, esize, esize);
    }
    else {
        fillrectangle(r.left, r.top, r.right, r.bottom);
    }
    // ���ư�ť����
    if (pbtn->text != NULL)
        drawtext(pbtn->text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

// �жϵ�pt�Ƿ��ڰ�ť��
int ptInButton(POINT p, TButton* pbtn)
{
    int bx, by;
    // ��ȡ����ԭ�����꣬��������Ϊ����ԭ��
    if (pbtn->container == NULL) {
        bx = pbtn->x;
        by = pbtn->y;
    }
    else {
        bx = pbtn->container->x + pbtn->x;
        by = pbtn->container->y + pbtn->y;
    }
    // ��Բ��ť��������Ƿ�����Բ��
    if (pbtn->type == btCIRCLE) {
        double a = pbtn->w / 2;
        double b = pbtn->h / 2;
        double dx = p.x - (bx + a);
        double dy = p.y - (by + b);
        if ((dx * dx) / (a * a) + (dy * dy) / (b * b) <= 1.0)
            return 1;
    }
    else {
        if (p.x >= bx && p.x <= bx + pbtn->w && p.y >= by && p.y <= by + pbtn->h)
            return 1;
    }
    return 0;
}

void initPanel(TPanel* ppanel, int x, int y, int w, int h)
{
    ppanel->x = x;
    ppanel->y = y;
    ppanel->w = w;
    ppanel->h = h;
}

void initPanel(TPanel* ppanel, int size, int align)
{
    switch (align) {
    case alTOP:
        ppanel->x = 0;
        ppanel->y = 0;
        ppanel->w = getwidth();
        ppanel->h = size;
        break;
    case alBOTTOM:
        ppanel->x = 0;
        ppanel->y = getheight() - size;
        ppanel->w = getwidth();
        ppanel->h = size;
        break;
    case alLEFT:
        ppanel->x = 0;
        ppanel->y = 0;
        ppanel->w = size;
        ppanel->h = getheight();
        break;
    case alRIGHT:
        ppanel->x = getwidth() - size;
        ppanel->y = 0;
        ppanel->w = size;
        ppanel->h = getheight();
    }
}

void addButton(TPanel* ppanel, TButton* pbutton)
{
    if (ppanel->btnCount < MAX_BUTTON - 1) {
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

void drawPanel(TPanel* ppanel)
{
    for (int i = 0; i < ppanel->btnCount; i++) {
        drawButton(ppanel->pbuttons[i]);
    }
}