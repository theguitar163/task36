// 任务：模拟 Windows XP 的屏保“变幻线”。
//

#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#define	WIDTH	640
#define	HEIGHT	480
#define	MAXSTEP	9

// 节点（多个连续的节点环，组成一个顶点）
typedef struct tagPointNode
{
	POINT pos;					// 位置
	struct tagPointNode* next;	// 指向下一个节点的指针
} TPointNode;

// 顶点（四个顶点，组成一个多边形）
typedef struct tagVertex
{
	TPointNode* m_head;		// 头结点的指针
	POINT m_step;			// 移动步长
} TVertex;

// 析构函数
void vertexFree(TVertex* pv)
{
	if (pv->m_head != NULL)
		free(pv->m_head);
}

// 初始化环链
void vertexInit(TVertex* pv, int linecount)
{
	// 初始化前进方向
	pv->m_step.x = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);
	pv->m_step.y = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);

	// 初始化节点环
	pv->m_head = (TPointNode*)malloc(linecount * sizeof(TPointNode));
	pv->m_head[0].pos.x = rand() % WIDTH;
	pv->m_head[0].pos.y = rand() % HEIGHT;
	pv->m_head[0].next = &(pv->m_head[linecount - 1]);
	for (int i = 1; i < linecount; i++)
	{
		pv->m_head[i].pos.x = pv->m_head[i - 1].pos.x - pv->m_step.x;
		pv->m_head[i].pos.y = pv->m_head[i - 1].pos.y - pv->m_step.y;
		pv->m_head[i].next = &(pv->m_head[i - 1]);
	}
}

// 获取头部节点坐标
POINT vertexGetHead(TVertex* pv)
{
	return pv->m_head->pos;
}

// 获取尾部节点坐标
POINT vertexGetTail(TVertex* pv)
{
	return pv->m_head->next->pos;
}

// 移动顶点
void vertexMove(TVertex* pv)
{
	pv->m_head->next->pos.x = pv->m_head->pos.x + pv->m_step.x;
	pv->m_head->next->pos.y = pv->m_head->pos.y + pv->m_step.y;
	pv->m_head = pv->m_head->next;

	// 判断顶点是否越界
	if (pv->m_head->pos.x < 0) {
		pv->m_head->pos.x = -pv->m_head->pos.x;
		pv->m_step.x = rand() % MAXSTEP + 1;
	}
	else if (pv->m_head->pos.x >= WIDTH) {
		pv->m_head->pos.x -= pv->m_head->pos.x - WIDTH + 1;
		pv->m_step.x = -rand() % MAXSTEP - 1;
	}
	if (pv->m_head->pos.y < 0) {
		pv->m_head->pos.y = -pv->m_head->pos.y;
		pv->m_step.y = rand() % MAXSTEP + 1;
	}
	else if (pv->m_head->pos.y >= HEIGHT) {
		pv->m_head->pos.y -= pv->m_head->pos.y - HEIGHT + 1;
		pv->m_step.y = -rand() % MAXSTEP - 1;
	}
}

// 多边形
typedef struct tagPolygon
{
	COLORREF m_color;		// 多边形的颜色
	TVertex m_vertex[4];	// 构成多边形的四个顶点
} TPolygon;

// 构造函数
void polygonInit(TPolygon* pp, int linecount)
{
	// 初始化颜色
	pp->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);

	// 初始化四个顶点
	for (int i = 0; i < 4; i++)
		vertexInit(&(pp->m_vertex[i]), linecount);
}

	// 移动多边形
void polygonMove(TPolygon* pp)
{
	int i;
	POINT p;
	// 擦掉多边形的尾部
	setcolor(BLACK);
	p = vertexGetTail(&(pp->m_vertex[3]));
	moveto(p.x,  p.y);
	for (i = 0; i < 4; i++) {
		p = vertexGetTail(&(pp->m_vertex[i]));
		lineto(p.x, p.y);
	}

	// 移动每个顶点
	for (i = 0; i < 4; i++)
		vertexMove(&(pp->m_vertex[i]));

	// 画多边形的头部
	setcolor(pp->m_color);
	p = vertexGetHead(&(pp->m_vertex[3]));
	moveto(p.x, p.y);
	for (i = 0; i < 4; i++) {
		p = vertexGetHead(&(pp->m_vertex[i]));
		lineto(p.x, p.y);
	}

	// 有 1% 的概率更换颜色
	if (rand() % 100 == 0)
		pp->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);
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

	// 关闭绘图窗口
	closegraph();
	return 0;
}
