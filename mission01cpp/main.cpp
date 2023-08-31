// 画七巧板图案
// 请用基本的画线语句，画出任意一个七巧板的图案。
// 画任何自己从网上搜到的七巧板图案都行。图案的整体大小、颜色填充等，自己设计。
// 可以使用 line 函数画线，使用 setfillcolor 和 floodfill 函数实现颜色填充。
// 难点：如果最大的三角形的直角边的长度为100，那么斜边的长为 100 * sqrt(2)，
//       于是很多坐标的计算就会显得有些繁琐。怎样使代码看上去更漂亮，是个学问。
//       剩下的，就是基本的坐标学习和对耐心的练习。
#include <easyx.h>
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include "tangram.h"

CBlock* arrbox[] = {
	new CTriangleS(135, {0, 0}, BROWN),
	new CTriangleS(-135, { -sqrt(2) / 2, sqrt(2) / 2 }, YELLOW),
	new CTriangleM(0, {-sqrt(2), -sqrt(2)}, BLUE),
	new CTriangleL(-45, {0, 0}, RED),
	new CTriangleL(45, {0, 0}, CYAN),
	new CSquare(-135, {0, 0}, GREEN),
	new CParallelogramL(0, {0, -sqrt(2)}, MAGENTA)
};

CBlock* arrhouse[] = {
	new CTriangleS(-45, {-sqrt(2) / 2, -sqrt(2) / 2}, BROWN),
	new CTriangleS(-135, {-sqrt(2) / 2, -sqrt(2) / 2}, YELLOW),
	new CTriangleM(180, {sqrt(2), 0}, BLUE),
	new CTriangleL(135, {-(sqrt(2) - 1), sqrt(2)}, RED),
	new CTriangleL(135, {0, 0}, CYAN),
	new CSquare(0, {0, 1}, GREEN),
	new CParallelogramR(45, {0, 1}, MAGENTA),
};

CBlock* arrturtle[] = {
	new CTriangleS(135, {0, 0}, YELLOW),
	new CTriangleS(135, {2, 0}, BROWN),
	new CTriangleM(-135, {3, 1}, BLUE),
	new CTriangleL(0, {0, 0}, CYAN),
	new CTriangleL(180, {2, 2}, RED),
	new CSquare(0, {-2, 1}, GREEN),
	new CParallelogramL(45, {-1, 1}, MAGENTA)
};

CBlock* arrfish[] = {
	new CTriangleS(0, {0, 0.5}, BROWN),
	new CTriangleS(90, {0, -0.5}, YELLOW),
	new CTriangleM(45, {1, 0.5}, BLUE),
	new CTriangleL(-90, {0, 0}, CYAN),
	new CTriangleL(180, {0, 0}, RED),
	new CSquare(0, {0, -0.5}, GREEN),
	new CParallelogramL(45, {1, -0.5}, MAGENTA)
};

CBlock* arrcat[] = {
	new CTriangleS(45, {0, 0}, BROWN),
	new CTriangleS(-135, {0, 0}, YELLOW),
	new CTriangleM(-90, {0.5 * sqrt(2), -2 * sqrt(2)}, BLUE),
	new CTriangleL(-135, {1.5 * sqrt(2), -sqrt(2)}, CYAN),
	new CTriangleL(-90, {1.5 * sqrt(2), -(2 + sqrt(2))}, RED),
	new CSquare(135, {0, 0}, GREEN),
	new CParallelogramR(-30, {1.5 * sqrt(2), -2 * sqrt(2)}, MAGENTA)
};

CBlock* arrbird[] = {
	new CTriangleS(135, {-0.5 * sqrt(2), (1 + 0.5 * sqrt(2))}, BROWN),
	new CTriangleS(135, {0, -2}, YELLOW),
	new CTriangleM(135, {1, -1}, BLUE),
	new CTriangleL(180, {0, 0}, CYAN),
	new CTriangleL(90, {0, 0}, RED),
	new CSquare(-90, {0, 0}, GREEN),
	new CParallelogramL(-45, {2, -1}, MAGENTA)
};

CBlock* arrduck[] = {
	new CTriangleS(135, {-sqrt(2), sqrt(2)}, BROWN),
	new CTriangleS(135, {1.8, sqrt(0.5)}, YELLOW),
	new CTriangleM(-45, {sqrt(2) - 1, -sqrt(2) - 1}, BLUE),
	new CTriangleL(-45, {sqrt(2), -sqrt(2)}, CYAN),
	new CTriangleL(135, {0, 0}, RED),
	new CSquare(-45, {-sqrt(0.5), sqrt(0.5) }, GREEN),
	new CParallelogramL(-90, {0, 0}, MAGENTA)
};

CBlock* arrshirt[] = {
	new CTriangleS(90, {0, 0}, BROWN),
	new CTriangleS(180, {2, -1}, YELLOW),
	new CTriangleM(135, {1, 0}, BLUE),
	new CTriangleL(180, {1, -1}, CYAN),
	new CTriangleL(0, {-1, -3}, RED),
	new CSquare(180, {0, 0},  GREEN),
	new CParallelogramR(-45, {-2, -2}, MAGENTA)
};

CBlock* arrwindmill[] = {
	new CTriangleS(-135, {0, 0}, BROWN),
	new CTriangleS(135, {1.5 * sqrt(2), -0.5 * sqrt(2)}, YELLOW),
	new CTriangleM(0, {0, -sqrt(2)}, BLUE),
	new CTriangleL(-45, {0, 0}, CYAN),
	new CTriangleL(45, {0.5 * sqrt(2), 0.5 * sqrt(2)}, RED),
	new CSquare(45, {0.5 * sqrt(2), -0.5 * sqrt(2)}, GREEN),
	new CParallelogramL(-90, {0, -sqrt(2)}, MAGENTA)
};

int main()
{
	initgraph(1200, 1200);
	setaspectratio(1, -1);
	setorigin(0, getheight());
	CTangram* box = new CTangram(arrbox);
	CTangram* house = new CTangram(arrhouse);
	CTangram* turtle = new CTangram(arrturtle);
	CTangram* fish = new CTangram(arrfish);
	CTangram* cat = new CTangram(arrcat);
	CTangram* bird = new CTangram(arrbird);
	CTangram* duck = new CTangram(arrduck);
	CTangram* shirt = new CTangram(arrshirt);
	CTangram* windmill = new CTangram(arrwindmill);

	box->drawTangram({ 200, 1000 });
	house->drawTangram({ 500, 1000 });
	turtle->drawTangram({ 800, 1000 });
	fish->drawTangram({ 200, 700 });
	cat->drawTangram({ 450, 800 });
	bird->drawTangram({ 800, 700 });
	duck->drawTangram({ 200, 300 });
	shirt->drawTangram({ 500, 400 });
	windmill->drawTangram({ 800,300 });
	// getmessage(EX_CHAR);
	_getch();

	delete box;
	delete house;
	delete turtle;
	delete fish;
	delete cat;
	delete bird;
	delete duck;
	delete shirt;
	delete windmill;

	closegraph();
	return 0;
}

