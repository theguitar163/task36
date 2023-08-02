// 任务：模拟 Windows XP 的屏保“变幻线”。

#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#define	WIDTH	640
#define	HEIGHT	480
#define	MAXSTEP	9

// 顶点（四个顶点，组成一个多边形）
typedef struct tagVertex
{
	POINT* m_points;		// 顶点环形队列，每个顶点队列包含多个临近的顶点坐标
	int m_count;			// 顶点环形队列的长度
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
	pvt->m_count = linecount;

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
	return pvt->m_count - 1;
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
	POINT* pphead = &(pvt->m_points[pvt->m_headidx]);
	POINT* pptail = &(pvt->m_points[vertexTailIndex(pvt)]);

	pptail->x = pphead->x + pvt->m_step.x;
	pptail->y = pphead->y + pvt->m_step.y;
	// 将环形队列头索引替换为尾部索引
	pvt->m_headidx = vertexTailIndex(pvt);

	pphead = &(pvt->m_points[pvt->m_headidx]);
	// 判断顶点是否越界
	if (pphead->x < 0) {
		pphead->x = -pphead->x;
		pvt->m_step.x = rand() % MAXSTEP + 1;
	}
	else if (pphead->x >= WIDTH) {
		pphead->x -= pphead->x - WIDTH + 1;
		pvt->m_step.x = -rand() % MAXSTEP - 1;
	}
	if (pphead->y < 0) {
		pphead->y = -pphead->y;
		pvt->m_step.y = rand() % MAXSTEP + 1;
	}
	else if (pphead->y >= HEIGHT) {
		pphead->y -= pphead->y - HEIGHT + 1;
		pvt->m_step.y = -rand() % MAXSTEP - 1;
	}
}

// 多边形
typedef struct tagPolygon
{
	COLORREF m_color;		// 多边形的颜色
	TVertex m_vertex[4];	// 构成多边形的四个顶点
} TPolygon;

// 构造函数
void polygonInit(TPolygon* ppl, int linecount)
{
	// 初始化颜色
	ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);

	// 初始化四个顶点
	for (int i = 0; i < 4; i++)
		vertexInit(&(ppl->m_vertex[i]), linecount);
}

void polygonFree(TPolygon* pp)
{
	for (int i = 0; i < 4; i++)
		vertexFree(&(pp->m_vertex[i]));
}

// 移动多边形
// 每次绘制多边形时，只有两步操作：
// 1.擦除顶点队列最后一组（pvt->m_count-1）多边形
// 2.将顶点队列向后移动，空出位置0
// 3.更新位置0顶点的坐标
// 4.绘制位置0顶点的多边形（其他位置顶点不变）
void polygonMove(TPolygon* ppl)
{
	int i;
	POINT p;
	// 擦掉环形队列尾部多边形
	setcolor(BLACK);
	p = vertexGetTail(&(ppl->m_vertex[3]));
	moveto(p.x,  p.y);
	for (i = 0; i < 4; i++) {
		p = vertexGetTail(&(ppl->m_vertex[i]));
		lineto(p.x, p.y);
	}

	// 移动每个顶点环形队列，将尾部变成头部
	for (i = 0; i < 4; i++)
		vertexMove(&(ppl->m_vertex[i]));

	// 绘制环形队列头部多边形
	setcolor(ppl->m_color);
	p = vertexGetHead(&(ppl->m_vertex[3]));
	moveto(p.x, p.y);
	for (i = 0; i < 4; i++) {
		p = vertexGetHead(&(ppl->m_vertex[i]));
		lineto(p.x, p.y);
	}

	// 有 1% 的概率更换颜色
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
	polygonInit(&s1, 7);
	polygonInit(&s2, 12);

	// 移动多边形，按任意键退出
	while (!_kbhit())
	{
		polygonMove(&s1);
		polygonMove(&s2);
		Sleep(1000);
	}
	polygonFree(&s1);
	polygonFree(&s2);
	// 关闭绘图窗口
	closegraph();
	return 0;
}
