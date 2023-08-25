// 双人击球游戏。
//
// 请顺序完成以下步骤：
// 1. 在屏幕上画一个较大的长方形作为游戏区域，里面有一个小球做 45 度反弹。
// 2. 将游戏区域长方形的左、右边界擦掉，改为在左右各画一个小矩形表示挡板，通过按键可以移动这个挡板。
//    左右挡板只能在己方的区域（以不同颜色区分）内移动。小球碰到挡板就反弹；小球越过左、右边界，游戏结束。
// 3. 左侧游戏者通过ASDW、右侧游戏者通过方向键在各自的区域内上下左右四个方向移动挡板，击打小球。
// 4. 挡板改为圆球，小球碰撞到球形挡板后，根据两球碰撞的接触位置，计算出小球的反弹角度。
// 5. 在顶部增加游戏信息，先累计赢 3 次者，胜利。

#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include "batgame.h"

TTable table;
TBat batA, batB;
TPlayer playerA, playerB;
TBall ball;

void initAll(int all)
{
    initTable(&table);
    initBat(&batA, &table, SIDE_LEFT, BAT_TYPE_BALL);
    initBat(&batB, &table, SIDE_RIGHT); // , BAT_TYPE_BALL);
    if (all) {
        initPlayer(&playerA, L"喜洋洋", SIDE_LEFT, &table);
        initPlayer(&playerB, L"美羊羊", SIDE_RIGHT, &table);
    }
    initBall(&ball, &table, &batA, &batB, &playerA, &playerB);
}

void render()
{
    drawTable(&table);
    drawBat(&batA);
    drawBat(&batB);
    drawBall(&ball);
    drawPlayer(&playerA);
    drawPlayer(&playerB);
}

int main()
{
    initgraph(1200, 600);
    srand(time(NULL));

    initAll();

    BeginBatchDraw();
    while (true) {
        if (KEY_DOWN(VK_ESCAPE))
            break;

        if (KEY_DOWN('A')) moveBat(&batA, VK_LEFT);
        if (KEY_DOWN(VK_LEFT)) moveBat(&batB, VK_LEFT);
        if (KEY_DOWN('D')) moveBat(&batA, VK_RIGHT);
        if (KEY_DOWN(VK_RIGHT)) moveBat(&batB, VK_RIGHT);
        if (KEY_DOWN('W')) moveBat(&batA, VK_UP);
        if (KEY_DOWN(VK_UP)) moveBat(&batB, VK_UP);
        if (KEY_DOWN('S')) moveBat(&batA, VK_DOWN);
        if (KEY_DOWN(VK_DOWN)) moveBat(&batB, VK_DOWN);

        if (!moveBall(&ball))
            initAll(0);
        render();
        FlushBatchDraw();
        Sleep(10);
    }
    EndBatchDraw();
    closegraph();
}
