#include <easyx.h>
#include "hanoi.h"

// ջ��غ��� ---------------------------------------------

// ��ʼ��ջ
void stackInit(TStack* ps, int x, int y, int rw, int rh)
{
	ps->top = -1;				// ջ��ָ���ʼ��Ϊ-1��ջΪ��
	ps->x = x;
	ps->y = y;
	ps->rw = rw;
	ps->rh = rh;
}

// ��ջ
TRing stackPop(TStack* ps)
{
	TRing r = { 0 };
	if (ps->top >= 0) {
		r = ps->stack[ps->top];
		ps->top--;
	}
	return r;
}

// ��ջ
void stackPush(TStack* ps, TRing r)
{
	if (ps->top < MAX_STACK - 1) {
		ps->top++;
		ps->stack[ps->top] = r;
	}
}

// ����ջ����������Բ����
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

// ��ŵ����غ��� -----------------------------------------

// ��ʼ����ŵ��
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

	// ��Բ���Ӵ�С���ζѵ���stkA��
	for (int i = count; i > 0; i--) {
		TRing r = { i, RGB(rand() % 256, 128 + rand() % 128, rand() % 256) };
		stackPush(&(phs->stkA), r);
	};
}

// ���ƺ�ŵ��
void drawHanoiStack(THanoiStack* phs)
{
	setbkcolor(WHITE);
	cleardevice();

	// ���Ƶ���
	setlinecolor(BLACK);
	setfillcolor(LIGHTGRAY);
	fillrectangle(phs->base_x, phs->base_y, phs->base_x + phs->base_w, phs->base_y + phs->base_h);

	// ��������
	int w = phs->base_w / 3;
	setfillcolor(DARKGRAY);
	fillrectangle(phs->base_x + 0.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 0.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);
	fillrectangle(phs->base_x + 1.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 1.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);
	fillrectangle(phs->base_x + 2.5 * w - phs->pole_w / 2, phs->base_y, phs->base_x + 2.5 * w + phs->pole_w / 2, phs->base_y - phs->pole_h);

	// ���ƶѵ���Բ��
	drawStack(&(phs->stkA));
	drawStack(&(phs->stkB));
	drawStack(&(phs->stkC));
	FlushBatchDraw();
}

// ��⺺ŵ�������õݹ���ã�
// count: ��ǰ����
// psrc: Դջ
// pvia: ����ջ
// pdst: Ŀ��ջ
void solveHanoiStack(THanoiStack* phs, int count, TStack* psrc, TStack* pvia, TStack* pdst)
{
	TRing rtmp;
	// ����ʣһ��ʱ��ֱ�Ӵ�Դջ�ƶ���Ŀ��ջ
	if (count == 1) {
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
		// ���ƺ�ŵ��
		drawHanoiStack(phs);
		Sleep(FRAME_RATE);
	}
	else {
		// ������ײ�Բ������count-1���ϵ�Բ����psrc������pdst���ƶ���pvia
		solveHanoiStack(phs, count - 1, psrc, pdst, pvia);

		// ����ײ�Բ����psrc�ƶ���pdst
		rtmp = stackPop(psrc);
		stackPush(pdst, rtmp);
		// ���ƺ�ŵ��
		drawHanoiStack(phs);
		Sleep(FRAME_RATE);

		// �ڽ��ƶ���pvia��count-1���ϵ�Բ��������psrc���ƶ���pdst
		solveHanoiStack(phs, count - 1, pvia, psrc, pdst);
	}
}