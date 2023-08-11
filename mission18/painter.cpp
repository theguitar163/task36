#include <easyx.h>
#include <graphics.h>
#include "painter.h"

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

// 恢复备份图像
void restorePainter(TPainter* ppainter)
{
    // 通过恢复备份图形擦除选择线
    putimage(ppainter->x, ppainter->y, &ppainter->imgBackup);
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

// 设定区域选则状态
void setSelectState(TPainter* ppainter, int state)
{
    ppainter->selectState = state;
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

// 绘制区域选择虚线
void PaintSelectRect(TPainter* ppainter, int startx, int starty)
{
    setlinestyle(PS_DASH, 1);
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
                rectangle(startx, starty, x, y);
                rectangle(startx, starty, x, y);
                ppainter->selectRect = { startx, starty, x, y };
                ppainter->selectState = 1;
                setlinecolor(ppainter->penColor);
                setrop2(R2_COPYPEN);
                setlinestyle(PS_SOLID, ppainter->penThickness);
                break;
            }
            FlushBatchDraw();
        }
    }
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
    else if (ppainter->penType == ptSELECT) {
        PaintSelectRect(ppainter, startx, starty);
    }
}

// 绘图程序主循环
void Run(TPainter* ppainter)
{
    BeginBatchDraw();

    clearPainter(ppainter);
    ExMessage m;
    while (true) {
        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
            // 左键单击判断
            if (m.message == WM_LBUTTONDOWN) {
                // 位于绘图区
                if (ptInPainter({ m.x, m.y }, ppainter)) {
                    // 当前为选区状态，先清除选区虚线
                    if (ppainter->selectState)
                        restorePainter(ppainter);
                    // 设置为非选区状态，避免干扰其他绘图操作
                    setSelectState(ppainter, 0);
                    // 进行正常绘图操作
                    painterClick(ppainter, m.x, m.y);
                }
                else {
                    // 当前为选区状态，先清除选区虚线
                    if (ppainter->selectState)
                        restorePainter(ppainter);
                    // 延迟取消选区状态，使得按钮事件函数能够获取正确选区状态和区域
                    buttonClick(ppainter->ppanel, m.x, m.y);
                    // 设置为非选区状态
                    setSelectState(ppainter, 0);
                }
            }
        }
        FlushBatchDraw();
        Sleep(10);
    }
    EndBatchDraw();
}
