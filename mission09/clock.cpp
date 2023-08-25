#include <math.h>
#include <easyx.h>
#include "clock.h"

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