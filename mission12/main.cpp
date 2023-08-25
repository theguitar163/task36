// 绘制星空动画。
// 用像素点代表星星。屏幕上有 200 颗星星，从左向右移动。
// 每颗星星拥有不同的亮度和移动速度。越亮的星星，表示离自己越近，移动速度越快。
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <easyx.h>

#define STAR_COUNT 800

typedef struct {
	int x, y;
	unsigned char v;
} TStar;

TStar stars[STAR_COUNT];

void initStars()
{
	srand(time(NULL));
	for (int i = 0; i < STAR_COUNT; i++) {
		stars[i].x = rand() % getwidth();
		stars[i].y = rand() % getheight();
		stars[i].v = rand() % 10;	// 速度太快看不清，限制在16以下
	}
}

void drawStars()
{
	unsigned char light;
	for (int i = 0; i < STAR_COUNT; i++) {
		light = 56 + stars[i].v * 20;  // 星星太暗看不清，限制亮度大于56
		putpixel(stars[i].x, stars[i].y, RGB(light, light, light));
		stars[i].x = (stars[i].x + stars[i].v) % getwidth();   // 确保星星循环移动
	}
}

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);
	setorigin(0, getheight());

	ExMessage m;
	BeginBatchDraw();
	initStars();
	while (true) {
		if (peekmessage(&m, EX_KEY))
			break;
		cleardevice();
		drawStars();
		FlushBatchDraw();
		Sleep(50);
	}
	EndBatchDraw();
	_getch();

	closegraph();
}

