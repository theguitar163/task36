#pragma once
#include <easyx.h>

struct tagPainter;

// 点击按钮事件响应函数指针类型
typedef void (TFunction)(struct tagPainter* ppainter);

// 按钮形状
#define bsCIRCLE  0
#define bsRECT    1
#define bsRDRECT  2

// 按钮类型
#define btDEFAULT 0
#define btCOLOR   1
#define btNUM     2
#define btBOOL    3

#define bgNONE      0
#define bgPENCOLOR  1
#define bgFILLCOLOR 2
#define bgPENTYPE   3

// 按钮结构
typedef struct tagButton {
    int shape;                  // 按钮形状bsCIRCLE/bsRDRECT/bsRECT
    int type;                   // 按钮类型btDEFAULT/btCOLOR/btNUM/btBOOL
    const TCHAR* text;          // 按钮上的文字
    COLORREF color;             // 按钮的颜色
    int w, h;                   // 按钮宽高
    TFunction* pOnClick = NULL; // 点击按钮事件关联函数指针
    int groupid = bgNONE;       // 群组id
    LONG tag = 0;               // 特殊类型值可兼容颜色、整数、布尔数，由type决定其含义
    int x, y;                   // 按钮的坐标
    int selected = 0;           // 选中状态
    struct tagPanel* container; // 控制板容器
} TButton;

// 设置按钮位置
void setButtonPos(TButton* pbtn, int x, int y);
// 读取按钮颜色
COLORREF getButtonColor(TButton* pbtn);
void setButtonColor(TButton* pbtn, COLORREF c);
// 设置按钮Tag的值
void setButtonTag(TButton* pbtn, LONG tag);
LONG getButtonTag(TButton* pbtn);
// 绘制按钮
void drawButton(TButton* pbtn);
// 判断点pt是否在按钮中
int ptInButton(POINT p, TButton* pbtn);

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
    int btnCount = 0;       // 按钮数量
    int btnClicked = -1;    // 最近被点击按钮的下标
    struct tagPainter* ppainter;
} TPanel;

// 再控制板上添加按钮的方向
#define adRIGHT   0    // 右侧添加
#define adBOTTOM  1    // 下方添加
#define adNEWLINE 2    // 新行添加

void initPanel(TPanel* ppanel, int size, int align);
void addButton(TPanel* ppanel, TButton* pbutton);
void addButton(TPanel* ppanel, TButton* pbutton, int spacing, int dir = adRIGHT);
void drawPanel(TPanel* ppanel);
void updateButtonGroup(TPanel* ppanel, int btnIdx);
void initButtonGroup(TPanel* ppanel);
void buttonClick(TPanel* ppanel, int x, int y);