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

CTriangleS* b1 = new CTriangleS(135, { 0, 0 }, BROWN);
CTangram* box = new CTangram({})
	
/*{
	new CTriangleS(135, { 0, 0 }, BROWN),
	new CTriangleS(-135, { -sqrt(2) / 2, sqrt(2) / 2 }, YELLOW),
	new CTriangleM(0, {-sqrt(2), -sqrt(2)}, BLUE),
	new CTriangleL(-45, {0, 0}, RED),
	new CTriangleL(45, {0, 0}, CYAN),
	new CSquare(-135, {0, 0}, GREEN),
	new CParallelogramL(0, {0, -sqrt(2)}, MAGENTA)
}};*/
CTangram house = {
	{eTriangleS, -45, -sqrt(2)/2, -sqrt(2)/2, BROWN},
	{eTriangleS, -135, -sqrt(2)/2, -sqrt(2)/2, YELLOW},
	{eTriangleM, 180, sqrt(2), 0, BLUE},
	{eTriangleL, 135, -(sqrt(2)-1), sqrt(2), RED},
	{eTriangleL, 135, 0, 0, CYAN},
	{eSquare, 0, 0, 1, GREEN},
	{eParallelogramR, 45, 0, 1, MAGENTA},
};
TTangram turtle = {
	{eTriangleS, 135, 0, 0, YELLOW},
	{eTriangleS, 135, 2, 0, BROWN},
	{eTriangleM, -135, 3, 1, BLUE},
	{eTriangleL,0, 0, 0, CYAN},
	{eTriangleL, 180, 2, 2, RED},
	{eSquare, 0, -2, 1, GREEN},
	{eParallelogramL, 45, -1, 1, MAGENTA}
};
TTangram fish = {
	{eTriangleS, 0, 0, 0.5, BROWN},
	{eTriangleS, 90, 0, -0.5, YELLOW},
	{eTriangleM, 45, 1, 0.5, BLUE},
	{eTriangleL, -90, 0, 0, CYAN},
	{eTriangleL, 180, 0, 0, RED},
	{eSquare, 0, 0, -0.5, GREEN},
	{eParallelogramL, 45, 1, -0.5, MAGENTA},
};
TTangram cat = {
	{eTriangleS, 45, 0, 0, BROWN},
	{eTriangleS, -135, 0, 0, YELLOW},
	{eTriangleM, -90, 0.5 * sqrt(2), -2 * sqrt(2), BLUE},
	{eTriangleL, -135, 1.5 * sqrt(2), -sqrt(2), CYAN},
	{eTriangleL, -90, 1.5 * sqrt(2), -(2 + sqrt(2)), RED},
	{eSquare, 135, 0, 0, GREEN},
	{eParallelogramR, -30, 1.5 * sqrt(2), -2 * sqrt(2), MAGENTA}
};

TTangram bird = {
	{eTriangleS, 135, -0.5 * sqrt(2), (1 + 0.5 * sqrt(2)), BROWN},
	{eTriangleS, 135, 0, -2, YELLOW},
	{eTriangleM, 135, 1, -1, BLUE},
	{eTriangleL, 180, 0, 0, CYAN},
	{eTriangleL, 90, 0, 0, RED},
	{eSquare, -90, 0, 0, GREEN},
	{eParallelogramL, -45, 2, -1, MAGENTA},
};

TTangram duct = {
	{eTriangleS, 135, -sqrt(2), sqrt(2), BROWN},
	{eTriangleS, 135, 1.8, sqrt(0.5), YELLOW},
	{eTriangleM, -45, sqrt(2)-1, -sqrt(2)-1, BLUE},
	{eTriangleL, -45, sqrt(2), -sqrt(2), CYAN},
	{eTriangleL, 135, 0, 0, RED},
	{eSquare, -45, -sqrt(0.5), sqrt(0.5), GREEN},
	{eParallelogramL, -90, 0, 0, MAGENTA},
};

TTangram shirt = {
	{eTriangleS, 90, 0, 0, BROWN},
	{eTriangleS, 180, 2, -1, YELLOW},
	{eTriangleM, 135, 1, 0, BLUE},
	{eTriangleL, 180, 1, -1, CYAN},
	{eTriangleL, 0, -1, -3, RED},
	{eSquare, 180, 0, 0,  GREEN},
	{eParallelogramR, -45, -2, -2, MAGENTA},
};

TTangram windmill = {
	{eTriangleS, -135, 0, 0, BROWN},
	{eTriangleS, 135, 1.5 * sqrt(2), -0.5 * sqrt(2), YELLOW},
	{eTriangleM, 0, 0, -sqrt(2), BLUE},
	{eTriangleL, -45, 0, 0, CYAN},
	{eTriangleL, 45, 0.5 * sqrt(2), 0.5 * sqrt(2), RED},
	{eSquare, 45, 0.5 * sqrt(2), -0.5 * sqrt(2), GREEN},
	{eParallelogramL, -90, 0, -sqrt(2), MAGENTA},
};

int main()
{
	initgraph(1200, 1200);
	setaspectratio(1, -1);
	setorigin(0, getheight());

	drawTangram(box, { 200, 1000 });
	drawTangram(house, { 500, 1000 });
	drawTangram(turtle, { 800, 1000 });
	drawTangram(fish, { 200, 700 });
	drawTangram(cat, { 450, 800 });
	drawTangram(bird, { 800, 700 });
	drawTangram(duct, { 200, 300 });
	drawTangram(shirt, { 500, 400 });
	drawTangram(windmill, { 800,300 });
	// getmessage(EX_CHAR);
	_getch();
	closegraph();
	return 0;
}

