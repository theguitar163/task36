// 任务：迷宫求解。
// 该任务包括两点要求：
// (1) 创建一个随机的迷宫，要求该迷宫有且仅有一条从起点通往终点的路线，同时，任何位置都不存在环路。
// (2) 自动求解从起点通往终点的路线。

#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>

#include "maze.h"
#include "draw_maze.h"
#include "panel.h"

#define PANEL_WIDTH 120

TButton buttons1[] = {
    {bsRDRECT, btDEFAULT, L"PRIM算法生成", LIGHTGRAY, 100, 20, &createMaze_prim},
    {bsRDRECT, btDEFAULT, L"深度优先生成", LIGHTGRAY, 100, 20, &createMaze_deepsearch},
    {bsRDRECT, btDEFAULT, L"递归分割生成", LIGHTGRAY, 100, 20, &createMaze_recursivedivision},
};
TButton buttons2[] = {
    {bsRDRECT, btDEFAULT, L"深度优先寻路", LIGHTGRAY, 100, 20, &findPath_deepsearch},
    {bsRDRECT, btDEFAULT, L"广度优先寻路", LIGHTGRAY, 100, 20, &findPath_broadsearch},
    {bsRDRECT, btDEFAULT, L"Ａ* 算法寻路", LIGHTGRAY, 100, 20, &findPath_astarsearch},
};

int main()
{
    srand(GetTickCount());
    initgraph(MAX_COL * CELL_SIZE + PANEL_WIDTH, MAX_ROW * CELL_SIZE);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    BeginBatchDraw();

    TPanel panel;
    initPanel(&panel, PANEL_WIDTH, alRIGHT);

    setButtonPos(&buttons1[0], 10, 20);
    addButton(&panel, &buttons1[0]);
    for (int i = 1; i < sizeof(buttons1) / sizeof(buttons1[0]); i++) {
        addButton(&panel, &buttons1[i], 10, adBOTTOM);
    }

    setButtonPos(&buttons2[0], 10, 150);
    addButton(&panel, &buttons2[0]);
    for (int i = 1; i < sizeof(buttons2) / sizeof(buttons2[0]); i++) {
        addButton(&panel, &buttons2[i], 10, adBOTTOM);
    }

    drawPanel(&panel);
    ExMessage m;
    while (true) {
        if (peekmessage(&m, EM_MOUSE | EM_KEY)) {
            // 左键单击判断
            if (m.message == WM_LBUTTONDOWN) {
                if (ptInPanel({m.x, m.y}, &panel))
                    buttonClick(&panel, m.x, m.y);
            }
        }
        FlushBatchDraw();
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
}
