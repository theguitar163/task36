#pragma once

#include <easyx.h>

#define	WIDTH	640
#define	HEIGHT	480
#define	MAXSTEP	9

// ����ζ���Vertex
// ����Vertex��һ���POINT����
typedef struct tagVertex {
	POINT* m_points;		// �����飬�������
	int m_pcount;			// �������е������
	POINT m_step;			// �ƶ�����
} TVertex;

// �����
typedef struct tagPolygon
{
	COLORREF m_color;	// ����ε���ɫ
	TVertex* m_vertexs;	// ����ζ�������
	int m_vcount;		// ����ζ�������
} TPolygon;

// ��ʼ������Vertex
void vertexInit(TVertex* pvt, int linecount);
// �ͷŶ���Vertex����Ҫ���ͷ�m_points������ڴ�
void vertexFree(TVertex* pvt);
// ��ȡ��һ��������
POINT vertexGetHead(TVertex* pv);
// ��ȡ���һ��������
POINT vertexGetTail(TVertex* pvt);
// �ƶ���������ÿ�����λ�ã�����[0]�����꣬�������һ����[m_count-1]������
void vertexMove(TVertex* pvt);

// ��ʼ�������polygon
void polygonInit(TPolygon* ppl, int vertexcount, int linecount);
// �ͷŶ����
void polygonFree(TPolygon* ppl);
// ���Ʋ��ƶ������
void polygonMove(TPolygon* ppl);