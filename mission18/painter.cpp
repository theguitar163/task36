#include <easyx.h>
#include <graphics.h>
#include "painter.h"
#include "imageproc.h"

void setButtonPos(TButton* pbtn, int x, int y)
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
    setlinecolor(BLACK);
    settextcolor(BLACK);
    RECT r = { ox + pbtn->x, oy + pbtn->y, ox + pbtn->x + pbtn->w, oy + pbtn->y + pbtn->h };
    if (pbtn->shape == bsCIRCLE) {
        if (pbtn->focused) {
            setlinecolor(LIGHTBLUE);
            ellipse(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillellipse(r.left, r.top, r.right, r.bottom);
    }
    else if (pbtn->shape == bsRDRECT) {
        int esize = ((pbtn->w > pbtn->h) ? pbtn->h : pbtn->w) / 4;
        if (pbtn->focused) {
            setlinecolor(LIGHTBLUE);
            roundrect(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2, esize+2, esize+2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillroundrect(r.left, r.top, r.right, r.bottom, esize, esize);
    }
    else {
        if (pbtn->focused) {
            setlinecolor(LIGHTBLUE);
            rectangle(r.left - 2, r.top - 2, r.right + 2, r.bottom + 2);
        }
        setlinecolor(BLACK);
        setfillcolor(pbtn->color);
        fillrectangle(r.left, r.top, r.right, r.bottom);
    }
    // 绘制按钮标题
    if (pbtn->text != NULL) {
        TCHAR str[200];
        if (pbtn->type == btNUM)
            swprintf_s(str, L"%s[%d]", pbtn->text, pbtn->value);
        else if (pbtn->type == btBOOL)
            swprintf_s(str, L"%s[%s]", pbtn->text, (pbtn->value == 0) ? L"○" : L"●");
        else
            wcscpy_s(str, pbtn->text);
        settextstyle(12, 0, L"宋体");
        // 自动设置字体颜色，主要调整明度
        COLORREF c = pbtn->color;
        float h, s, l;
        RGBtoHSL(c, &h, &s, &l);
        settextcolor(HSLtoRGB(h, s, (l > 0.5) ? 0 : 1));
        drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    }
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

// 初始化控制板
// 根据对齐方式，自动设置控制板位置
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

// 添加按钮，不考虑按钮位置
void addButton(TPanel* ppanel, TButton* pbutton)
{
    if (ppanel->btnCount < MAX_BUTTON - 1) {
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

// 添加按钮到控制板中
// 根据指定的添加方式和间隔自动设置按钮的位置
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
        else { // 默认为右侧添加
            pbutton->x = p->x + p->w + spacing;
            pbutton->y = p->y;
        }
        ppanel->pbuttons[ppanel->btnCount] = pbutton;
        pbutton->container = ppanel;
        ppanel->btnCount++;
    }
}

// 绘制控制板及所包含的按钮
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

// 按钮点击事件分派处理
void buttonClick(TPanel* ppanel, int x, int y)
{
    for (int i = 0; i < ppanel->btnCount; i++) {
        if (ptInButton({ x, y }, ppanel->pbuttons[i])) {
            ppanel->btnFocused = i;
            TFunction* pfun = ppanel->pbuttons[i]->pfun;
            if (pfun!=NULL) 
                (*pfun)(ppanel->ppainter);

            drawPanel(ppanel);
            FlushBatchDraw();

            break;
        }
    }
}

// 群组按钮点击后，自动更新按钮群组其他按钮状态
void updateButtonGroup(TPanel* ppanel, int btnIdx)
{
    int gid = ppanel->pbuttons[btnIdx]->groupid;
    for (int i = 0; i < ppanel->btnCount; i++) {
        if (ppanel->pbuttons[i]->groupid == gid) {
            if (btnIdx == i)
                ppanel->pbuttons[i]->focused = 1;
            else
                ppanel->pbuttons[i]->focused = 0;
        }
    }
}

// 初始化绘图板
void initPainter(TPainter* ppainter, HWND hwnd, TPanel* ppanel, int panelsize, int panelalign)
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
    initPanel(ppanel, panelsize, panelalign);
    ppainter->hwnd = hwnd;
    ppainter->ppanel = ppanel;
    ppanel->ppainter = ppainter;
    clearPainter(ppainter);
}

// 清除绘图板
void clearPainter(TPainter* ppainter)
{
    setfillcolor(WHITE);
    solidrectangle(ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h);
    drawPanel(ppainter->ppanel);
}

// 备份当前图像
void backupPainter(TPainter* ppainter)
{
    getimage(&ppainter->imgBackup, ppainter->x, ppainter->y, ppainter->w, ppainter->h); 
}

// 绘制绘图板
void drawPainter(TPainter* ppainter)
{
    drawPanel(ppainter->ppanel);
}

// 判定点p是否位于绘图板画布中
// shrinksize主要用于PEN的直径大于1可能超出画布范围
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

// 画布事件分派
void painterClick(TPainter* ppainter, int startx, int starty)
{
    backupPainter(ppainter);
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

// 绘制线
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
                    setlinestyle(PS_SOLID, ppainter->penThickness);
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

// 绘制矩形
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
                setlinestyle(PS_SOLID, ppainter->penThickness);
                if (ppainter->isFill) {
                    setfillcolor(ppainter->fillColor);
                    fillrectangle(startx, starty, x, y);
                }
                else
                    rectangle(startx, starty, x, y);
                break;
            }
            FlushBatchDraw();
        }
    }
}

// 绘制椭圆
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
                setlinestyle(PS_SOLID, ppainter->penThickness);
                if (ppainter->isFill) {
                    setfillcolor(ppainter->fillColor);
                    fillellipse(startx, starty, x, y);
                }
                else
                    ellipse(startx, starty, x, y);
                break;
            }
            FlushBatchDraw();
        }
    }
    //   setlinestyle(PS_SOLID, m_size);
}

// 绘制橡皮擦
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

// 绘制马赛克
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