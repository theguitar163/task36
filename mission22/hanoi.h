#pragma once

#include <easyx.h>

#define MAX_STACK 100
#define FRAME_RATE (1000/60)

typedef struct tagRing {
	int size;					// Բ����С
	COLORREF color;				// Բ����ɫ
} TRing;

typedef struct tagStack {
	int top;					// ջ��ָ��
	TRing stack[MAX_STACK];		// ջ����
	// ����Ϊ����ͼ����Ҫ
	int x;						// ջ�����ĵ�x����
	int y;						// ջ�����ĵ�y����
	int rw;						// Բ����λ��ȣ�ʵ�ʿ��Ϊrw * ring.size
	int rh;						// Բ����λ�߶�
} TStack;

// ��ŵ���ṹ
typedef struct tagHanoiStack {
	int layercount;				// ��ŵ��Բ������
	TStack stkA;
	TStack stkB;
	TStack stkC;
	// ����Ϊ����ͼ����Ҫ
	int base_x;					// ����x����
	int base_y;					// ����y����
	int base_w;					// �������
	int base_h;					// �����߶�
	int pole_w;					// ���ӿ��
	int pole_h;					// ���Ӹ߶�
} THanoiStack;

// ��ʼ����ŵ��
void initHanoiStack(THanoiStack* phs, int count, int bx, int by, int bw, int bh, int pw, int ph);
// ���ƺ�ŵ��
void drawHanoiStack(THanoiStack* phs);
// ��⺺ŵ�������õݹ���ã�
void solveHanoiStack(THanoiStack* phs, int count, TStack* psrc, TStack* pvia, TStack* pdst);