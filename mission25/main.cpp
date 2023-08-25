// 任务：求解八皇后问题。
// 八皇后问题（Eight queens），由国际西洋棋棋手马克斯·贝瑟尔于1848年提出：
// 在8×8格的国际象棋上摆放 8 个皇后，使其不能互相攻击，即任意两个皇后都不
// 能处于同一行、同一列或同一斜线上，问有多少种摆法。
// 请写一个程序，动态演示八皇后问题的求解过程。

#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include "eightqueens.h"

int main()
{
    TCrownImage crown;
    TBoard board;

    initgraph(BORDER_WIDTH * 2 + CELL_WIDTH * MAX_CELL, BORDER_WIDTH * 2 + CELL_WIDTH * MAX_CELL);
    srand(GetTickCount());

    // 加载皇冠图形
//    initCrownFile(&crown, L"\\C语言编程\\mission\\mission25\\crown.png", L"\\C语言编程\\mission\\mission25\\crownmask.png");
    initCrownRes(&crown, L"PNG", L"crown", L"crownmask");
    // 初始化棋盘
    initBoard(&board, &crown);

    BeginBatchDraw();
    // 求解八皇后问题
    addQueen(&board, 0);
    EndBatchDraw();

    _getch();
    closegraph();
    return 0;
}