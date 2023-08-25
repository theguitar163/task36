// 模拟太阳、地球、月亮三个天体的运动。
// 画一个黄色的圆表示太阳。
// 画一个绿色的圆表示地球。地球以椭圆形轨道围绕太阳旋转。
// 再画一个黄色的小圆表示月亮，月亮以椭圆轨道围绕地球旋转。
// 模拟这三个天体运动的过程。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include <math.h>

#define ULEN 80
#define PI 3.1415926

// 绘制太阳
void drawSun(POINT p)
{
    setfillcolor(YELLOW);
    fillcircle(p.x, p.y, 50);
}

// 绘制地球
void drawEarth(POINT p)
{
    static POINT oldp = { 0, 0 };
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    fillcircle(oldp.x, oldp.y, 10);

    setfillcolor(BLUE);
    fillcircle(p.x, p.y, 10);
    oldp = p;
}

// 绘制月亮
void drawMoon(POINT p)
{
    static POINT oldp = { 0, 0 };
    setlinecolor(BLACK);
    setfillcolor(BLACK);
    fillcircle(oldp.x, oldp.y, 5);

    setfillcolor(WHITE);
    fillcircle(p.x, p.y, 5);
    oldp = p;
}



int main()
{
    initgraph(1200, 1200);
    setaspectratio(1, -1);
    setorigin(getwidth() / 2, getheight() / 2);

    BeginBatchDraw();
    ExMessage m;
    int erx = 500, ery = 250;
    int mrx = 80, mry = 40;
    int ex, ey, mx, my;
    double etheta = 0, mtheta = 0;
    // 椭圆轨道坐标计算公式
    // 其中每次循环theta增长一定角度（弧度），增长量决定了旋转速度
    // x = sin(theta) * rx;
    // y = cos(theta) * ry;
    while (true) {
        if (peekmessage(&m, EX_KEY))
            break;

        drawSun({ 0, 0 });

        ex = sin(etheta) * erx;
        ey = cos(etheta) * ery;
        drawEarth({ ex, ey });

        mx = ex+sin(mtheta) * mrx;
        my = ey+cos(mtheta) * mry;
        drawMoon({ mx, my });

        FlushBatchDraw();
        Sleep(20);
        etheta += 0.01;     // 地球旋转速度，一周为一年
        mtheta += 0.1;      // 月球旋转速度，一周为一月
    }
    EndBatchDraw();
    closegraph();
}

