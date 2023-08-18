// 迷宫寻路算法：深度优先遍历
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "draw_maze.h"

int hasRoad(int x, int y)
{
    if (Maze[x - 1][y] == itROAD || Maze[x + 1][y] == itROAD || Maze[x][y - 1] == itROAD || Maze[x][y + 1] == itROAD)
        return 1;
    else
        return 0;
}

void findPath_deepsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TList stack;
    initList(&stack, MAX_COL * MAX_ROW);

    int x = sx, y = sy;
    Maze[x][y] = itVISITED;
    push(&stack, { x, y });
    drawCell(x, y, RED);
    FlushBatchDraw();
    while (true) {
        // 到达终点
        if (x == ex && y == ey)
            break;

        if (hasRoad(x,y)) {

            if (Maze[x + 1][y] == itROAD) {
                x++;
            }
            else if (Maze[x - 1][y] == itROAD) {
                x--;
            }
            else if (Maze[x][y + 1] == itROAD) {
                y++;
            }
            else if (Maze[x][y - 1] == itROAD) {
                y--;
            }
            Maze[x][y] = itVISITED;
            push(&stack, { x, y });
            drawCell(x, y, RED);
            FlushBatchDraw();

        }
        else {
            ITEM it;
            if (!pop_stack(&stack, &it)) {
                printf("why?");
                break;
            }
            x = it.x;
            y = it.y;
            if (hasRoad(x, y))
                push(&stack, it);
            else {
                drawCell(x, y, YELLOW);
                FlushBatchDraw();
            }
//            _getch();
        }

 /*       if (x == 41 && y == 45) {
            printf("(%d,%d)%d %d %d %d\n", x, y, Maze[x - 1][y], Maze[x + 1][y], Maze[x][y - 1], Maze[x][y + 1]);
            drawCell(x, y, GREEN);
            FlushBatchDraw();
            _getch();
        }
        FlushBatchDraw();*/
    }
    freeList(&stack);
}