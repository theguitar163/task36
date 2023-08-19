// 迷宫寻路算法：广度优先遍历
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "tree.h"
#include "draw_maze.h"


/*void visit(TListp* plist, ITEM* pitem)
{
    Maze[pitem->x][pitem->y] = itVISITED;
    push(plist, pitem);
}

void findROAD_astarsearch()
{
    int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
    TListp queue;
    initList(&queue, MAX_COL * MAX_ROW);

    ITEM* proot;
    ITEM* pend = NULL;
    ITEM* pchild;

    proot = createNode({ sx, sy });
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
}*/

int findMinNode(TListp* plist)
{
	ITEM* pcur, * pmin;
	int idx = 0;
	for (int i = 1; i < plist->size; i++) {
		pmin = plist->array[idx];
		pcur = plist->array[i];
		if (pcur->Gx + pcur->Hx < pmin->Gx + pmin->Hx)
			idx = i;
	}
	return idx;
}

// 计算欧氏距离
int calcHx(ITEM* it1, int ex, int ey)
{
	return abs(it1->x - ex) + abs(it1->y - ey);
}

int isRoad(int x, int y)
{
	// 不能越界
	if (x < 0 || x >= MAX_COL || y < 0 || y >= MAX_ROW)
		return 0;
	// 等于道路
	if (Maze[y][x] == itROAD)
		return 1;
	// 否则返回0
	return 0;
}

void findPath_astarsearch()
{
	int sx = 1, sy = 1, ex = MAX_COL - 2, ey = MAX_ROW - 2;
	TListp openList;
	TListp closeList;
	int dir[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	initList(&openList, MAX_COL * MAX_ROW);
	initList(&closeList, MAX_COL * MAX_ROW);

	ITEM* pstart = createNode({ sx, sy });
	ITEM* pend;
	ITEM* pcur;
	append(&openList, pstart);

	int found = 0;
	while (openList.size>0) {
		// 在 openList 中查找预估最短路径的节点
		int curidx = findMinNode(&openList);
		pcur = get(&openList, curidx);
		Maze[pcur->x][pcur->y] = itVISITED;
		// 从 openList 中移除该节点
		remove(&openList, curidx);
		// 将该节点加入 closeList 中
		append(&closeList, pcur);

		if (pcur->x == ex && pcur->y == ey) {
			found = 1;
			break;
		}

		for (int i = 0; i < 4; i++) {
			int nx = pcur->x + dir[i][0];
			int ny = pcur->y + dir[i][1];
			if (isRoad(nx, ny)) {
				ITEM* pnext = createNode({ nx, ny });
				pnext->pparent = pcur;
				// Gx为从起点到pnext点的步长
				pnext->Gx = pcur->Gx + 1;
				// Hx为pnext点到终点的欧氏距离
				pnext->Hx = calcHx(pnext, ex, ey);

				// 加入放入可移动的路径
				append(&openList, pnext);
			}
		}
	}

	if (found) {
		pcur = pend;
		while (true) {
			drawCell(pcur->x, pcur->y, RED);
			FlushBatchDraw();
			pcur = getParent(pcur);
			if (pcur == pstart) {
				drawCell(pcur->x, pcur->y, RED);
				FlushBatchDraw();
				break;
			}
		}
	}
}

/*
void A_find()
{
	TList openList;		// 可移动路径（按F值自动排序）
	TList closeList;    // 已走过路径

	// 从目标位置开始寻找起点，之后回溯即可得到起点到终点的路径
	ITEM start, end;
	ITEM pcur = end;
	// 创建高效容器
	ITEM destPos = start;
	// 将目标位置加入 openList
	push(&openList, pcur);
	// 后续迭代进行，根据 A* 寻路规则，利用高效容器寻找最短路径
	while (true) {
		drawCell(pcur.x, pcur.y, RGB(0, 255, 255));
		// 将 openList 中 F 值最小的节点放到 closeList 中
		pcur = cont.getMinNode();
		// 标记节点加入到 openList 中的节点
		Maze[pcur.x][pcur.y] = itVISITED;
		drawCell(pcur.x, pcur.y, RGB(0, 0, 255));
		// 后续 if 将周围通路加入 openList
		if (Maze[pcur.x - 1][pcur.y] == itROAD) {
			Maze[pcur.x - 1][pcur.y] = itVISITED;
			push(&openList, Point2(pcur.x - 1, pcur.y));


			int gVal = 0;
			Node* par = nullptr;
			if (!closeList.empty()) {
				par = *(--closeList.end());
				gVal = par->gVal + 1;
			}
			Node* temp = new Node(_p2.x, _p2.y, gVal, abs(_p2.x - destPos.x) + abs(_p2.y - destPos.y), par);
			if (_p2.x == destPos.x && _p2.y == destPos.y)
				p_destNode = temp;
			openList.insert(temp);
			temp = nullptr;


			drawCell(pcur.x - 1, pcur.y, RGB(0, 255, 255));
			if (start == Point2(pcur.x - 1, pcur.y)) break;
		}
		if (Maze[pcur.x][pcur.y - 1] == itROAD) {
			Maze[pcur.x][pcur.y - 1] = itVISITED;
			push(&openList, Point2(pcur.x, pcur.y - 1));
			drawCell(pcur.x, pcur.y - 1, RGB(0, 255, 255));
			if (start == Point2(pcur.x, pcur.y - 1)) break;
		}
		if (Maze[pcur.x + 1][pcur.y] == itROAD) {
			Maze[pcur.x + 1][pcur.y] = itVISITED;
			push(&openList, Point2(pcur.x + 1, pcur.y));
			drawCell(pcur.x + 1, pcur.y, RGB(0, 255, 255));
			if (start == Point2(pcur.x + 1, pcur.y)) break;
		}
		if (Maze[pcur.x][pcur.y + 1] == itROAD) {
			Maze[pcur.x][pcur.y + 1] = itVISITED;
			push(&openList, Point2(pcur.x, pcur.y + 1));
			drawCell(pcur.x, pcur.y + 1, RGB(0, 255, 255));
			if (start == Point2(pcur.x, pcur.y + 1)) break;
		}
	}

	// 找到起点后，获取高效容器中的起点，回溯得到最短路径
	auto st = cont.getDestNode();
	while (st->parent != nullptr)
	{
		drawCell(st->x, st->y, RGB(255, 255, 0));
		st = st->parent;
		drawCell(st->x, st->y, RGB(255, 0, 0));
	}
}
*/