// 任务：画标准国旗（框线图）。
// 要求精确地计算出国旗中的每个元素的大小、位置、旋转角度等，并绘制出框线图和填充图
// 难点：需要根据网格精确地计算每个小五角星的旋转角度。同时，将绘制五角星的代码封装为函数，方便反复调用。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <math.h>
#define PI 3.14159265359
#define ULEN 30

void drawStar(POINT op, int r, POINT toward)
{
	POINT pts_out[5], pts_in[5], pts[10];
	double rr;
//	circle(op.x, op.y, r);

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
		// 交错绘制线五角星
		int from = i * 2 % 5, to = (i * 2 + 2) % 5;
		line(pts_out[from].x, pts_out[from].y, pts_out[to].x, pts_out[to].y);
		// 交错填入pts数组
		pts[i * 2] = pts_out[i];
		pts[i * 2 + 1] = pts_in[i];
	}
	// 绘制五角星朝向示意线
//	line(op.x, op.y, toward.x, toward.y);

	setfillcolor(YELLOW);
	fillpolygon((POINT*)pts, 10);
}

void drawFlag(POINT op)
{
	setfillcolor(RED);
	solidrectangle(op.x, op.y, op.x + 30 * ULEN, op.y - 20 * ULEN);
	rectangle(op.x, op.y, op.x + 30 * ULEN, op.y - 20 * ULEN);

	line(op.x, op.y - 10 * ULEN, op.x + 30 * ULEN, op.y - 10 * ULEN);
	line(op.x + 15 * ULEN, op.y, op.x + 15 * ULEN, op.y - 20 * ULEN);
	for (int i = 1; i < 10; i++) {
		line(op.x, op.y - i * ULEN, op.x + 15 * ULEN, op.y - i * ULEN);
	}
	for (int i = 1; i < 15; i++) {
		line(op.x + i * ULEN, op.y, op.x + i * ULEN, op.y - 10 * ULEN);
	}
}

int main()
{
	initgraph(1200, 800);
	setaspectratio(1, -1);
	setorigin(0, getheight());

	POINT op_flag = { 100, 700 };
	POINT op_star = { op_flag.x + 5 * ULEN, op_flag.y - 5 * ULEN };
	POINT op_star1 = { op_flag.x + 10 * ULEN, op_flag.y - 2 * ULEN };
	POINT op_star2 = { op_flag.x + 12 * ULEN, op_flag.y - 4 * ULEN };
	POINT op_star3 = { op_flag.x + 12 * ULEN, op_flag.y - 7 * ULEN };
	POINT op_star4 = { op_flag.x + 10 * ULEN, op_flag.y - 9 * ULEN };


	setlinecolor(RED);
	drawFlag(op_flag);

	setlinecolor(WHITE);
	drawStar(op_star, 3 * ULEN, { op_star.x, op_star.y + 3 * ULEN });
	drawStar(op_star1, ULEN, op_star);
	drawStar(op_star2, ULEN, op_star);
	drawStar(op_star3, ULEN, op_star);
	drawStar(op_star4, ULEN, op_star);
	_getch();
	closegraph();
}