#include <easyx.h>
#include <graphics.h>
#include "painter.h"
#include "imageproc.h"

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int type)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->x2 = x2;
    pbtn->y2 = y2;
    pbtn->w = x2 - x;
    pbtn->h = y2 - y;
    pbtn->color = color;
    pbtn->text = text;
    pbtn->type = type;
}

void initButton(TButton* pbtn, int x1, int y1, int radius, int type)
{
    pbtn->x = x1;
    pbtn->y = y1;
    pbtn->radius = radius;
    pbtn->type = type;
}

void initButton(TButton* pbtn, int x, int y, int w, int h, int type)
{
    pbtn->x = x;
    pbtn->y = y;
    pbtn->w = w;
    pbtn->h = h;
    pbtn->type = type;
    pbtn->text = NULL;
}

void initButton(TButton* pbtn, int x, int y)
{
    pbtn->x = x;
    pbtn->y = y;
}

// 绘制按钮
void drawButton(TButton* pbtn)
{
    int ox, oy;
    // 获取容器原点坐标，无容器则为窗口原点
    if (pbtn->container == NULL) {
        ox = 0;
        oy = 0;
    }
    else {
        ox = pbtn->container->x;
        oy = pbtn->container->y;
    }

    // 绘制按钮，包括椭圆形、圆角矩形、矩形三种
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
    // 绘制按钮标题
    if (pbtn->text != NULL)
        drawtext(pbtn->text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
}

// 判断点pt是否在按钮中
int ptInButton(POINT p, TButton* pbtn)
{
    int bx, by;
    // 获取容器原点坐标，无容器则为窗口原点
    if (pbtn->container == NULL) {
        bx = pbtn->x;
        by = pbtn->y;
    }
    else {
        bx = pbtn->container->x + pbtn->x;
        by = pbtn->container->y + pbtn->y;
    }
    // 椭圆按钮，计算电是否在椭圆内
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
    setlinecolor(BLACK);
    setfillcolor(WHITE);
    fillrectangle(ppanel->x, ppanel->y, ppanel->x + ppanel->w, ppanel->y + ppanel->h);
    for (int i = 0; i < ppanel->btnCount; i++) {
        drawButton(ppanel->pbuttons[i]);
    }
}

void buttonClick(TPanel* ppanel, int x, int y)
{
    for (int i = 0; i < ppanel->btnCount; i++) {
        if (ptInButton({ x, y }, ppanel->pbuttons[i])) {
            ppanel->btnFocused = i;
            TFunction* pfun = ppanel->pbuttons[i]->pfun;
            if (pfun!=NULL) (*pfun)(ppanel->ppainter);
            break;
        }
    }
}

void initPainter(TPainter* ppainter, TPanel* ppanel, int panelsize, int panelalign)
{
    switch (panelalign) {
    case alTOP:
        ppainter->x = 0;
        ppainter->y = panelsize;
        ppainter->w = getwidth();
        ppainter->h = getheight() - panelsize;
        break;
    case alBOTTOM:
        ppainter->x = 0;
        ppainter->y = 0;
        ppainter->w = getwidth();
        ppainter->h = getheight() - panelsize;
        break;
    case alLEFT:
        ppainter->x = 0;
        ppainter->y = 0;
        ppainter->w = panelsize;
        ppainter->h = getheight();
        break;
    case alRIGHT:
        ppainter->x = 0;
        ppainter->y = 0;
        ppainter->w = getwidth() - panelsize;
        ppainter->h = getheight();
    }
    clearPainter(ppainter);
    initPanel(ppanel, panelsize, panelalign);
    ppainter->ppanel = ppanel;
    ppanel->ppainter = ppainter;
}

void clearPainter(TPainter* ppainter)
{
    setfillcolor(WHITE);
    solidrectangle(ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h);
}

void drawPainter(TPainter* ppainter)
{
    drawPanel(ppainter->ppanel);
}

int ptInPainter(POINT p, TPainter* ppainter, int shrinksize)
{
    int left = ppainter->x + shrinksize;
    int right = ppainter->x + ppainter->w - shrinksize;
    int top = ppainter->y + shrinksize;
    int bottom = ppainter->y + ppainter->h - shrinksize;
    if (p.x > left && p.x < right && p.y > top && p.y < bottom)
        return 1;
    return 0;
}

void painterClick(TPainter* ppainter, int startx, int starty)
{
    if (ppainter->penType == ptLINE) {
        PaintLine(ppainter, startx, starty);
    }
    else if (ppainter->penType == ptRECT) {
        PaintRect(ppainter, startx, starty);
    }
    else if (ppainter->penType == ptELLIPSE) {
        PaintEllipse(ppainter, startx, starty);
    }
    else if (ppainter->penType == ptERASER) {
        PaintEraser(ppainter, startx, starty);
    }
    else if (ppainter->penType == ptMOSAIC) {
        PaintMosaic(ppainter, startx, starty);
    }
}

void PaintLine(TPainter* ppainter, int startx, int starty)
{
    int x = startx;
    int y = starty;
    // 控制画线
    setlinecolor(ppainter->penColor);
    if (ptInPainter({ x, y }, ppainter)) {
        ExMessage m;
        while (true) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_MOUSEMOVE) {
                if (ptInPainter({ m.x,m.y }, ppainter)) {
                    line(x, y, m.x, m.y);
                    x = m.x;
                    y = m.y;
                }
            }
            else if (m.message == WM_LBUTTONUP) {
                break;
            }
            FlushBatchDraw();
        }
    }
}

void PaintRect(TPainter* ppainter, int startx, int starty)
{
    setlinestyle(PS_SOLID, 1);
    if (ptInPainter({ startx, starty }, ppainter)) {
        ExMessage m;
        int x = startx;
        int y = starty;
        setlinecolor(WHITE);
        setrop2(R2_XORPEN);
        rectangle(startx, starty, x, y);
        while (true) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_MOUSEMOVE) {
                if (ptInPainter({ m.x, m.y }, ppainter)) {
                    rectangle(startx, starty, x, y);
                    x = m.x;
                    y = m.y;
                    rectangle(startx, starty, x, y);
                }
            }
            else if (m.message == WM_LBUTTONUP) {
                setlinecolor(ppainter->penColor);
                setrop2(R2_COPYPEN);
                rectangle(startx, starty, x, y);
                break;
            }
            FlushBatchDraw();
        }
    }
    //   setlinestyle(PS_SOLID, m_size);
}

void PaintEllipse(TPainter* ppainter, int startx, int starty)
{
    setlinestyle(PS_SOLID, 1);
    if (ptInPainter({ startx, starty }, ppainter)) {
        ExMessage m;
        int x = startx;
        int y = starty;
        setlinecolor(WHITE);
        setrop2(R2_XORPEN);
        rectangle(startx, starty, x, y);
        while (true) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_MOUSEMOVE) {
                if (ptInPainter({ m.x, m.y }, ppainter)) {
                    ellipse(startx, starty, x, y);
                    x = m.x;
                    y = m.y;
                    ellipse(startx, starty, x, y);
                }
            }
            else if (m.message == WM_LBUTTONUP) {
                setlinecolor(ppainter->penColor);
                setrop2(R2_COPYPEN);
                ellipse(startx, starty, x, y);
                break;
            }
            FlushBatchDraw();
        }
    }
    //   setlinestyle(PS_SOLID, m_size);
}

void PaintEraser(TPainter* ppainter, int startx, int starty)
{
    int r = 20;
    if (ptInPainter({ startx, starty}, ppainter, r)) {
        ExMessage m;
        int x = startx;
        int y = starty;
        setfillcolor(WHITE);
        while (true) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_MOUSEMOVE) {
                if (ptInPainter({ m.x, m.y }, ppainter, r)) {
                    solidcircle(m.x, m.y, r);
                }
            }
            else if (m.message == WM_LBUTTONUP) {
                break;
            }
            FlushBatchDraw();
        }
    }
}

void PaintMosaic(TPainter* ppainter, int startx, int starty)
{
    int size = 12;
    if (ptInPainter({ startx, starty }, ppainter, size)) {
        ExMessage m;
        int x = startx;
        int y = starty;
        setfillcolor(WHITE);
        while (true) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_MOUSEMOVE) {
                if (ptInPainter({ m.x, m.y }, ppainter, size)) {
                    for (int i = -size; i < size; i = i + size / 3) {
                        for (int j = -size; j < size; j = j + size / 3) {
                            setfillcolor(getpixel(m.x + i + rand() % (size/3), m.y + j + rand() % (size/3)));
                            solidrectangle(m.x + i, m.y + j, m.x + i + (size/3), m.y + j + (size/3));
                        }
                    }
                }
            }
            else if (m.message == WM_LBUTTONUP) {
                break;
            }
            FlushBatchDraw();
        }
    }
}