// 模拟物理弹跳的小球。
// 从屏幕左边向右抛出一个小球，自己指定小球 x 方向和 y 方向的初始速度。
// 小球在 y 方向的速度受重力加速度影响。小球碰到底部以及两侧边框时会反弹，
// 反弹时 x 方向和 y 方向的速度会受阻力影响。模拟这个动画过程，直到小球停止运动。
// 小球运动受重力影响，为抛物线运动。若斜抛物体速度为v, 方向为θ，则小球坐标为：
// y = vtsinθ - gt ^ 2 / 2
// x = vtcosθ
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <math.h>
#include <windows.h>

#define G 10
#define ZEROF 0.00000000001
#define ballr 10
#define dragCoef 0.9

int left = 10, right = 790, top = 700, bottom = 100;

// 改变了坐标系为左下角并设置为y轴朝上，故需将鼠标坐标（采用物理坐标）进行映射
int mapy(int y)
{
    return getheight() - y;
}

// 取得运动球体的最新坐标
POINT getNewPos(POINT startp, double vx, double vy, double t)
{
    t = t / 200;   // 正常应除以1000，但为了加快球运行速度缩减分母为200
    return { startp.x + (int)round(vx * t), startp.y + (int)round(vy * t - G * t * t * 0.5) };
}

// 发射前准备,取得起始位置，x,y两个方向的速度
void prepareThrow(POINT *startp, double *vx, double *vy)
{
    ExMessage m;
    int sx, sy, ox, oy;
    int is_down = 0;
    long t;

    while (true) {
        peekmessage(&m, EX_MOUSE);

        if (m.message == WM_LBUTTONDOWN || !is_down) {
            is_down = 1;
            t = GetTickCount();
            sx = ox = m.x;
            sy = oy = mapy(m.y);
        }
        else if (m.message == WM_MOUSEMOVE && m.lbutton) {
            // 擦除
            setlinecolor(BLACK);
            line(sx, sy, ox, oy);
            setlinecolor(WHITE);
            line(sx, sy, m.x, mapy(m.y));
            ox = m.x;
            oy = mapy(m.y);
        }
        else if (m.message == WM_LBUTTONUP && is_down) {
            is_down = 0;
            setlinecolor(BLACK);
            line(sx, sy, m.x, mapy(m.y));
            if (GetTickCount() - t > 500) {
                *vx = abs(m.x - sx);
                *vy = abs(mapy(m.y) - sy);
                startp->x = sx;
                startp->y = sy;
                break;
            }
        }
    }
}

// 开始弹射球体，以startp为起始位置、vx/vy为xy轴方向的速度
void throwBall(POINT startp, double vx, double vy)
{
    long t = GetTickCount();
    POINT oldp = startp, p;
    BeginBatchDraw();

    // 当vx和vy两个方向的速度均为0时，则球体停止滚动
    // tips：浮点数不大可能==0，所以使用相对小的数为阈值，使循环停止
    while (fabs(vx)> ZEROF && fabs(vy)> ZEROF) {

        setfillcolor(BLACK);
        fillcircle(oldp.x, oldp.y, ballr);
        p = getNewPos(startp, vx, vy, GetTickCount() - t);

        // 若碰到四面墙壁，则需要重新修正startp、xy轴速度、起始时间，以及重新计算下一个位置。
        if (p.x - (ballr+1)<= left || p.x + (ballr + 1) >= right || p.y - (ballr + 1) <= bottom || p.y + (ballr + 1) >= top) {
            // 时间从新开始
            t = GetTickCount();
            // 记录当前位置为起始位置
            startp = p;
            if (p.x - (ballr + 1) <= left || p.x + (ballr + 1) >= right) {
                // 修正起始位置，避免起始位置超界
                if (p.x - (ballr+1) <= left) startp.x = left + ballr+1;
                if (p.x + (ballr+1) >= right) startp.x = right - ballr-1;
                // 修正速度为原来的dragCoef，模拟碰壁后的阻力
                vx = -vx * dragCoef;    // 碰左右墙壁，翻转x轴速度的方向
                vy = vy * dragCoef;
                // TODO:若球体已经开始下坠，则球体应该继续下坠，但初速度如何计算？
            }
            if (p.y - (ballr + 1) <= bottom || p.y + (ballr + 1) >= top) {
                // 修正起始位置，避免起始位置超界
                if (p.y - (ballr+1) <=bottom) startp.y = bottom + ballr+1;
                if (p.y + (ballr+1) >=top) startp.y = top - ballr-1;
                // 修正速度为原来的dragCoef，模拟碰壁后的阻力
                vx = vx * dragCoef;
                vy = -vy * dragCoef;    // 碰上下墙壁，翻转y轴速度的方向
            }
            // 以新的起点、xy轴方向的速度、时间，重新计算位置
            p = getNewPos(startp, vx, vy, GetTickCount() - t);
        }

        // 保留当前位置至oldp，便于下一循环擦除
        oldp = p;

        // 在新的位置p上绘制球体
        setfillcolor(WHITE);
        fillcircle(p.x, p.y, ballr);
        // 避免小球闪烁
        FlushBatchDraw();

        Sleep(10);
    }
    EndBatchDraw();
}

int main()
{
    initgraph(800, 800);
    setaspectratio(1, -1);		// 将坐标系调整为正常（y轴向上，x轴向右）
    setorigin(0, getheight());	// 将原点调整至窗口左下角
    rectangle(left, top, right, bottom);
    POINT p;
    double vx, vy;

    prepareThrow(&p, &vx, &vy);
    printf("(%d %d -- %f, %f", p.x, p.y, vx, vy);
    throwBall(p, vx, vy);
    _getch();
    closegraph();
}

