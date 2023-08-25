// 多边形与圆。
// 先清空屏幕，在屏幕中央输出数字 3，并画一个正三角形。
// 然后延时 200 毫秒，清空屏幕，在屏幕中央输出数字 4，画一个正四边形。
// 再延时 200 毫秒，清空，输出数字 5，画正五边形。
// ……
// 延时 200 毫秒，清空，输出数字 50，画正 50 边形。
// 在整个过程中，请确保若干正 n 边形的外接圆的半径相同。
// 可以使用 Windows API 函数 Sleep 简单实现延时功能。
// 可以使用 swprintf 函数实现将整型变量格式化输出至 wchar_t 字符串，
// 通过 textwidth 和 textheight 计算出字符串占用的空间大小，
// 再通过 outtextxy 函数将字符串输出至屏幕中央位置。
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <easyx.h>
#include <math.h>
#include <wchar.h>
#define PI 3.14159265359
#define ULEN 30

void drawPolygon(POINT op, int r, int n)
{
	POINT *pts = (POINT *)malloc( n * sizeof(POINT));
	wchar_t str[20];

	// 由于Y轴朝下，需要将其旋转180度（弧度PI）
	double radian = - PI / 2;

	swprintf(str, 10, L"%d边形", n);
	settextstyle(48, 0, _T("微软雅黑"));
	outtextxy(op.x - textwidth(str) / 2, op.y - textheight(str) / 2, str);

	for (int i = 0; i < n; i++) {
		pts[i].x = r * cos(radian + i * 2 * PI / n) + op.x;
		pts[i].y = r * sin(radian + i * 2 * PI / n) + op.y;
	}

	polygon(pts, n);
	free(pts);
}

int main()
{
	initgraph(800, 800);
	// 此处不改变原始坐标系朝向，若改变则outtextxy会输出颠倒的字符
	// setaspectratio(1, -1);
	// setorigin(0, getheight());

	for (int i=3; i<=50; i++) {
		drawPolygon({ getwidth()/2, getheight()/2 }, 300, i);
		Sleep(300);
		cleardevice();
	}
	_getch();
	closegraph();
}
