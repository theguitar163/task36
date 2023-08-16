// 任务：迷宫求解。
// 该任务包括两点要求：
// (1) 创建一个随机的迷宫，要求该迷宫有且仅有一条从起点通往终点的路线，同时，任何位置都不存在环路。
// (2) 自动求解从起点通往终点的路线。

// 迷宫生成算法：递归回溯（Recursive backtracker）

// 迷宫寻路算法：深度优先遍历
// 迷宫寻路算法：广度优先遍历
// 迷宫寻路算法：A - Star
#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#include "maze.h"
#include "draw_maze.h"

// 随机获取区间中的偶数
int getEven1(int min, int max)
{
    // 确保为偶数
    min = (min % 2 == 1) ? min + 1 : min;
    max = (max % 2 == 1) ? max - 1 : max;
    if (max - min < 4) return -1;
    // Divide both by 2 to ensure the range
    int randomNum = ((rand() % ((max - min)) + min) + 1) / 2;

    return randomNum * 2; // multiply by 2 to make the number even
}

int main()
{
    srand(GetTickCount());	//随机数种子

    printf("%d\n", getEven1(0,4));
    printf("%d\n", getEven1(1,3));
    printf("%d\n", getEven1(1,3));
    printf("%d\n", getEven1(1,3));
    _getch();

    initgraph(MAX_COL * CELL_SIZE, MAX_ROW * CELL_SIZE);

    BeginBatchDraw();

 //   createMaze_prim();
 //   _getch();

//    createMaze_deepsearch();
    createMaze_recursivedivision();

    EndBatchDraw();
    _getch();
}
