#pragma once

#include <easyx.h>

#define CELL_SIZE 10//小方格边长

#define COLOR_ROAD WHITE       // 空气颜色
#define COLOR_WALL DARKGRAY    // 墙壁颜色
#define COLOR_WAIT LIGHTRED   // 选中颜色

/*函数声明*/
void clearscreen(COLORREF color);               // 清空地图为RGB颜色(带有网格)
void drawCell(int x, int y, COLORREF color);    // 在x，y位置画颜色为RGB的方格
void drawMaze();
void displayMaze();