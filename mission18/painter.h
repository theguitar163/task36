#pragma once

#include "panel.h"

// ��������
#define ptLINE     1
#define ptRECT     2
#define ptCIRCLE   3
#define ptELLIPSE  4
#define ptERASER   5
#define ptMOSAIC   6
#define ptSELECT   7

// ����ṹ
typedef struct tagPainter {
    HWND hwnd;  // ���ھ��
    int x;      // ������Χ
    int y;
    int w;
    int h;
    int startx, starty;         // ������ʼλ��
    int penType = ptLINE;       // ��������
    int penThickness = 1;       // �����߿�
    COLORREF penColor = BLACK;  // ������ɫ
    int isFill = 0;             // �Ƿ����
    COLORREF fillColor = WHITE; // �����ɫ
    IMAGE imgBackup;    // ����ͼ�񱸷�
    TPanel* ppanel;     // ��ť���ư�
    RECT selectRect = {0};  // ѡ�����
    int selectState = 0;
} TPainter;

void initPainter(TPainter* ppainter, HWND hwnd, TPanel*ppanel, int panelsize, int panelalign);
void clearPainter(TPainter* ppainter);
void clearSelectRect(TPainter* ppainter);
void backupPainter(TPainter* ppainter);
int ptInPainter(POINT p, TPainter* ppainter, int shrinksize=0);

void PaintLine(TPainter* ppainter, int startx, int starty);
void PaintRect(TPainter* ppainter, int startx, int starty);
void PaintEllipse(TPainter* ppainter, int startx, int starty);
void PaintEraser(TPainter* ppainter, int startx, int starty);
void PaintMosaic(TPainter* ppainter, int startx, int starty);
void PaintSelectRect(TPainter* ppainter, int startx, int starty);

void painterClick(TPainter* ppainter, int startx, int starty);
void Run(TPainter* ppainter);
