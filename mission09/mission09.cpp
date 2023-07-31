// 模拟指针形式的钟表。
// 自己设计一个钟表表盘并画出来，包括表盘上的刻度、数字等。
// 然后根据当前时间，画出时针、分针、秒针，并实现钟表的持续走时。
// 重点：在不插图的前提下，钟表尽可能的漂亮，
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>

#define ULEN 80
#define PI 3.1415926
#define CLOCK_R 220
#define SHAND_R 200
#define MHAND_R 180
#define HHAND_R 150

// 绘制表盘
void drawClockFace(POINT op)
{
    POINT p1, p2;
    int len, width = 2;
    setlinestyle(PS_SOLID | PS_JOIN_BEVEL, width);
    circle(op.x, op.y, CLOCK_R);
    for (int i = 0; i < 60; i++) {
        p1.x = op.x + round(cos(2 * PI / 60 * i) * CLOCK_R);
        p1.y = op.y + round(sin(2 * PI / 60 * i) * CLOCK_R);

        if (i % 5 == 0) {
            if (i % 15 == 0) {
                len = 15;
                width = 5;
            }
            else {
                len = 10;
                width = 4;
            }
        } 
        else {
            len = 5;
            width = 2;
        }
        p2.x = op.x + round(cos(2 * PI / 60 * i) * (CLOCK_R - len));
        p2.y = op.y + round(sin(2 * PI / 60 * i) * (CLOCK_R - len));

        setlinestyle(PS_SOLID | PS_JOIN_BEVEL, width);
        line(p1.x, p1.y, p2.x, p2.y);
    }
}

// 绘制秒针
void drawSecHand(POINT op, double theta)
{
    POINT p;
    p.x = op.x + (int)round(cos(theta) * SHAND_R);
    p.y = op.y + (int)round(sin(theta) * SHAND_R);
    setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 2);
    line(op.x, op.y, op.x + p.x, op.y + p.y);
}

// 绘制分针
void drawMinHand(POINT op, double theta)
{
    POINT p;
    p.x = op.x + cos(theta) * MHAND_R;
    p.y = op.y + sin(theta) * MHAND_R;
    setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
    line(op.x, op.y, op.x + p.x, op.y + p.y);
}

// 绘制时针
void drawHurHand(POINT op, double theta)
{
    POINT p;
    p.x = op.x + cos(theta) * HHAND_R;
    p.y = op.y + sin(theta) * HHAND_R;
    setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 8);
    line(op.x, op.y, op.x + p.x, op.y + p.y);
}


int main()
{
    initgraph(1200, 1200);
    setaspectratio(1, -1);
    setorigin(getwidth() / 2, getheight() / 2);

    BeginBatchDraw();
    ExMessage m;
    double stheta, mtheta, htheta;
    stheta = mtheta = htheta = 0.5 * PI;
    // 圆形轨道坐标计算公式
    // 其中每次循环theta增长一定角度（弧度），增长量决定了旋转速度
    // x = cos(theta) * r;
    // y = sin(theta) * r;
    while (true) {
        if (peekmessage(&m, EX_KEY))
            break;
        cleardevice();

        drawClockFace({ 0, 0 });

        drawSecHand({ 0,0 }, stheta);
        drawMinHand({ 0,0 }, mtheta);
        drawHurHand({ 0,0 }, htheta);

        FlushBatchDraw();
        Sleep(1000);
        stheta -= (double) (2 * PI / 60);           // 一周60秒
        mtheta -= (double) (2 * PI / (60*60));      // 一周60分钟
        htheta -= (double) (2 * PI / (12*60*60));   // 一周12小时
    }
    EndBatchDraw();
    closegraph();
}

