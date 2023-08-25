// 任务：汉诺塔求解。
// 汉诺塔（Tower of Hanoi），一块黄铜板上插着三根宝石针。在其中一根针从下到上地穿好了由大到小的 64 片金片。
// 按照下面的规则移动这些金片：一次只移动一片，不管在哪根针上，小片必须在大片上面。
// 请将所有的金片都从穿好的那根针上移到另外一根针上。
// 请写一个程序，动态演示 n 层汉诺塔的求解过程。
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "hanoi.h"

int main()
{
	THanoiStack hanoi;

	initgraph(800, 600);
	srand(time(NULL));
	initHanoiStack(&hanoi, 8, 50, getheight() - 150, getwidth() - 100, 20, 4, getheight() - 300);

	BeginBatchDraw();
	drawHanoiStack(&hanoi);
	_getch();

	solveHanoiStack(&hanoi, hanoi.layercount, &hanoi.stkA, &hanoi.stkB, &hanoi.stkC);
	EndBatchDraw();

	_getch();
	closegraph();
	return 0;
}

