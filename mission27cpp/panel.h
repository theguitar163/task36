#pragma once
#include <easyx.h>

class CPanel;

// 点击按钮事件响应函数指针类型
typedef void (TFunction)();

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
#define bgFORMAT    1

// 按钮结构
class CButton
{
public:
    int m_shape;                  // 按钮形状bsCIRCLE/bsRDRECT/bsRECT
    int m_type;                   // 按钮类型btDEFAULT/btCOLOR/btNUM/btBOOL
    const TCHAR* m_text;          // 按钮上的文字
    COLORREF m_color;             // 按钮的颜色
    int m_w, m_h;                   // 按钮宽高
    TFunction* pOnClick = NULL; // 点击按钮事件关联函数指针
    int m_groupid = bgNONE;       // 群组id
    LONG m_tag = 0;               // 特殊类型值可兼容颜色、整数、布尔数，由type决定其含义
    int m_x, m_y;                   // 按钮的坐标
    int m_selected = 0;           // 选中状态
    CPanel* m_container; // 控制板容器

    // 设置按钮位置
    void setButtonPos(int x, int y);
    // 读取按钮颜色
    COLORREF getButtonColor();
    void setButtonColor(COLORREF c);
    // 设置按钮Tag的值
    void setButtonTag(LONG tag);
    LONG getButtonTag();
    // 绘制按钮
    void drawButton();
    // 判断点pt是否在按钮中
    int ptInButton(POINT p);
};


// 最大按钮数量
#define MAX_BUTTON 100

// 控制面板对齐方式
#define alTOP    0
#define alBOTTOM 1
#define alLEFT   2
#define alRIGHT  3

// 再控制板上添加按钮的方向
#define adRIGHT   0    // 右侧添加
#define adBOTTOM  1    // 下方添加
#define adNEWLINE 2    // 新行添加

// 简易控制面板
// 可在上面添加按钮（因为使用纯C，不太方便实现抽象类多态添加其他控件）
class CPanel
{
public:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
    CButton* m_pbuttons[MAX_BUTTON];
    int m_btnCount = 0;       // 按钮数量
    int m_btnClicked = -1;    // 最近被点击按钮的下标
 //   struct tagPainter* ppainter;

    void initPanel(int size, int align);
    void addButton(CButton* pbutton);
    void addButton(CButton* pbutton, int spacing, int dir = adRIGHT);
    void drawPanel();
    void updateButtonGroup(int btnIdx);
    void initButtonGroup();
    void buttonClick(int x, int y);
    int ptInPanel(POINT p);
};


