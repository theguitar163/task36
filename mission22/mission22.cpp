// 任务：汉诺塔求解。
// 汉诺塔（Tower of Hanoi），一块黄铜板上插着三根宝石针。在其中一根针从下到上地穿好了由大到小的 64 片金片。
// 按照下面的规则移动这些金片：一次只移动一片，不管在哪根针上，小片必须在大片上面。
// 请将所有的金片都从穿好的那根针上移到另外一根针上。
// 请写一个程序，动态演示 n 层汉诺塔的求解过程。
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>

#define MAX_STACK 100
#define FRAME_RATE (1000/60)

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

void drawStack(TStack* ps)
{
	for (int i = 0; i <= ps->top; i++) {
		setlinecolor(BLACK);
		setfillcolor(ps->stack[i].color);
		fillroundrect(
			ps->x - (ps->rw * ps->stack[i].size) / 2,
			ps->y - ps->rh * (i + 1) - 5,
			ps->x + (ps->rw * ps->stack[i].size) / 2,
			ps->y - ps->rh * (i + 1) + 5,
			ps->rw / 4, ps->rh / 4);
	}
}

TRing stackPop(TStack* ps)
{
	TRing r = { 0 };
	if (ps->top >= 0) {
		r = ps->stack[ps->top];
		ps->top--;

		drawStack(ps);
		Sleep(FRAME_RATE);
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
	int base_x;
	int base_y;
	int base_w;
	int base_h;
	int pole_w;
	int pole_h;
} THanoiStack;

void initHanoiStack(THanoiStack* phs, int count, int bx, int by, int bw, int bh, int pw, int ph)
{
	phs->layercount = count;
	phs->base_x = bx;
	phs->base_y = by;
	phs->base_w = bw;
	phs->base_h = bh;
	phs->pole_w = pw;
	phs->pole_h = ph;

	stackInit(&(phs->stkA), bx + bw / 3 * 0.5, by, 20, 20);
	stackInit(&(phs->stkB), bx + bw / 3 * 1.5, by, 20, 20);
	stackInit(&(phs->stkC), bx + bw / 3 * 2.5, by, 20, 20);

	for (int i = count; i > 0; i--) {
		TRing r = { i, RGB(rand() % 256, 128 + rand() % 128, rand() % 256) };
		stackPush(&(phs->stkA), r);
	};
}

void drawHanoiStack(THanoiStack* phs)
{
	setbkcolor(WHITE);
	cleardevice();

	// 绘制底座
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(phs->base_x, phs->base_y, phs->base_x + phs->base_w, phs->base_y + phs->base_h);
	
	// 绘制宝石针
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

void solveHanoiStack(THanoiStack* phs, int count, TStack* psrc, TStack* pvia, TStack* pdst)
{
	TRing rtmp;
	if (count == 1) {
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);

		drawHanoiStack(phs);
		Sleep(FRAME_RATE);
	}
	else {
		solveHanoiStack(phs, count - 1, psrc, pdst, pvia);
		
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);

		drawHanoiStack(phs);
		Sleep(FRAME_RATE);

		solveHanoiStack(phs, count - 1, pvia, psrc, pdst);
	}
}

int main()
{
	THanoiStack hanoi;

	initgraph(800, 600);
	srand(time(NULL));
	initHanoiStack(&hanoi, 8, 50, getheight() - 150, getwidth() - 100, 20, 10, getheight() - 300);

	BeginBatchDraw();
	drawHanoiStack(&hanoi);

	solveHanoiStack(&hanoi, hanoi.layercount, &hanoi.stkA, &hanoi.stkB, &hanoi.stkC);
	EndBatchDraw();

	_getch();
	closegraph();
	return 0;
}

