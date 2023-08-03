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

int main()
{
	THanoiStack hanoi;

	initgraph(800, 600);
	srand(time(NULL));
	initHanoiStack(&hanoi, 8, 50, getheight() - 150, getwidth() - 100, 20, 4, getheight() - 300);

	BeginBatchDraw();
	drawHanoiStack(&hanoi);
	_getch();

	solveHanoiStack(&hanoi, hanoi.layercount, &hanoi.stkA, &hanoi.stkB, &hanoi.stkC);
	EndBatchDraw();

	_getch();
	closegraph();
	return 0;
}

