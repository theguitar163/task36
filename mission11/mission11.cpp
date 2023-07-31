// 绘制下面两张传说中的视觉错觉图片。
// 1.https://codebus.cn/f/a/0/0/436/11.png
// 2.https://codebus.cn/f/a/0/0/436/11-2.png
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define PI 3.1415926

void drawLineGrid()
{
	setlinecolor(RGB(155, 155, 155));
	setlinestyle(PS_SOLID, 10);
	for (int y = 50; y < getheight(); y += 60)
		line(0, y, getwidth(), y);

	for (int x = 50; x < getwidth(); x += 60)
		line(x, 0, x, getheight());

	// 交叉点绘制圆形
	setlinestyle(PS_SOLID, 0);
	setfillcolor(WHITE);
	for (int x = 50; x < getwidth(); x += 60) {
		for (int y = 50; y < getheight(); y += 60) {
			fillcircle(x, y, 5 * sqrt(2));
		}
	}
}

void drawDot(int x, int y, int r, double angle)
{
	setlinestyle(PS_SOLID, 2);
	setlinecolor(WHITE);
	setfillcolor(RGB(0x9E, 0x02, 0xFB));
	fillcircle(x, y, r);
	setlinecolor(BLACK);
	arc(x - r, y + r, x + r, y - r, angle, angle+PI);
}

void drawDotGrid()
{
	setfillcolor(RGB(0xB9, 0xE6, 0x00));
	fillrectangle(0, getheight(), getwidth(), 0);
	double anglex = 0, angley = PI;
	for (int y = getheight()-20; y > 0; y -= 40) {
		anglex = angley;
		for (int x = 20; x < getwidth(); x += 40) {
			drawDot(x, y, 14, anglex);
			anglex += (PI / 4);
		}
		angley -= (PI / 4);
	}
}

int main()
{
	initgraph(640, 640);
	setaspectratio(1, -1);
	setorigin(0, getheight());

	drawLineGrid();
	Sleep(1000);
	drawDotGrid();
	_getch();
	closegraph();
}