#include <easyx.h>
#include "panel.h"

void setButtonColor(TButton* pbtn, COLORREF c)
{
    pbtn->color = c;
}

COLORREF getButtonColor(TButton* pbtn)
{
    return pbtn->color;
}

// ���ð�ťλ��
void setButtonPos(TButton* pbtn, int x, int y)
{
    pbtn->x = x;
    pbtn->y = y;
}

// ���ð�ťTag��ֵ
void setButtonTag(TButton* pbtn, LONG tag)
{
    pbtn->tag = tag;
}

LONG getButtonTag(TButton* pbtn)
{
    return pbtn->tag;
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
    setlinecolor(BLACK);
    settextcolor(BLACK);
    RECT r = { ox + pbtn->x, oy + pbtn->y, ox + pbtn->x + pbtn->w, oy + pbtn->y + pbtn->h };
    if (pbtn->shape == bsCIRCLE) {
        if (pbtn->selected) {
            setlinecolor(LIGHTBLUE);
            ellipse(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillellipse(r.left, r.top, r.right, r.bottom);
    }
    else if (pbtn->shape == bsRDRECT) {
        int esize = ((pbtn->w > pbtn->h) ? pbtn->h : pbtn->w) / 4;
        if (pbtn->selected) {
            setlinecolor(LIGHTBLUE);
            roundrect(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2, esize + 2, esize + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillroundrect(r.left, r.top, r.right, r.bottom, esize, esize);
    }
    else {
        if (pbtn->selected) {
            setlinecolor(LIGHTBLUE);
            rectangle(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillrectangle(r.left, r.top, r.right, r.bottom);
    }
    // ���ư�ť����
    if (pbtn->text != NULL) {
        TCHAR str[200];
        if (pbtn->type == btNUM)
            swprintf_s(str, L"%s[%d]", pbtn->text, pbtn->tag);
        else if (pbtn->type == btBOOL)
            swprintf_s(str, L"%s[%s]", pbtn->text, (pbtn->tag == 0) ? L"��" : L"��");
        else
            wcscpy_s(str, pbtn->text);
        settextstyle(12, 0, L"����");
        // �Զ�����������ɫ����Ҫ��������
        COLORREF c = pbtn->color;
        float h, s, l;
        RGBtoHSL(c, &h, &s, &l);
        settextcolor(HSLtoRGB(h, s, (l > 0.5) ? 0 : 1));
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }
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
    if (pbtn->shape == bsCIRCLE) {
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

// ��ʼ�����ư�
// ���ݶ��뷽ʽ���Զ����ÿ��ư�λ��
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

// ��Ӱ�ť�������ǰ�ťλ��
void addButton(TPanel* ppanel, TButton* pbutton)
{
    if (ppanel->btnCount < MAX_BUTTON - 1) {
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

// ��Ӱ�ť�����ư���
// ����ָ������ӷ�ʽ�ͼ���Զ����ð�ť��λ��
void addButton(TPanel* ppanel, TButton* pbutton, int spacing, int dir)
{
    if (ppanel->btnCount < MAX_BUTTON - 1) {
        TButton* p = ppanel->pbuttons[ppanel->btnCount - 1];
        if (ppanel->btnCount == 0) {
            pbutton->x = spacing;
            pbutton->y = spacing;
        }
        else if (dir == adBOTTOM) {
            pbutton->x = p->x;
            pbutton->y = p->y + p->h + spacing;
        }
        else if (dir == adNEWLINE) {
            pbutton->x = spacing;
            pbutton->y = p->y + p->h + spacing;
        }
        else { // Ĭ��Ϊ�Ҳ����
            pbutton->x = p->x + p->w + spacing;
            pbutton->y = p->y;
        }
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

// ���ƿ��ư弰�������İ�ť
void drawPanel(TPanel* ppanel)
{
    setlinecolor(BLACK);
    setfillcolor(WHITE);
    setlinestyle(PS_SOLID, 1);
    fillrectangle(ppanel->x, ppanel->y, ppanel->x + ppanel->w, ppanel->y + ppanel->h);
    for (int i = 0; i < ppanel->btnCount; i++) {
        drawButton(ppanel->pbuttons[i]);
    }
}

// ��ť����¼����ɴ���
void buttonClick(TPanel* ppanel, int x, int y)
{
    // �������а�ť
    for (int i = 0; i < ppanel->btnCount; i++) {
        // �жϵ���������Ƿ��ڰ�ť��
        if (ptInButton({ x, y }, ppanel->pbuttons[i])) {
            ppanel->btnClicked = i;
            TFunction* pfun = ppanel->pbuttons[i]->pOnClick;
            if (pfun != NULL)
                // (*pfun)(ppanel->ppainter);
                (*pfun)();
            int btnIdx = ppanel->btnClicked;
            updateButtonGroup(ppanel, btnIdx);
            drawPanel(ppanel);
            FlushBatchDraw();

            break;
        }
    }
}

// Ⱥ�鰴ť������Զ����°�ťȺ��������ť״̬
void updateButtonGroup(TPanel* ppanel, int btnIdx)
{
    int gid = ppanel->pbuttons[btnIdx]->groupid;
    if (gid == bgNONE) return;
    for (int i = 0; i < ppanel->btnCount; i++) {
        if (ppanel->pbuttons[i]->groupid == gid) {
            if (btnIdx == i)
                ppanel->pbuttons[i]->selected = 1;
            else
                ppanel->pbuttons[i]->selected = 0;
        }
    }
}

// ΪȺ�鰴ť����Ĭ��ѡ�а�ť
// ���հ�ť��ӵ�˳��ÿ��Ⱥ���һ�����ֵİ�ťΪĬ��ѡ��״̬
void initButtonGroup(TPanel* ppanel)
{
    int gid = bgNONE;
    for (int i = 0; i < ppanel->btnCount; i++) {
        if (ppanel->pbuttons[i]->groupid != bgNONE && ppanel->pbuttons[i]->groupid != gid ) {
            updateButtonGroup(ppanel, i);
            gid = ppanel->pbuttons[i]->groupid;
        }
    }
}

int ptInPanel(POINT p, TPanel* ppanel)
{
    int left = ppanel->x;
    int right = ppanel->x + ppanel->w;
    int top = ppanel->y;
    int bottom = ppanel->y + ppanel->h;
    if (p.x > left && p.x < right && p.y > top && p.y < bottom)
        return 1;
    return 0;
}