// 任务：模拟 Windows XP 的屏保“变幻线”
// 变幻线每一个顶点都由一组等距的点组成
// 每一次更新，只需要擦除最后一根线，重新绘制第一根线，其他的线不动
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "skyline.h"

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
	polygonInit(&s2, 3, 12);

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
