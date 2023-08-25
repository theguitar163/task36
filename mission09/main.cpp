// 模拟指针形式的钟表。
// 自己设计一个钟表表盘并画出来，包括表盘上的刻度、数字等。
// 然后根据当前时间，画出时针、分针、秒针，并实现钟表的持续走时。
// 重点：在不插图的前提下，钟表尽可能的漂亮，
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include "clock.h"

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

