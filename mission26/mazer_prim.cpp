// 迷宫生成算法：普利姆算法迷宫（Prim）
// 1.让迷宫全都是墙；
// 2.选一个格A设为路，把它[隔壁的隔壁]的所有墙放入列表；
// 3.当列表里还有墙时；
//     1..从列表里随机选一个格B(墙)，
//     2.以这个格B(墙)为起点，扫描四个方向[隔壁的隔壁]，如果是路，则把[隔壁]添加进临时列表
//     3.从临时列表中随机挑一个格C，
//     4.这个格C打通，
//     5.把格B设为通路，把它[隔壁的隔壁]的所有墙放入列表；
//     6.从列表里移除格B。
// Prim算法就是不断地从所有可以是通路的位置中随意选一个挖洞，
// 直到没有可能为通路的位置。整个实现过程还是相当于随意为路线附权值的Prim算法。

#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#include "list.h"
#include "maze.h"
#include "draw_maze.h"

BYTE Maze[MAX_COL][MAX_ROW];

// 将(rx，ry)四个方向[隔壁的隔壁]还是墙壁状态的路点加入待选列表
// 并且将这些路点从墙壁状态改为待定状态（主要用于过程展示） 
void addWall(int rx, int ry, TList* pWallList)
{
    if (rx - 2 >= 0 && Maze[rx - 2][ry] == itWALL) {
        append(pWallList, { rx - 2, ry });
        Maze[rx - 2][ry] = itWAIT;
    }
    if (rx + 2 <= MAX_COL - 1 && Maze[rx + 2][ry] == itWALL) {
        append(pWallList, { rx + 2, ry });
        Maze[rx + 2][ry] = itWAIT;
    }
    if (ry - 2 >= 0 && Maze[rx][ry - 2] == itWALL) {
        append(pWallList, { rx, ry - 2 });
        Maze[rx][ry - 2] = itWAIT;
    }
    if (ry + 2 <= MAX_ROW - 1 && Maze[rx][ry + 2] == itWALL) {
        append(pWallList, { rx, ry + 2 });
        Maze[rx][ry + 2] = itWAIT;
    }
}

// 依据路点A(rx, ry)随机找到隔墙的路点B，将A、B路点中间的墙点打通
// 从A(rx, ry)路点出发，若四个方向中隔墙外已经是路点，则将此墙点加入列表中
// 从墙点列表中随机选择一个，将此墙打通（设置为ROAD）
void breakWall(int rx, int ry)
{
    TList list;
    initList(&list, 4);
    // 四个方向上，[隔壁]的[隔壁]已经是ROAD，则将[隔壁]添加进临时列表
    if (rx - 2 >= 0 && Maze[rx - 2][ry] == itROAD)
        append(&list, { rx - 1, ry });
    if (rx + 2 <= MAX_COL - 1 && Maze[rx + 2][ry] == itROAD)
        append(&list, { rx + 1, ry });
    if (ry - 2 >= 0 && Maze[rx][ry - 2] == itROAD)
        append(&list, { rx, ry - 1 });
    if (ry + 2 <= MAX_ROW - 1 && Maze[rx][ry + 2] == itROAD)
        append(&list, { rx, ry + 1 });

    // 此时list中存放了四个方向的墙[隔壁]
    if (list.size > 0) {
        // 随机挑选一个墙
        int idx = rand() % list.size;
        ITEM wall = get(&list, idx);
        // 将此墙打通，则A(rx, ry) - (wall.x, wally) - （B）均为ROAD，形成通路
        Maze[wall.x][wall.y] = itROAD;
        freeList(&list);
    }
}

// 迷宫生成算法：普利姆算法迷宫（Prim）
// Prim算法就是不断地从所有可以是通路的位置中随意选一个挖洞，
// 直到没有可能为通路的位置。整个实现过程还是相当于随意为路线附权值的Prim算法。
void createMaze_prim()
{
    TList wallList;
    initList(&wallList, MAX_COL * MAX_ROW);

    // 1.让迷宫全都是墙
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            Maze[x][y] = itWALL;
        }
    }

    // 2.选一个路点，将其设为通路
    int rx = 1, ry = 1;
    Maze[rx][ry] = itROAD;
    // 然后把此路点的[隔壁的隔壁]仍然为墙的路点放入列表
    addWall(rx, ry, &wallList);

    // 3.当列表里还有墙状路点时
    while (wallList.size > 0) {
        // 从列表中随机选一个墙状路点wall
        int idx = rand() % wallList.size;
        ITEM wall = get(&wallList, idx);

        // 从wall四面[隔壁的隔壁]随机找一个通路路点，把两者之间墙打通
        breakWall(wall.x, wall.y);
        // 将此墙状路点改为通路
        Maze[wall.x][wall.y] = itROAD;

        // 将wall[隔壁的隔辈]仍然为墙的路点加入列表 (注意，不要重复添加路点！)
        addWall(wall.x, wall.y, &wallList);
        // 从列表中删除移除此墙状路点
        remove(&wallList, idx);

//        drawMaze();
    }

    drawMaze();
//    displayMaze();
    freeList(&wallList);
}

