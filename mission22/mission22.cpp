// 任务：汉诺塔求解。
// 汉诺塔（Tower of Hanoi），一块黄铜板上插着三根宝石针。在其中一根针从下到上地穿好了由大到小的 64 片金片。
// 按照下面的规则移动这些金片：一次只移动一片，不管在哪根针上，小片必须在大片上面。
// 请将所有的金片都从穿好的那根针上移到另外一根针上。
// 请写一个程序，动态演示 n 层汉诺塔的求解过程。
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>

#define MAX_STACK 100

typedef struct tagRing {
	int size;
	COLORREF color;
} TRing;

typedef struct tagStack {
	int top;
	TRing stack[MAX_STACK];
	int x;
	int y;
	int rw;
	int rh;
} TStack;

void stackInit(TStack* ps, int x, int y, int rw, int rh)
{
	ps->top = -1;
	ps->x = x;
	ps->y = y;
	ps->rw = rw;
	ps->rh = rh;
}

TRing stackPop(TStack* ps)
{
	TRing r = { 0 };
	if (ps->top >= 0) {
		r = ps->stack[ps->top];
		ps->top--;
	}
	return r;
}

void stackPush(TStack* ps, TRing r)
{
	if (ps->top < MAX_STACK - 1) {
		ps->top++;
		ps->stack[ps->top] = r;
	}
}

typedef struct tagHanoiStack {
	int layercount;
	TStack stkA;
	TStack stkB;
	TStack stkC;
	int board_x;
	int board_y;
	int board_w;
	int board_h;
	int pole_w;
	int pole_h;
} THanoiStack;


void initHanoiStack(THanoiStack* phs, int count)
{

	stackInit(&(phs->stkA));
	for (int i = count; i > 1; i--) {
		TRing r = { i, RGB(rand() % 256, 128 + rand() % 128, rand() % 256) };
		stackPush(ps, r);
	};
}

void drawStack(TStack* ps, int x, int y, int w, int h)
{
	for (int i = 0; i <= ps->top; i++) {
		setlinecolor(BLACK);
		setfillcolor(ps->stack[i].color);
		fillroundrect(x - (w * ps->stack[i].size) / 2,
			y - h * i - 5,
			x + (w * ps->stack[i].size) / 2,
			y - h * i + 5,
			5, 5);
	}
}

void drawHanoiStack(TStack* psa, TStack* psb, TStack* psc, int count)
{
	int x = 50;
	int y = getheight() - 50;
	setbkcolor(WHITE);
	cleardevice();

	// 绘制底座
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(x, y, getwidth() - x, y + 20);

	// 绘制宝石针
	int w = (getwidth() - x * 2) / 3;
	setfillcolor(DARKGRAY);
	fillrectangle(x + 0.5 * w-2, y, x + 0.5 * w+2, 50);
	fillrectangle(x + 1.5 * w-2, y, x + 1.5 * w+2, 50);
	fillrectangle(x + 2.5 * w-2, y, x + 2.5 * w+2, 50);

	// 绘制圆圈
	int h = (getheight() - 50 * 2) / (count+1);
	drawStack(psa, x + 0.5 * w, y - 20, 20, 20);
	drawStack(psb, x + 1.5 * w, y - 20, 20, 20);
	drawStack(psc, x + 2.5 * w, y - 20, 20, 20);
}

void move(int count, TStack* psrc, TStack* pvia, TStack* pdst)
{
	TRing rtmp;
	if (count == 1) {
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
	}
	else {
		move(count - 1, psrc, pdst, pvia);
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
		move(count - 1, pvia, psrc, pdst);
	}
}

TStack stkA, stkB, stkC;

int main()
{
	initgraph(800, 600);
	initHanoiStack(&stkA, 8);

	drawHanoiStack(&stkA, &stkB, &stkC, 8);
	drawHanoiStack(&stkA, &stkB, &stkC, 8);

	_getch();
	closegraph();
	return 0;
}

