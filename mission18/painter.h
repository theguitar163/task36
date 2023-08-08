#pragma once

#include <easyx.h>

#define btCIRCLE  0
#define btRECT    1
#define btRDRECT  2

#define MAX_BUTTON 100

#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

struct tagPainter;

// 函数指针类型
typedef void (TFunction)(struct tagPainter* ppainter);

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

// 简易控制面板
// 可在上面添加按钮（因为使用纯C，不太方便实现抽象类多态添加其他控件）
typedef struct tagPanel {
    int x;
    int y;
    int w;
    int h;
    TButton* pbuttons[MAX_BUTTON];
    int btnCount = 0;
    struct tagPainter* ppainter;
} TPanel;

#define dtFreehand 0
#define dtLine     1
#define dtRect     2
#define dtCircle   3
#define dtellipse  4

typedef struct tagPainter {
    int x;
    int y;
    int w;
    int h;
    int drawType = 0;
    COLORREF drawColor = BLACK;
    TPanel* ppanel;
} TPainter;

void initPainter(TPainter* ppainter, TPanel*ppanel, int panelsize, int panelalign);
void clearPainter(TPainter* ppainter);
void drawPainter(TPainter* ppainter);

void initButton(TButton* pbtn, int x, int y, int x2, int y2, COLORREF color, TCHAR* text, int mod);
void initButton(TButton* pbtn, int x1, int y1, int radius, int mod);
void initButton(TButton* pbtn, int x, int y, int w, int h, int mod);
void initButton(TButton* pbtn, int x, int y);
// 绘制按钮
void drawButton(TButton* pbtn);
// 判断点pt是否在按钮中
int ptInButton(POINT p, TButton* pbtn);
void initPanel(TPanel* ppanel, int x, int y, int w, int h);
void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void drawPanel(TPanel* ppanel);

void buttonClick(TPanel* ppanel, int x, int y);