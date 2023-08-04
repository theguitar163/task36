// 任务：雪花。
// 画出分形学的一个经典图案：科赫雪花（Koch snowflake）
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define ROUND(x) (int)round(x)
#define PI 3.14159265359

void drawline(int n, double x, double y, double len, double alpha)
{
    double px = x, py = y;
    double r = len / 3;
    double theta = alpha;
    moveto(ROUND(px), ROUND(py));
    for (int i = 0; i < 4; i++) {

        if (n == 1) {
            px = px + cos(theta) * r;
            py = py + sin(theta) * r;
            lineto(ROUND(px), ROUND(py));
        }
        else {
            drawline(n - 1, px, py, r, theta);
            px = px + cos(theta) * r;
            py = py + sin(theta) * r;
        }
        if (i % 2 == 0) {
            theta = theta + PI / 3;
        }
        else {
            theta = theta - 2 * PI / 3;

        }
    }
}

void drawSnowflake(int n, double x, double y, double len, double alpha)
{
    double px, py;
    drawline(n, x, y, len, alpha);
    x = x + cos(alpha) * len;
    y = y + sin(alpha) * len;
    alpha = alpha - 2 * PI / 3;

    drawline(n, x, y, len, alpha);
    x = x + cos(alpha) * len;
    y = y + sin(alpha) * len;
    alpha = alpha - 2 * PI / 3;
    
    drawline(n, x, y, len, alpha);
    x = x + cos(alpha) * len;
    y = y + sin(alpha) * len;
    alpha = alpha + 2 * PI / 3;
}

int main()
{
    initgraph(800, 800);
    setaspectratio(1, -1);
    setorigin(0, getheight());

    drawSnowflake(10, 100, 600, 600, 0.0);
    _getch();
    closegraph();
    return 0;
}
