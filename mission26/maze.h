#pragma once

#include <easyx.h>

#define MAX_COL 55
#define MAX_ROW 55

#define itROAD 0
#define itWALL 1
#define itWAIT 2
#define itVISITED 3

#define MAX_CHILDREN 4

typedef struct tagItem {
    int x;
    int y;
    // 以下为兼顾广度优先寻路算法，记录遍历路径生成的树形节点
    struct tagItem* pparent = NULL;
    struct tagItem* pchildren[MAX_CHILDREN] = { NULL };
    // 以下为兼顾A*寻路算法，记录//	A*算法的两个参数
    int Gx;	//移动距离
    int Hx;	//离终点的距离
} ITEM;

extern BYTE Maze[MAX_COL][MAX_ROW];

int hasRoad(int x, int y);
int isRoad(int x, int y);
void restoreMaze();

void createMaze_prim();
void createMaze_deepsearch();
void createMaze_recursivedivision();

void findPath_deepsearch();
void findPath_broadsearch();
void findPath_astarsearch();