
#include "maze.h"

// ȫ���Թ�
BYTE Maze[MAX_COL][MAX_ROW];

// ��ǽ����Ľڵ�ȫ���ָ�Ϊ��·
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

// �ж���Χ�Ƿ���ڵ�·
int hasRoad(int x, int y)
{
    if (Maze[x - 1][y] == itROAD || Maze[x + 1][y] == itROAD || Maze[x][y - 1] == itROAD || Maze[x][y + 1] == itROAD)
        return 1;
    else
        return 0;
}

// �ж��Ƿ��ǵ�·
int isRoad(int x, int y)
{
    // ����Խ��
    if (x < 0 || x >= MAX_COL || y < 0 || y >= MAX_ROW)
        return 0;
    // ���ڵ�·
    if (Maze[x][y] == itROAD)
        return 1;
    // ���򷵻�0
    return 0;
}