// 模拟 45 度反弹的小球。
// 在屏幕里画一个大矩形。
// 在矩形内的随机位置生成一个小球，小球沿着某个 45 度方向匀速前进，
// 碰到矩形边框后做 45 度反弹，反弹后速度不减，
// 再次碰撞后再次反弹，如此往复循环。
// 模拟这个动画过程。
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>
//#include <windows.h>

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);		// 将坐标系调整为正常（y轴向上，x轴向右）
	setorigin(0, getheight());	// 将原点调整至窗口左下角

	// 绘制矩形边框
	int left = 100, right = 700, top = 500, bottom = 200;
	rectangle(left, top, right, bottom);
	// 初始化随机种子
	srand((unsigned)time(NULL));
	// 随机指定小球起始位置，必须在矩形范围内
	int r = 10;
	POINT p = { left + r + rand() % (right - left - r*2), bottom + r + rand() % (top - bottom - r*2 ) };
	
	// 随机指定小球x轴和y轴运行方向
	int dirx = ((rand() % 1) ? 1 : -1), diry = ((rand() % 1) ? 1 : -1);
	int speed = 2;
	ExMessage m;
	BeginBatchDraw();
	while (true) {
		// 擦除小球
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(p.x, p.y, r);

		if (peekmessage(&m, EX_KEY))
			break;

		// 测试计算小球新的位置
		int newx, newy;
		newx = p.x + dirx * speed;
		newy = p.y + diry * speed;

		// 检测是否与左右两边发生碰撞，若碰撞则改变x轴运行方向
		// 注意，小球半径已经预留一个像素
		if (newx - (r + 1) < left || newx + (r + 1) > right)
			dirx = -dirx;
		// 检测是否与上下两边发生碰撞，若碰撞则改变y轴运行方向
		if (newy - (r + 1) < bottom || newy + (r + 1) > top)
			diry = -diry;
		// 使用最新的x轴y轴运行方向计算实际小球位置
		p.x = p.x + dirx * speed;
		p.y = p.y + diry * speed;

		// 在最新的位置绘制小球
		setfillcolor(GREEN);
		fillcircle(p.x, p.y, r);
		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
}
