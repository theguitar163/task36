
#include "maze.h"

// 全局迷宫
BYTE Maze[MAX_COL][MAX_ROW];

// 将墙以外的节点全部恢复为道路
void restoreMaze()
{
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            if (Maze[x][y] != itWALL) {
                Maze[x][y] = itROAD;
            }
        }
    }
}

// 判断周围是否存在道路
int hasRoad(int x, int y)
{
    if (Maze[x - 1][y] == itROAD || Maze[x + 1][y] == itROAD || Maze[x][y - 1] == itROAD || Maze[x][y + 1] == itROAD)
        return 1;
    else
        return 0;
}

// 判断是否是道路
int isRoad(int x, int y)
{
    // 不能越界
    if (x < 0 || x >= MAX_COL || y < 0 || y >= MAX_ROW)
        return 0;
    // 等于道路
    if (Maze[x][y] == itROAD)
        return 1;
    // 否则返回0
    return 0;
}