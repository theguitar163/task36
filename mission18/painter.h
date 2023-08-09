#pragma once

#include <easyx.h>


struct tagPainter;

// 函数指针类型
typedef void (TFunction)(struct tagPainter* ppainter);

// 按钮类型
#define btCIRCLE  0
#define btRECT    1
#define btRDRECT  2

// 按钮结构
typedef struct tagButton {
    int type;                 // 按钮类型btCIRCLE/btRDRECT/btRECT
    const TCHAR* text;        // 按钮上的文字
    COLORREF color;           // 按钮的颜色
    int w, h;
    TFunction* pfun = NULL;
    int x, y, x2, y2;         // 按钮的坐标
    int radius;               // 圆形按钮的半径    
    struct tagPanel* container;
} TButton;

// 最大按钮数量
#define MAX_BUTTON 100

// 控制面板对齐方式
#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

// 简易控制面板
// 可在上面添加按钮（因为使用纯C，不太方便实现抽象类多态添加其他控件）
typedef struct tagPanel {
    int x;
    int y;
    int w;
    int h;
    TButton* pbuttons[MAX_BUTTON];
    int btnCount = 0;
    int btnFocused = -1;
    struct tagPainter* ppainter;
} TPanel;

// 画笔类型
#define ptFREEHAND 0
#define ptLINE     1
#define ptRECT     2
#define ptCIRCLE   3
#define ptELLIPSE  4
#define ptERASER   5
#define ptMOSAIC   6

// 画板结构
typedef struct tagPainter {
    int x;  // 画布范围
    int y;
    int w;
    int h;
    int startx, starty;         // 画笔起始位置
    int penType = ptLINE;       // 画笔类型
    int penThickness = 1;       // 画笔线宽
    COLORREF penColor = BLACK;  // 画笔颜色
    COLORREF fillColor = WHITE; // 填充颜色
    IMAGE imgBackup;    // 画布图像备份
    TPanel* ppanel;     // 按钮控制板
} TPainter;

void initPainter(TPainter* ppainter, TPanel*ppanel, int panelsize, int panelalign);
void clearPainter(TPainter* ppainter);
void drawPainter(TPainter* ppainter);
int ptInPainter(POINT p, TPainter* ppainter, int shrinksize=0);
void painterClick(TPainter* ppainter, int startx, int starty);

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int mod);
void initButton(TButton* pbtn, int x1, int y1, int radius, int mod);
void initButton(TButton* pbtn, int x, int y, int w, int h, int mod);
void initButton(TButton* pbtn, int x, int y);
// 绘制按钮
void drawButton(TButton* pbtn);
// 判断点pt是否在按钮中
int ptInButton(POINT p, TButton* pbtn);

void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void drawPanel(TPanel* ppanel);

void buttonClick(TPanel* ppanel, int x, int y);

void PaintLine(TPainter* ppainter, int startx, int starty);
void PaintRect(TPainter* ppainter, int startx, int starty);
void PaintEllipse(TPainter* ppainter, int startx, int starty);
void PaintEraser(TPainter* ppainter, int startx, int starty);
void PaintMosaic(TPainter* ppainter, int startx, int starty);


