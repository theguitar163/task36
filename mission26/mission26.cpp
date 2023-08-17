// 任务：迷宫求解。
// 该任务包括两点要求：
// (1) 创建一个随机的迷宫，要求该迷宫有且仅有一条从起点通往终点的路线，同时，任何位置都不存在环路。
// (2) 自动求解从起点通往终点的路线。

// 迷宫寻路算法：深度优先遍历
// 迷宫寻路算法：广度优先遍历
// 迷宫寻路算法：A - Star
#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#include "maze.h"
#include "draw_maze.h"

int main()
{
    int seed = GetTickCount();
//    srand(931036593);	//随机数种子
    srand(GetTickCount());
    initgraph(MAX_COL * CELL_SIZE, MAX_ROW * CELL_SIZE);

    BeginBatchDraw();

    // PRIM迷宫生成
//    createMaze_prim();
//    _getch();

    // 深度优先迷宫生成
    createMaze_deepsearch();
   findPath_deepsearch();
 //   _getch();

    // 递归分割迷宫生成
//    findPath_deepsearch();
    _getch();

    EndBatchDraw();
    closegraph();

    printf("%d\n", seed);
}
