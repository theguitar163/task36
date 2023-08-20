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
    // ����Ϊ��˹������Ѱ·�㷨����¼����·�����ɵ����νڵ�
    struct tagItem* pparent = NULL;
    struct tagItem* pchildren[MAX_CHILDREN] = { NULL };
    // ����Ϊ���A*Ѱ·�㷨����¼//	A*�㷨����������
    int Gx;	//�ƶ�����
    int Hx;	//���յ�ľ���
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