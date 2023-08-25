// 画正弦波、等比螺旋线、等差螺旋线。
// 首先画出数学的二维坐标轴，并在坐标轴上相应的标出刻度。
// 然后，用红色画出一条正弦波，用绿色画出一条等比螺旋线，用蓝色画出一条等差螺旋线。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>

#define ULEN 80
#define PI 3.1415926
int left, right, top, bottom;

// 绘制坐标
void drawCoordinate()
{
    setlinecolor(WHITE);
    line(left, 0, right, 0);
    line(right, 0, right - 10, 5);
    line(right, 0, right - 10, -5);
    for (int i = 1; i < right / ULEN; i++) {
        line(i * ULEN, 3, i * ULEN, -3);
        line(-i * ULEN, 3, -i * ULEN, -3);
    }

    line(0, top, 0, bottom);
    line(0, top, 5, top - 10);
    line(0, top, -5, top - 10);
    for (int i = 1; i < top / ULEN; i++) {
        line(-3, i * ULEN, 3, i * ULEN);
        line(-3, -i * ULEN, 3, -i * ULEN);
    }
}

// 正弦波
// y = sin(x) 注意x为弧度
void drawSineWave()
{
    double dx, dy;
    setlinecolor(RED);
    moveto(0, 0);
    for (int i = 0; i < right; i++) {
        dx = (double)i / ULEN;   //tips: 需要强制转换为double，否则结果会是整型数
        dy = sin(dx) * ULEN;
        lineto(i, (int)round(dy));
    }
    moveto(0, 0);
    for (int i = 0; i < right; i++) {
        dx = (double)-i / ULEN;  // tips：需要强制转为double
        dy = sin(dx) * ULEN;
        lineto(-i, (int)round(dy));
    }

}

// 等比螺旋线
// x = cos(theta) * ratio * ULEN
// y = sin(theta) * ratio * ULEN
// theta为弧度值（0 - n * 2π），代表旋转n圈
// ratio为比率值，每个循环theta角增加0.01，ratio同步等比放大*1.002倍
void drawERatioHelix()
{
    double dx, dy;
    double ratio = 0.01;
    moveto(0, 0);
    setlinecolor(GREEN);
    for (double theta = 0; theta < 10 * PI; theta += 0.01) {
        dx = cos(theta) * ratio * ULEN;
        dy = sin(theta) * ratio * ULEN;
        lineto((int)round(dx), (int)round(dy));
        ratio *= 1.002;
    }
}

// 等差比螺旋线
// x = cos(theta) * ratio * ULEN
// y = sin(theta) * ratio * ULEN
// theta为弧度值（0 - n * 2π），代表旋转n圈
// ratio为比率值，每个循环theta角增加0.01，ratio同步增加0.002
void drawEDiffHelix()
{
    double dx, dy;
    double ratio = 0;
    moveto(0, 0);
    setlinecolor(BLUE);
    for (double theta = 0; theta < 10 * PI; theta += 0.01) {
        dx = cos(theta) * ratio * ULEN;
        dy = sin(theta) * ratio * ULEN;
        lineto((int)round(dx), (int)round(dy));
        ratio += 0.002;
    }
}

int main()
{
    initgraph(1200, 1200);
    setaspectratio(1, -1);
    setorigin(getwidth()/2, getheight()/2);

    left = -getwidth() / 2;
    right = getwidth() / 2;
    top = getheight() / 2;
    bottom = -getheight() / 2;

    drawCoordinate();
    drawSineWave();
    drawERatioHelix();
    drawEDiffHelix();

    _getch();
    closegraph();
}

