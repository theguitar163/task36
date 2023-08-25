#pragma once

#include <easyx.h>

#define MAX_STACK 100
#define FRAME_RATE (1000/60)

typedef struct tagRing {
	int size;					// 圆环大小
	COLORREF color;				// 圆环颜色
} TRing;

typedef struct tagStack {
	int top;					// 栈顶指针
	TRing stack[MAX_STACK];		// 栈数组
	// 以下为绘制图形需要
	int x;						// 栈底中心点x坐标
	int y;						// 栈底中心点y坐标
	int rw;						// 圆环单位宽度，实际宽度为rw * ring.size
	int rh;						// 圆环单位高度
} TStack;

// 汉诺塔结构
typedef struct tagHanoiStack {
	int layercount;				// 汉诺塔圆环层数
	TStack stkA;
	TStack stkB;
	TStack stkC;
	// 以下为绘制图形需要
	int base_x;					// 底座x坐标
	int base_y;					// 底座y坐标
	int base_w;					// 底座宽度
	int base_h;					// 底座高度
	int pole_w;					// 柱子宽度
	int pole_h;					// 柱子高度
} THanoiStack;

// 初始化汉诺塔
void initHanoiStack(THanoiStack* phs, int count, int bx, int by, int bw, int bh, int pw, int ph);
// 绘制汉诺塔
void drawHanoiStack(THanoiStack* phs);
// 求解汉诺塔（采用递归调用）
void solveHanoiStack(THanoiStack* phs, int count, TStack* psrc, TStack* pvia, TStack* pdst);