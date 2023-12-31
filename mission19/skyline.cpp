#include <easyx.h>
#include <graphics.h>
#include "skyline.h"

// 初始化顶点Vertex
void vertexInit(TVertex* pvt, int linecount)
{
	// 初始化前进方向
	pvt->m_step.x = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);
	pvt->m_step.y = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);

	// 分配点数组内存
	pvt->m_points = (POINT*)malloc(linecount * sizeof(POINT));
	pvt->m_pcount = linecount;

	// 随机生成顶点第一个点的坐标
	POINT* pp = &(pvt->m_points[0]);
	pp->x = rand() % WIDTH;
	pp->y = rand() % HEIGHT;
	// 根据第一个点的坐标，计算其他点的坐标
	for (int i = 1; i < linecount; i++) {
		POINT* pp0 = &(pvt->m_points[i - 1]);		// 取前一个点的指针
		pp = &(pvt->m_points[i]);					// 取当前点的指针
		// 以前一个点坐标为基准，按移动步长计算当前点的坐标
		// 注意：使用减步长，保证每个点不会超界
		pp->x = pp0->x - pvt->m_step.x;
		pp->y = pp0->y - pvt->m_step.y;
	}
}

// 释放顶点Vertex，主要是释放m_points分配的内存
void vertexFree(TVertex* pvt)
{
	if (pvt->m_points != NULL)
		free(pvt->m_points);
}

// 获取第一个点坐标
POINT vertexGetHead(TVertex* pv)
{
	return pv->m_points[0];
}

// 获取最后一个点坐标
POINT vertexGetTail(TVertex* pvt)
{
	return pvt->m_points[pvt->m_pcount - 1];
}

// 移动点数组中每个点的位置，更新[0]的坐标，丢弃最后一个点[m_count-1]的坐标
void vertexMove(TVertex* pvt)
{
	// 移动点数组[0,1,2,3,...n-1] => [0,0,1,2,...n-2]，丢弃最后一个点
	for (int i = pvt->m_pcount - 1; i > 0; i--) {
		pvt->m_points[i] = pvt->m_points[i - 1];
	}

	// 依据[0]点坐标，计算[0]点新位置
	POINT p;
	p.x = pvt->m_points[0].x + pvt->m_step.x;
	p.y = pvt->m_points[0].y + pvt->m_step.y;
	// 判断p点是否越界
	if (p.x < 0) {
		p.x = -p.x;
		pvt->m_step.x = rand() % MAXSTEP + 1;
	}
	else if (p.x >= WIDTH) {
		p.x -= p.x - WIDTH + 1;
		pvt->m_step.x = -rand() % MAXSTEP - 1;
	}

	if (p.y < 0) {
		p.y = -p.y;
		pvt->m_step.y = rand() % MAXSTEP + 1;
	}
	else if (p.y >= HEIGHT) {
		p.y -= p.y - HEIGHT + 1;
		pvt->m_step.y = -rand() % MAXSTEP - 1;
	}
	// 更新[0]点坐标
	pvt->m_points[0] = p;
}

// 初始化多边形polygon
// vertexcount: 顶点数量
// linecount: 线的数量（顶点所含点的数量）
void polygonInit(TPolygon* ppl, int vertexcount, int linecount)
{
	// 初始化颜色
	ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);

	// 为每个顶点分配内存
	ppl->m_vcount = vertexcount;
	ppl->m_vertexs = (TVertex*)malloc(ppl->m_vcount * sizeof(TVertex));

	// 初始化每个顶点
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexInit(&(ppl->m_vertexs[i]), linecount);
}

// 释放多边形
// 注意：需要释放每个顶点（每个顶点内含有为点数组申请的内存）
void polygonFree(TPolygon* ppl)
{
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexFree(&(ppl->m_vertexs[i]));
	free(ppl->m_vertexs);
}

// 绘制并移动多边形
void polygonMove(TPolygon* ppl)
{
	POINT p;
	// 1.擦除顶点所含点数组中最后一个点[m_count-1]包围的多边形
	setcolor(BLACK);
	p = vertexGetTail(&(ppl->m_vertexs[ppl->m_vcount - 1]));
	moveto(p.x, p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetTail(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 2.移动顶点所含点数组的位置+1，丢弃最后一个点[m_count-1]，更新第一个点[0]坐标
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexMove(&(ppl->m_vertexs[i]));

	// 3.绘制顶点所含点数组中第一个点[0]包围的多边形（其他位置顶点不变）
	setcolor(ppl->m_color);
	p = vertexGetHead(&(ppl->m_vertexs[ppl->m_vcount - 1]));
	moveto(p.x, p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetHead(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 4.有 1% 的概率更换颜色
	if (rand() % 100 == 0)
		ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);
}