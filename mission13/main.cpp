// 通过键盘控制一个小球上下左右移动。
// 画出 20 行、30 列的空心圆阵列，将空心圆阵列中坐标为（10, 10）的圆画为实心的。
// 任务 1：通过 a、s、d、w 四个键，实现实心圆的移动。
//         例如，按下 d 键，实心圆从（10, 10）移到（11, 10）。输入大写的 A、S、D、W 仍然有效。
// 任务 2：实现实心圆的自动行走。例如，按下 d 键，实心圆从（10, 10）移到（11, 10），再到（12, 10），
//         一直向右走，移动速度为 500 毫秒移一格。
//         移动到右边界后，从左边穿出来。再按其它方向，便会向其它方向自动行走。
// 任务 3：通过四个方向键实现对实心圆的移动控制（提示：参考微软官方文档中对 _getch 函数的介绍，
//           其中有如何获取功能键的讲解）。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0

void drawBallGrid()
{
	for (int x = 20; x < getwidth(); x += 40) {
		for (int y = 20; y < getheight(); y += 40) {
			circle(x, y, 10);
		}
	}
}

int ballx, bally, step;

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);
	setorigin(0, getheight());

	ballx = getwidth() / 2;
	bally = getheight() / 2;
	step = 2;
	ExMessage m;
	BeginBatchDraw();
	while (true) {
		cleardevice();
		drawBallGrid();
		fillcircle(ballx, bally, 10);

		if (KEY_DOWN(VK_ESCAPE))
			break;
		// 按一次移动一个step
		if (KEY_DOWN('A') || KEY_DOWN(VK_LEFT))
			ballx-=step;
		if (KEY_DOWN('D') || KEY_DOWN(VK_RIGHT))
			ballx+=step;
		if (KEY_DOWN('W') || KEY_DOWN(VK_UP))
			bally+=step;
		if (KEY_DOWN('S') || KEY_DOWN(VK_DOWN))
			bally-=step;

		// 超出屏幕后修正坐标
		if (ballx>getwidth())
			ballx = ballx % getwidth();
		if (bally>getheight())
			bally = bally % getheight();
		if (ballx < 0)
			ballx = getwidth();
		if (bally < 0)
			bally = getheight();

		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
	_getch();
	closegraph();
}
