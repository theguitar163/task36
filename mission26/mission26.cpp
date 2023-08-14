﻿// 任务：迷宫求解。
// 该任务包括两点要求：
// (1) 创建一个随机的迷宫，要求该迷宫有且仅有一条从起点通往终点的路线，同时，任何位置都不存在环路。
// (2) 自动求解从起点通往终点的路线。

// 迷宫生成算法：普利姆算法迷宫（Prim）
// 1.让迷宫全都是墙；
// 2.选一个格，作为迷宫的通路，然后把它的邻墙放入列表；
// 3.当列表里还有墙时；
// ——1.从列表里随机选一个墙，如果它对面的格子不是迷宫的通路；
// ————1.把墙打通，让对面的格子成为迷宫的通路；
// ————2.把那个格子的邻墙加入列表；
// ——2.如果对面的格子已经是通路了，那就从列表里移除这面墙。
// Prim算法就是不断地从所有可以是通路的位置中随意选一个挖洞，
// 直到没有可能为通路的位置。整个实现过程还是相当于随意为路线附权值的Prim算法。

// 迷宫生成算法：递归分割算法（Recursive division）
// 可以用递归分割法创建迷宫，算法的工作原理如下：
// 1.开始创建迷宫，使整个空间没有壁，我们称之为“室”。
// 2.在随机位置生成壁将室分割为两个子室，并在壁上随机开孔，使子室联通。
// 3.重复步骤2，直到所有子室全部不可分割（即子室某一个维度等于1）。
// 例如，在矩形迷宫中，在任意点建立彼此垂直的两个壁。 这两个壁将大腔室分成由四个壁分开的四个较小腔室。 
// 随机选择四个墙壁中的三个，并在三个墙壁的随机点处打开一个单元格的孔。 
// 继续以这种方式递归，直到每个室在两个方向中的任一个方向上具有一个单元的宽度。
// 因为没有起点的概念，所以没有起点位置参数，取而代之的是最小面积参数，
// 这个参数是我用来制造“房间”的，当子空间面积小于这个值就会强制停止分割，
// 这样就可以造出随机的房间，并在一定程度上避免该算法过于弱智的“一条长长的死胡同”问题。

// 迷宫生成算法：递归回溯（Recursive backtracker）

// 迷宫生成算法：深度遍历图（Deep traversal graph）
// 1.将起点作为当前格并标记
// 2.当还存在未标记的格时
// ——1.如果当前格有未标记的邻格
// ————1.随机选择一个未标记的邻格
// ————2.将当前格入栈
// ————3.移除当前格与邻格的墙
// ————4.标记邻格并用它作为当前格
// ——2.反之，如果栈不空
// ————1.栈顶的格子出栈
// ————2.令其成为当前格
// ——3.反之，随机选择一个格子为当前格并标记
// 通俗的说，就是从起点开始随机走，走不通了就原路返回，从下一个能走的地方再开始随机走。

// 迷宫寻路算法：深度优先遍历
// 迷宫寻路算法：广度优先遍历
// 迷宫寻路算法：A - Star
#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#define MAX_COL 3
#define MAX_ROW 3

typedef POINT ITEM;

BYTE maze[MAX_COL * 2 + 1][MAX_ROW * 2 + 1];
// 用于存放未访问的格子（仍然是墙）
typedef struct tagList {
    ITEM* array;
    int size;
} TList;

void initList(TList* plist, int maxsize)
{
    plist->array = (ITEM*)malloc(sizeof(ITEM) * maxsize);
    plist->size = 0;
}

void freeList(TList* plist)
{
    free(plist->array);
    plist->array = NULL;
    plist->size = 0;
}

ITEM get(TList* plist, int idx)
{
    return plist->array[idx];
}

void append(TList* plist, ITEM item)
{
    if (plist->size < MAX_COL * MAX_ROW) {
        plist->array[plist->size] = item;
        plist->size++;
    }
}
void remove(TList* plist, int idx)
{
    if (plist->size > 0) {
        for (int i = idx; i < plist->size; i++)
            plist->array[i] = plist->array[i + 1];
        plist->size--;
    }
}

void clear(TList* plist)
{
    plist->size = 0;
}

TList openlist;

// 添加当前格子(sx, sy)的隔墙相邻的未访问的格子（仍然是墙） 
void addWall(int sx, int sy)
{
    // 左
    if (sx - 2 >= 0 && maze[sx - 2][sy] == 1) {
        append(&openlist, { sx - 2, sy });
        maze[sx - 2][sy] = 2;
    }
    // 右
    if (sx + 2 <= MAX_COL && maze[sx + 2][sy] == 1) {
        append(&openlist, { sx + 2, sy });
        maze[sx + 2][sy] = 2;
    }
    // 上
    if (sy - 2 >= 0 && maze[sx][sy - 2] == 1) {
        append(&openlist, { sx, sy - 2 });
        maze[sx][sy - 2] = 2;

    }
    // 下
    if (sy + 2 <= MAX_ROW && maze[sx][sy + 1] == 1) {
        append(&openlist, { sx, sy + 1 });
        maze[sx][sy + 2] = 2;
    }
}

void breakWall(int wx, int wy)
{
    TList list;
    initList(&list, 4);
    //左
    if (wx - 2 >= 0 && maze[wx - 2][wy] == 0) {
        append(&list, { wx - 1, wy });
    }
    //右
    if (wx + 2 <= MAX_COL && maze[wx + 2][wy] == 0) {
        append(&list, { wx + 1, wy });
    }
    //上
    if (wy - 2 >= 0 && maze[wx][wy - 2] == 0) {
        append(&list, { wx, wy - 1 });
    }
    //下
    if (wy + 2 <= MAX_ROW && maze[wx][wy + 2] == 0) {
        append(&list, { wx, wy + 1 });
    }

    int idx = rand() % list.size;
    ITEM it = get(&list, idx);
    /*将x，y与B打通*/
    maze[it.x][it.y] = 0;
    freeList(&list);
}

void displayMaze()
{
    for (int i = 0; i < MAX_ROW * 2 + 1; i++) {
        for (int j = 0; j < MAX_COL * 2 + 1; j++)
            printf("%d ", maze[j][i]);
        printf("\n");
    }
    _getch();
}

int main()
{
    srand(GetTickCount());
    // 迷宫生成算法：普利姆算法迷宫（Prim）
    // Prim算法就是不断地从所有可以是通路的位置中随意选一个挖洞，
    // 直到没有可能为通路的位置。整个实现过程还是相当于随意为路线附权值的Prim算法。

    // 1.让迷宫全都是墙；
// 2.选一个格，作为迷宫的通路，然后把它的邻墙放入列表；
// 3.当列表里还有墙时；
// ——1.从列表里随机选一个墙，如果它对面的格子不是迷宫的通路；
// ————1.把墙打通，让对面的格子成为迷宫的通路；
// ————2.把那个格子的邻墙加入列表；
// ——2.如果对面的格子已经是通路了，那就从列表里移除这面墙。
// Prim算法就是不断地从所有可以是通路的位置中随意选一个挖洞，
// 直到没有可能为通路的位置。整个实现过程还是相当于随意为路线附权值的Prim算法。
    initList(&openlist, MAX_COL * MAX_ROW);

    // 1.让迷宫全都是墙
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            maze[x][y] = 1;
        }
    }

    // 2.选一个路点，作为迷宫的通路，
    int sx = 1, sy = 1;
    maze[sx][sy] = 1;
    // 然后把此路点的相邻的墙点(非边界墙)放入列表
    addWall(sx, sy);

    // 3.当列表里还有墙时；
    while (openlist.size > 0) {
        // 3.1.从列表里随机选一个墙，
        /*从待选路点随机选一个路点A*/
        int idx = rand() % openlist.size;
        ITEM wall = get(&openlist, idx);

        /*将A与它四周一个随机已经变成路的路点打通*/
        breakWall(wall.x, wall.y);

        maze[wall.x][wall.y] = 0;
        /*将A四周不是路的路点加入待选列表*///(注意，不要重复添加路点！)
        addWall(wall.x, wall.y);
        /*从待选路点中移除A*/
        remove(&openlist, idx);
        displayMaze();
        printf("\n");
    }

    displayMaze();
}