// 任务：雪花。
// 画出分形学的一个经典图案：科赫雪花（Koch snowflake）
#include <stdio.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define PI 3.14159265359

void drawSnowflake(int n, int len, int x, int y, double alpha)
{
    double px = x, py = y;
    double flen = len / n;
    double theta = alpha;
    moveto(x, y);
    for (int i = 0; i < n; i++) {
        px = px + cos(theta) * flen;
        py = py + sin(theta) * flen;
        lineto((int)round(px), (int)round(py));
        if (i % 2 == 0)
            theta = theta - 2 * PI / 3;
        else
            theta = theta + PI / 3;
        _getch();
    }
}

int main()
{
    initgraph(800, 800);
    setaspectratio(1, -1);
    setorigin(0, getheight());

    drawSnowflake(3*4, 300, 200, 300, PI / 3);
    _getch();
    closegraph();
    return 0;
}
