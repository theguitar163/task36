#pragma once

#include "panel.h"

// 画笔类型
#define ptLINE     1
#define ptRECT     2
#define ptCIRCLE   3
#define ptELLIPSE  4
#define ptERASER   5
#define ptMOSAIC   6
#define ptSELECT   7

// 画板结构
typedef struct tagPainter {
    HWND hwnd;  // 窗口句柄
    int x;      // 画布范围
    int y;
    int w;
    int h;
    int startx, starty;         // 画笔起始位置
    int penType = ptLINE;       // 画笔类型
    int penThickness = 1;       // 画笔线宽
    COLORREF penColor = BLACK;  // 画笔颜色
    int isFill = 0;             // 是否填充
    COLORREF fillColor = WHITE; // 填充颜色
    IMAGE imgBackup;    // 画布图像备份
    TPanel* ppanel;     // 按钮控制板
    RECT selectRect = {0};  // 选择矩形
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
