#pragma once

#include <easyx.h>

#define	WIDTH	640
#define	HEIGHT	480
#define	MAXSTEP	9

// 多边形顶点Vertex
// 顶点Vertex由一组点POINT构成
typedef struct tagVertex {
	POINT* m_points;		// 点数组，多个坐标
	int m_pcount;			// 点数组中点的数量
	POINT m_step;			// 移动步长
} TVertex;

// 多边形
typedef struct tagPolygon
{
	COLORREF m_color;	// 多边形的颜色
	TVertex* m_vertexs;	// 多边形顶点数组
	int m_vcount;		// 多边形顶点数量
} TPolygon;

// 初始化顶点Vertex
void vertexInit(TVertex* pvt, int linecount);
// 释放顶点Vertex，主要是释放m_points分配的内存
void vertexFree(TVertex* pvt);
// 获取第一个点坐标
POINT vertexGetHead(TVertex* pv);
// 获取最后一个点坐标
POINT vertexGetTail(TVertex* pvt);
// 移动点数组中每个点的位置，更新[0]的坐标，丢弃最后一个点[m_count-1]的坐标
void vertexMove(TVertex* pvt);

// 初始化多边形polygon
void polygonInit(TPolygon* ppl, int vertexcount, int linecount);
// 释放多边形
void polygonFree(TPolygon* ppl);
// 绘制并移动多边形
void polygonMove(TPolygon* ppl);