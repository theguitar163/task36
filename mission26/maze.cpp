
#include "maze.h"

void clearMaze()
{
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            if (Maze[x][y] != itWALL) {
                Maze[x][y] = itROAD;
            }
        }
    }
}

int hasRoad(int x, int y)
{
    if (Maze[x - 1][y] == itROAD || Maze[x + 1][y] == itROAD || Maze[x][y - 1] == itROAD || Maze[x][y + 1] == itROAD)
        return 1;
    else
        return 0;
}