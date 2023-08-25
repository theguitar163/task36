#pragma once

#include <easyx.h>

#define ULEN 80
#define PI 3.1415926
#define CLOCK_R 220
#define SHAND_R 200
#define MHAND_R 180
#define HHAND_R 150

// 绘制表盘
void drawClockFace(POINT op);
// 绘制秒针
void drawSecHand(POINT op, double theta);
// 绘制分针
void drawMinHand(POINT op, double theta);
// 绘制时针
void drawHurHand(POINT op, double theta);