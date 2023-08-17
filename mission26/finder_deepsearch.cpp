
#include "maze.h"
#include "list.h"

void finder_deepsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TList stack[MAX_COL * MAX_ROW];
    int x = sx, y = sy;
    
    while (true) {
        if (x == ex && y == ey)
            break;
        if (Maze[x + 1][y] == itROAD) {
            x++;
            push(&stack)
        else if (Maze[x - 1][y] == itROAD)
            x--;
        else if (Maze[x][y + 1] == itROAD)
            y++;
        else if (Maze[x][y - 1] == itROAD)
            y--;
        else {

        }
            

    }
}