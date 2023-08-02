// 任务：模拟 Windows XP 的屏保“变幻线”。

#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#define	WIDTH	640
#define	HEIGHT	480
#define	MAXSTEP	9

// 多边形顶点Vertex
// 顶点Vertex由一组点POINT构成
typedef struct tagVertex {
	POINT* m_points;		// 点数组，多个坐标
	int m_pcount;			// 点数组的长度
	POINT m_step;			// 移动步长
} TVertex;

// 析构函数
void vertexFree(TVertex* pvt)
{
	if (pvt->m_points != NULL)
		free(pvt->m_points);
}

// 初始化环链
void vertexInit(TVertex* pvt, int linecount)
{
	// 初始化前进方向
	pvt->m_step.x = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);
	pvt->m_step.y = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);

	// 初始化节点环型队列
	pvt->m_points = (POINT*)malloc(linecount * sizeof(POINT));
	pvt->m_pcount = linecount;

	POINT* pp = &(pvt->m_points[0]);
	POINT* pp0;
	pp->x = rand() % WIDTH;
	pp->y = rand() % HEIGHT;
	for (int i = 1; i < linecount; i++) {
		pp0 = &(pvt->m_points[i - 1]);		// 取前一个点的指针
		pp = &(pvt->m_points[i]);			// 取当前点的指针
		// 按移动步长计算当前点的坐标。使用减步长，保证每个点不会超界
		pp->x = pp0->x - pvt->m_step.x;		
		pp->y = pp0->y - pvt->m_step.y;
	}
}

int vertexTailIndex(TVertex* pvt)
{
	return pvt->m_pcount - 1;
}

// 获取头部节点坐标
POINT vertexGetHead(TVertex* pv)
{
	return pv->m_points[0];
}

// 获取尾部节点坐标
POINT vertexGetTail(TVertex* pvt)
{
	// 环形队列尾
	return pvt->m_points[vertexTailIndex(pvt)];
}

// 移动顶点，将环形队列头尾位置
void vertexMove(TVertex* pvt)
{
	// 移动点数组[0,1,2,3,...n-1] => [0,0,1,2,...n-2]，丢弃最后一个点
	for (int i = pvt->m_pcount - 1; i > 0; i--) {
		pvt->m_points[i] = pvt->m_points[i - 1];
	}

	// 按照原来[0]的点计算新位置
	POINT p;
	p.x = pvt->m_points[0].x + pvt->m_step.x;
	p.y = pvt->m_points[0].y + pvt->m_step.y;
	// 判断顶点是否越界
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
	// 更新第一个点作为为p点
	pvt->m_points[0] = p;
}

// 多边形
typedef struct tagPolygon
{
	COLORREF m_color;	// 多边形的颜色
	TVertex* m_vertexs;	// 多边形顶点数组
	int m_vcount;		// 多边形顶点数量
} TPolygon;

// 构造函数
void polygonInit(TPolygon* ppl, int vertexcount, int linecount)
{
	// 初始化颜色
	ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);
	ppl->m_vcount = vertexcount;
	ppl->m_vertexs = (TVertex*)malloc(ppl->m_vcount * sizeof(TVertex));
	// 初始化四个顶点
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexInit(&(ppl->m_vertexs[i]), linecount);
}

void polygonFree(TPolygon* ppl)
{
	for (int i = 0; i < 4; i++)
		vertexFree(&(ppl->m_vertexs[i]));
	free(ppl->m_vertexs);
}

// 绘制并移动多边形
void polygonMove(TPolygon* ppl)
{
	POINT p;
	// 1.擦除顶点所含点数组中最后一个点[m_count-1]包围的多边形
	setcolor(BLACK);
	p = vertexGetTail(&(ppl->m_vertexs[ppl->m_vcount-1]));
	moveto(p.x,  p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetTail(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 2.移动顶点所含点数组的位置+1，丢弃最后一个点[m_count-1]，更新第一个点[0]坐标
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexMove(&(ppl->m_vertexs[i]));

	// 3.绘制顶点所含点数组中第一个点[0]包围的多边形（其他位置顶点不变）
	setcolor(ppl->m_color);
	p = vertexGetHead(&(ppl->m_vertexs[ppl->m_vcount-1]));
	moveto(p.x, p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetHead(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 4.有 1% 的概率更换颜色
	if (rand() % 100 == 0)
		ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);
}

// 主函数
int main()
{
	// 初始化绘图窗口
	initgraph(WIDTH, HEIGHT);

	// 初始化随机种子
	srand((unsigned)time(NULL));

	// 定义两个多边形对象，分别有 7 层线和 12 层线
	TPolygon s1, s2;
	polygonInit(&s1, 5, 7);
	polygonInit(&s2, 9, 12);

	// 移动多边形，按任意键退出
	while (!_kbhit())
	{
		polygonMove(&s1);
		polygonMove(&s2);
		Sleep(20);
	}
	polygonFree(&s1);
	polygonFree(&s2);
	// 关闭绘图窗口
	closegraph();
	return 0;
}
