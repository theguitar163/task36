#include <easyx.h>
#include <graphics.h>
#include "painter.h"

// ��ʼ����ͼ��
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

// �����ͼ��
void clearPainter(TPainter* ppainter)
{
    setfillcolor(WHITE);
    solidrectangle(ppainter->x, ppainter->y, ppainter->x + ppainter->w, ppainter->y + ppainter->h);
    drawPanel(ppainter->ppanel);
}

// ���ݵ�ǰͼ��
void backupPainter(TPainter* ppainter)
{
    getimage(&ppainter->imgBackup, ppainter->x, ppainter->y, ppainter->w, ppainter->h); 
}

// �ָ�����ͼ��
void restorePainter(TPainter* ppainter)
{
    // ͨ���ָ�����ͼ�β���ѡ����
    putimage(ppainter->x, ppainter->y, &ppainter->imgBackup);
}

// �ж���p�Ƿ�λ�ڻ�ͼ�廭����
// shrinksize��Ҫ����PEN��ֱ������1���ܳ���������Χ
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

// �趨����ѡ��״̬
void setSelectState(TPainter* ppainter, int state)
{
    ppainter->selectState = state;
}

// ������
void PaintLine(TPainter* ppainter, int startx, int starty)
{
    int x = startx;
    int y = starty;
    // ���ƻ���
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

// ���ƾ���
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

// ������Բ
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

// ������Ƥ��
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

// ����������
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

// ��������ѡ������
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

// �����¼�����
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

// ��ͼ������ѭ��
void Run(TPainter* ppainter)
{
    BeginBatchDraw();

    clearPainter(ppainter);
    ExMessage m;
    while (true) {
        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
            // ��������ж�
            if (m.message == WM_LBUTTONDOWN) {
                // λ�ڻ�ͼ��
                if (ptInPainter({ m.x, m.y }, ppainter)) {
                    // ��ǰΪѡ��״̬�������ѡ������
                    if (ppainter->selectState)
                        restorePainter(ppainter);
                    // ����Ϊ��ѡ��״̬���������������ͼ����
                    setSelectState(ppainter, 0);
                    // ����������ͼ����
                    painterClick(ppainter, m.x, m.y);
                }
                else {
                    // ��ǰΪѡ��״̬�������ѡ������
                    if (ppainter->selectState)
                        restorePainter(ppainter);
                    // �ӳ�ȡ��ѡ��״̬��ʹ�ð�ť�¼������ܹ���ȡ��ȷѡ��״̬������
                    buttonClick(ppainter->ppanel, m.x, m.y);
                    // ����Ϊ��ѡ��״̬
                    setSelectState(ppainter, 0);
                }
            }
        }
        FlushBatchDraw();
        Sleep(10);
    }
    EndBatchDraw();
}
