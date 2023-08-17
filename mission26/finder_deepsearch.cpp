#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "draw_maze.h"

void findPath_deepsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TList stack;
    initList(&stack, MAX_COL * MAX_ROW);

    int x = sx, y = sy;
    push(&stack, { x, y });
    Maze[x][y] = itVISITED;
    while (true) {
        // µΩ¥Ô÷’µ„
        if (x == ex && y == ey)
            break;

        if (Maze[x - 1][y] == itROAD || Maze[x + 1][y] == itROAD || Maze[x][y - 1] == itROAD || Maze[x][y + 1] == itROAD) {
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
        }
        else {
            ITEM it;
            if (!pop(&stack, &it)) {
                printf("why?");
                break;
            }
            x = it.x;
            y = it.y;
            drawCell(x, y, YELLOW);
            FlushBatchDraw();
//            _getch();
        }
        FlushBatchDraw();
    }
    freeList(&stack);
}