// 用五条直线画出一个正五角星。
// 要求绘制的五角星为正五角星，由 5 条直线构成。
// 难点：计算五角星的 5 个顶点坐标是该题目的重点。
//       5 个顶点均匀分布在五角星的外接圆上，利用三角函数可以轻松求得圆周上的五等分点坐标。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <math.h>
#define PI 3.14159265359

// 绘制五角星
// 参数：op顶点，r外直径，toward为顶点指向的点
void drawStar(POINT op, int r, POINT toward)
{
	POINT pts[5];
	circle(op.x, op.y, r);

	// 求toward点到op点连线与x轴的夹角(弧度)
	// 注意反正切函数atain2参数为（y, x）
	double radian = atan2((double)(toward.y - op.y), (double)(toward.x - op.x));

	// 正五角星五个外定点坐标，可以简化为将圆周分为5等份
	// 从弧度0开始（此时五角星顶点向右），依次为0、1/5 * 2π、2/5 * 2π、3/5 * 2π、4/5 * 2π，
	// 同时考虑偏移夹角radian
	for (int i = 0; i < 5; i++) {
		pts[i].x = r * cos(radian + i * 0.2 * 2 * PI) + op.x;
		pts[i].y = r * sin(radian + i * 0.2 * 2 * PI) + op.y;
	}

	for (int i = 0; i < 5; i++) {
		int from = i * 2 % 5, to = (i * 2 + 2) % 5;
		line(pts[from].x, pts[from].y, pts[to].x, pts[to].y);
	}
	// 绘制五角星朝向示意线
	line(op.x, op.y, toward.x, toward.y);
}

// 绘制填充五角星
// 参数：op顶点，r外直径，toward为顶点指向的点
void fillStar(POINT op, int r, POINT toward)
{
	POINT pts_out[5], pts_in[5], pts[10];
	double rr;

	// 求toward点到op点连线与x轴的夹角(弧度)
	// 注意反正切函数atain2参数为（y, x）
	double radian = atan2((double)(toward.y - op.y), (double)(toward.x - op.x));

	// 计算五角星五个外定点坐标，可以简化为将圆周分为5等份
	// 从弧度0开始（此时五角星顶点向右），依次为0、1/5 * 2π、2/5 * 2π、3/5 * 2π、4/5 * 2π，
	// 同时考虑偏移夹角radian
	for (int i = 0; i < 5; i++) {
		pts_out[i].x = r * cos(radian + i * 0.2 * 2 * PI) + op.x;
		pts_out[i].y = r * sin(radian + i * 0.2 * 2 * PI) + op.y;
	}

	// 计算内圆的半径
	// rr = r * sin18 / cos36
	rr = r * sin(0.1 * PI) / cos(0.2 * PI);

	// 计算五角星五个内顶点坐标，计算方法与外顶点相同
	// 唯一差别在于，起始弧度不是从0开始，而是从36度（1/10π）开始
	for (int i = 0; i < 5; i++) {
		pts_in[i].x = rr * cos(radian + (i * 0.2 + 0.1) * 2 * PI) + op.x;
		pts_in[i].y = rr * sin(radian + (i * 0.2 + 0.1) * 2 * PI) + op.y;
	}

	// 将外顶点和内顶点数组合并，按多边形绘制要求，交错填充进pts数组中
	for (int i = 0; i < 5; i++) {
		// 交错填入pts数组
		pts[i * 2] = pts_out[i];
		pts[i * 2 + 1] = pts_in[i];
	}
	setfillcolor(YELLOW);
	fillpolygon((POINT*)pts, 10);
}

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);
	setorigin(getwidth()/2, getheight()/2);
	int r = 80;

	drawStar({ 0, 0 }, r, { 0, r});
	fillStar({ -200, 0 }, r, { 0, 0 });
	fillStar({ 200, 0 }, r, { 0, 0 });
	fillStar({ 0, 200 }, r, { 0, 0 });
	fillStar({ 0, -200 }, r, { 0, 0 });
	_getch();
	closegraph();
}