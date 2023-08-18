// 迷宫寻路算法：广度优先遍历
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "tree.h"
#include "draw_maze.h"


void visit(TListp* plist, ITEM* pitem)
{
    Maze[pitem->x][pitem->y] = itVISITED;
    push(plist, pitem);
}

void findPath_broadsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TListp queue;
    initList(&queue, MAX_COL * MAX_ROW);

    ITEM* proot;
    ITEM* pend = NULL;
    ITEM* pchild;

    proot = createNode({sx, sy});
    ITEM* pcur = proot;
    visit(&queue, pcur);
    // 是否成功标志
    int flag = 0;
    while (queue.size > 0) {
        pop_queue(&queue, &pcur);
        
        drawCell(pcur->x, pcur->y, YELLOW);
        FlushBatchDraw();
        // 是否是终点
        if (pcur->x == ex && pcur->y == ey) {
            pend = pcur;
            flag = 1;        //已到达终点标志
            break;
        }

        if (hasRoad(pcur->x, pcur->y)) {
            if (Maze[pcur->x + 1][pcur->y] == itROAD) {
                pchild = createNode({ pcur->x + 1, pcur->y });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x - 1][pcur->y] == itROAD) {
                pchild = createNode({ pcur->x - 1, pcur->y });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x][pcur->y + 1] == itROAD) {
                pchild = createNode({ pcur->x, pcur->y + 1 });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
            if (Maze[pcur->x][pcur->y - 1] == itROAD) {
                pchild = createNode({ pcur->x, pcur->y - 1 });
                addChild(pcur, pchild);
                visit(&queue, pchild);
            }
        }
    }
    // 成功则输出路径
    if (flag) {
        pcur = pend;
        while (true) {
            drawCell(pcur->x, pcur->y, RED);
            FlushBatchDraw();
            pcur = getParent(pcur);
            if (pcur == proot) {
                drawCell(pcur->x, pcur->y, RED);
                FlushBatchDraw();
                break;
            }
        }
    }
    freeList(&queue);
    freeTree(proot);
}