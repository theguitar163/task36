// 模拟 45 度反弹的 10 个小球。
// 在屏幕里画一个大矩形。
// 在矩形内的随机位置生成 10 个小球，每个小球都沿着某个 45 度方向匀速前进，
// 碰到矩形边框后做 45 度反弹，反弹后速度不减，
// 再次碰撞后再次反弹，如此往复循环。模拟这个动画过程。
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>

int left = 100, right = 700, top = 500, bottom = 200;

#define BALL_COUNT 50	// 最多10个球
// 定义球体结构
typedef struct tagBall {
	int x, y;			// 球体圆心坐标
	int r;				// 球体半径
	int dirx, diry;		// 球体x和y轴运行方向（1或-1）
	int speed;			// 移动速度
} TBall;

// 初始化小球结构
void initBall(TBall *pball)
{
	// 球体半径随机，取值范围
	pball->r = 8 + rand() % 10;
	// 随机指定小球起始位置，必须在矩形范围内
	pball->x = left + pball->r + rand() % (right - left - pball->r * 2);
	pball->y = bottom + pball->r + rand() % (top - bottom - pball->r * 2);
	// 随机指定小球x轴和y轴运行方向
	pball->dirx = ((rand() % 10 <5) ? 1 : -1);
	pball->diry = ((rand() % 10 <5) ? 1 : -1);
	// 初始化运动速度（由于规定了45度角，故可以每个小球x、y轴速度可以简化为一个速度）
	pball->speed = 1 + rand() % 5;
}

// 绘制小球，并且更新小球结构
void drawBall(TBall *pball)
{
	// 擦除小球
	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(pball->x, pball->y, pball->r);

	// 测试计算小球新的位置
	int newx, newy;
	newx = pball->x + pball->dirx * pball->speed;
	newy = pball->y + pball->diry * pball->speed;

	// 检测是否与左右两边发生碰撞，若碰撞则改变x轴运行方向
	// 注意，小球半径已经预留一个像素
	if (newx - (pball->r + 1) < left || newx + (pball->r + 1) > right)
		pball->dirx = -pball->dirx;
	// 检测是否与上下两边发生碰撞，若碰撞则改变y轴运行方向
	if (newy - (pball->r + 1) < bottom || newy + (pball->r + 1) > top)
		pball->diry = -pball->diry;

	// 使用最新的x轴y轴运行方向计算实际小球位置
	pball->x = pball->x + pball->dirx * pball->speed;
	pball->y = pball->y + pball->diry * pball->speed;

	// 在最新的位置绘制小球
	setfillcolor(GREEN);
	fillcircle(pball->x, pball->y, pball->r);
}

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);		// 将坐标系调整为正常（y轴向上，x轴向右）
	setorigin(0, getheight());	// 将原点调整至窗口左下角

	// 绘制矩形边框
	int left = 100, right = 700, top = 500, bottom = 200;
	rectangle(left, top, right, bottom);

	// 声明小球数组
	TBall ball[BALL_COUNT];

	// 初始化随机种子
	srand((unsigned)time(NULL));
	// 初始化小球数组中所有结构内容
	for (int i=0; i<BALL_COUNT; i++)
		initBall(&ball[i]);

	ExMessage m;
	BeginBatchDraw();
	while (true) {
		if (peekmessage(&m, EX_KEY))
			break;

		// 逐一绘制小球数组中所有小球
		for (int i = 0; i < BALL_COUNT; i++)
			drawBall(&ball[i]);

		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
}
