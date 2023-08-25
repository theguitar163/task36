// 不可思议的图案。
// 任务一：请问如下数学步骤将会生成什么图案？
// (1) 随机生成 3 个点 P[0]、P[1]、P[2]；
// (2) 随机生成 1 个点 P；
// (3) 绘制点 P；
// (4) 随机生成整数 n（0 ≤ n ≤ 2）；
// (5) 令 P = P 与 P[n] 的中点；
// (6) 重复执行步骤(3)～(5) 三万次。
// 写一个程序模拟该过程以求得最终图案。
// 任务二：请继续完成以下任务，求以下迭代公式的图像：
// x’ = a * x + b * y + e
// y’ = c * x + d * y + f
// p = 概率
// x、y 值范围：0 ~ 1
// double a[5] = { 0.35173,  0.35338, 0.5,    0.5154,   0.00364 };
// double b[5] = { 0.35537, -0.3537,  0,     -0.0018,   0 };
// double c[5] = { -0.35537,  0.35373, 0,      0.00157,  0 };
// double d[5] = { 0.35173,  0.35338, 0.5,    0.58795,  0.57832 };
// double e[5] = { 0.3545,   0.2879,  0.25,   0.2501,   0.5016 };
// double f[5] = { 0.5,      0.1528,  0.462,  0.1054,   0.0606 };
// double p[5] = { 0.1773,   0.38,    0.1773, 0.2091,   0.0563 };
// 公式重复 5 万次。
// 简单解释一下步骤：
// (1) 生成坐标(x, y)，范围是 0 < x < 1，0 < y < 1。
// (2) 产生数组下标 n，范围 0 ≤ n ≤ 4。其中 n == 0 的概率为 17.73 % ，n == 1 的概率为 38 % ……，每个值的概率由数组 p[5] 指定，p[i] 的值就是 i 的概率。
// (3) 计算 x’ = a[n] * x + b[n] * y + e[n]，y’ = c[n] * x + d[n] * y + f[n]
// (4) 令 x = x'，y = y'。
// (5) 在(x, y) 位置画一个红色的点。
// (6) 重复执行步骤(2)～(5) 五万次。
// 写一个程序模拟该过程以求得最终图案。
// 注：x、y 坐标是数学坐标，要表示到屏幕上，需要相应放大，且颠倒 y 轴。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <time.h>
#include <math.h>

// 绘制分形三角形
void Q1()
{
	POINT p[3];
	POINT pp;
	int w = getwidth();
	int h = getheight();
	srand(time(NULL));

	p[0].x = rand() % w;
	p[0].y = rand() % h;
	p[1].x = rand() % w;
	p[1].y = rand() % h;
	p[2].x = rand() % w;
	p[2].y = rand() % h;
	pp.x = rand() % w;
	pp.y = rand() % h;

	int step = 0;
	ExMessage m;
	while (!peekmessage(&m, EX_KEY) && step++ < 100000) {
		putpixel(pp.x, pp.y, WHITE);
		int n = rand() % 3;

		pp.x = (int)round((p[n].x + pp.x) * 0.5);
		pp.y = (int)round((p[n].y + pp.y) * 0.5);
	}
}

// 绘制枫叶
void Q2()
{
	// x’ = a * x + b * y + e
	// y’ = c * x + d * y + f
	// p = 概率
	// x、y 值范围：0 ~ 1
	double a[5] = { 0.35173,  0.35338, 0.5,    0.5154,   0.00364 };
	double b[5] = { 0.35537, -0.3537,  0,     -0.0018,   0 };
	double c[5] = { -0.35537, 0.35373, 0,      0.00157,  0 };
	double d[5] = { 0.35173,  0.35338, 0.5,    0.58795,  0.57832 };
	double e[5] = { 0.3545,   0.2879,  0.25,   0.2501,   0.5016 };
	double f[5] = { 0.5,      0.1528,  0.462,  0.1054,   0.0606 };
	double p[5] = { 0.1773,   0.38,    0.1773, 0.2091,   0.0563 };
	srand(time(NULL));
	double x = (double)rand() / RAND_MAX;
	double y = (double)rand() / RAND_MAX;
	int n;

	int count[5] = { 0,0,0,0,0 };
	int step = 0;
	ExMessage m;
	while (!peekmessage(&m, EX_KEY) && step++ < 100000) {
		// 使用RAND_MAX控制概率，减少误差
		int r = rand() % RAND_MAX;
		if (r < p[0] * RAND_MAX)
			n = 0;
		else if (r < (p[0] + p[1]) * RAND_MAX)
			n = 1;
		else if (r < (p[0] + p[1] + p[2]) * RAND_MAX)
			n = 2;
		else if (r < (p[0] + p[1] + p[2] + p[3]) * RAND_MAX)
			n = 3;
		else if (r < (p[0] + p[1] + p[2] + p[3] + p[4]) * RAND_MAX)
			n = 4;

		// 记录每个n的次数
		count[n] = count[n]+1;

		double xx = a[n] * x + b[n] * y + e[n];
		double yy = c[n] * x + d[n] * y + f[n];
		x = xx;
		y = yy;

		putpixel(x * getwidth(), y * getheight(), RED);
	}
	// 检测每个n最终的的概率结果基本正确，迭代次数越大概率的准确性越高
	// 0.1776, 0.3795, 0.1774, 0.2084, 0.0571
	printf("%.4f %.4f %.4f %.4f %.4f\n",
		(double)count[0] / step,
		(double)count[1] / step,
		(double)count[2] / step,
		(double)count[3] / step,
		(double)count[4] / step);
}

int main()
{
	initgraph(800, 800);
	cleardevice();
	Q1();
	Q2();
	_getch();
	closegraph();
}

