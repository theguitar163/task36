// 打字母游戏。
// 1.进入游戏后，屏幕顶端随机位置产生一个随机的大写字母下落，按对应按键后字母消失。
// 2.每按对 10 个按键，字母下降速度加快一点，以增加游戏难度。
// 3.字母落到底部后游戏结束。
// 4.游戏窗口右侧显示提示信息：游戏时间、正确数量、错误数量、正确率、打字速度等。
// 5.随机落下的字母由1个变为 3个，按对任何1个按键，对应的字母消失，并重新产生一个新的字母落下。
//   3个字母的逻辑互不影响。
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <easyx.h>
#include <time.h>
#include "typing.h"

int main()
{
	initgraph(1200, 800);
	srand(time(NULL));

	TTyping typing;

	typing.lettercount = 3;
	AddLetter(&typing);

	settextstyle(48, 0, _T("Consolas"));
	BeginBatchDraw();
	ExMessage m;
	while (true) {
		cleardevice();
		render(&typing);

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 27)
				break;
			if (hitLetters(&typing, ch)) {
				AddLetter(&typing);
			}
		}
		int outcount = moveLetters(&typing);
		FlushBatchDraw();

		Sleep(10);
	}
	EndBatchDraw();
	closegraph();
}
