#include <easyx.h>
#include "hanoi.h"

// 栈相关函数 ---------------------------------------------

// 初始化栈
void stackInit(TStack* ps, int x, int y, int rw, int rh)
{
	ps->top = -1;				// 栈顶指针初始化为-1，栈为空
	ps->x = x;
	ps->y = y;
	ps->rw = rw;
	ps->rh = rh;
}

// 出栈
TRing stackPop(TStack* ps)
{
	TRing r = { 0 };
	if (ps->top >= 0) {
		r = ps->stack[ps->top];
		ps->top--;
	}
	return r;
}

// 入栈
void stackPush(TStack* ps, TRing r)
{
	if (ps->top < MAX_STACK - 1) {
		ps->top++;
		ps->stack[ps->top] = r;
	}
}

// 绘制栈（绘制所有圆环）
void drawStack(TStack* ps)
{
	for (int i = 0; i <= ps->top; i++) {
		setlinecolor(BLACK);
		setfillcolor(ps->stack[i].color);
		fillroundrect(
			ps->x - (ps->rw * ps->stack[i].size) / 2,
			ps->y - ps->rh * i - 5,
			ps->x + (ps->rw * ps->stack[i].size) / 2,
			ps->y - ps->rh * i + 5,
			ps->rw / 4,
			ps->rh / 4);
	}
}

// 汉诺塔相关函数 -----------------------------------------

// 初始化汉诺塔
void initHanoiStack(THanoiStack* phs, int count, int bx, int by, int bw, int bh, int pw, int ph)
{
	phs->layercount = count;
	phs->base_x = bx;
	phs->base_y = by;
	phs->base_w = bw;
	phs->base_h = bh;
	phs->pole_w = pw;
	phs->pole_h = ph;

	stackInit(&(phs->stkA), bx + bw / 3 * 0.5, by - 20, 20, 20);
	stackInit(&(phs->stkB), bx + bw / 3 * 1.5, by - 20, 20, 20);
	stackInit(&(phs->stkC), bx + bw / 3 * 2.5, by - 20, 20, 20);

	// 将圆环从大到小依次堆叠在stkA上
	for (int i = count; i > 0; i--) {
		TRing r = { i, RGB(rand() % 256, 128 + rand() % 128, rand() % 256) };
		stackPush(&(phs->stkA), r);
	};
}

// 绘制汉诺塔
void drawHanoiStack(THanoiStack* phs)
{
	setbkcolor(WHITE);
	cleardevice();

	// 绘制底座
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(phs->base_x, phs->base_y, phs->base_x + phs->base_w, phs->base_y + phs->base_h);

	// 绘制柱子
	int w = phs->base_w / 3;
	setfillcolor(DARKGRAY);
	fillrectangle(phs->base_x + 0.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 0.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);
	fillrectangle(phs->base_x + 1.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 1.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);
	fillrectangle(phs->base_x + 2.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 2.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);

	// 绘制堆叠的圆环
	drawStack(&(phs->stkA));
	drawStack(&(phs->stkB));
	drawStack(&(phs->stkC));
	FlushBatchDraw();
}

// 求解汉诺塔（采用递归调用）
// count: 当前层数
// psrc: 源栈
// pvia: 过渡栈
// pdst: 目标栈
void solveHanoiStack(THanoiStack* phs, int count, TStack* psrc, TStack* pvia, TStack* pdst)
{
	TRing rtmp;
	// 当仅剩一层时，直接从源栈移动到目标栈
	if (count == 1) {
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
		// 绘制汉诺塔
		drawHanoiStack(phs);
		Sleep(FRAME_RATE);
	}
	else {
		// 留下最底层圆环，将count-1以上的圆环从psrc，经由pdst，移动到pvia
		solveHanoiStack(phs, count - 1, psrc, pdst, pvia);

		// 将最底层圆环从psrc移动至pdst
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
		// 绘制汉诺塔
		drawHanoiStack(phs);
		Sleep(FRAME_RATE);

		// 在将移动到pvia的count-1以上的圆环，经由psrc，移动到pdst
		solveHanoiStack(phs, count - 1, pvia, psrc, pdst);
	}
}