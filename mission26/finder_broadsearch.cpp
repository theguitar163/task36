// 迷宫寻路算法：广度优先遍历
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "draw_maze.h"


void visit(TList* plist, int x, int y)
{
    Maze[x][y] = itVISITED;
    push(plist, { x, y });
    drawCell(x, y, YELLOW);
    FlushBatchDraw();
}

void findPath_broadsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TList queue;
    initList(&queue, MAX_COL * MAX_ROW);

    ITEM cur = { sx, sy };
    visit(&queue, cur.x, cur.y);
    // 是否成功标志
    int flag = 0;
    while (queue.size > 0) {
        pop_queue(&queue, &cur);

        // 是否是终点
        if (cur.x == ex && cur.y == ey) {  
            flag = 1;        //已到达终点标志
            break;
        }

        if (hasRoad(cur.x, cur.y)) {
            ITEM next;

            if (Maze[cur.x + 1][cur.y] == itROAD) {
                visit(&queue, cur.x + 1, cur.y);
            }
            if (Maze[cur.x - 1][cur.y] == itROAD) {
                visit(&queue, cur.x - 1, cur.y);
            }
            if (Maze[cur.x][cur.y + 1] == itROAD) {
                visit(&queue, cur.x, cur.y + 1);
            }
            if (Maze[cur.x][cur.y - 1] == itROAD) {
                visit(&queue, cur.x, cur.y - 1);
            }
        }
    }
    // 成功则输出路径
    if (!flag) {
        printf("die\n");
    }
    freeList(&queue);
}