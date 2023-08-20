// �Թ�Ѱ·�㷨��������ȱ���
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
    // �Ƿ�ɹ���־
    int flag = 0;
    while (queue.size > 0) {
        pop_queue(&queue, &pcur);

        drawCell(pcur->x, pcur->y, YELLOW);
        FlushBatchDraw();
        // �Ƿ����յ�
        if (pcur->x == ex && pcur->y == ey) {
            pend = pcur;
            flag = 1;        //�ѵ����յ��־
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
    // �ɹ������·��
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

// ����ŷ�Ͼ���
int calcHx(ITEM* it1, int ex, int ey)
{
	return abs(it1->x - ex) + abs(it1->y - ey);
}

int isRoad(int x, int y)
{
	// ����Խ��
	if (x < 0 || x >= MAX_COL || y < 0 || y >= MAX_ROW)
		return 0;
	// ���ڵ�·
	if (Maze[y][x] == itROAD)
		return 1;
	// ���򷵻�0
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
		// �� openList �в���Ԥ�����·���Ľڵ�
		int curidx = findMinNode(&openList);
		pcur = get(&openList, curidx);
		Maze[pcur->x][pcur->y] = itVISITED;
		// �� openList ���Ƴ��ýڵ�
		remove(&openList, curidx);
		// ���ýڵ���� closeList ��
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
				// GxΪ����㵽pnext��Ĳ���
				pnext->Gx = pcur->Gx + 1;
				// HxΪpnext�㵽�յ��ŷ�Ͼ���
				pnext->Hx = calcHx(pnext, ex, ey);

				// ���������ƶ���·��
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
	TList openList;		// ���ƶ�·������Fֵ�Զ�����
	TList closeList;    // ���߹�·��

	// ��Ŀ��λ�ÿ�ʼѰ����㣬֮����ݼ��ɵõ���㵽�յ��·��
	ITEM start, end;
	ITEM pcur = end;
	// ������Ч����
	ITEM destPos = start;
	// ��Ŀ��λ�ü��� openList
	push(&openList, pcur);
	// �����������У����� A* Ѱ·�������ø�Ч����Ѱ�����·��
	while (true) {
		drawCell(pcur.x, pcur.y, RGB(0, 255, 255));
		// �� openList �� F ֵ��С�Ľڵ�ŵ� closeList ��
		pcur = cont.getMinNode();
		// ��ǽڵ���뵽 openList �еĽڵ�
		Maze[pcur.x][pcur.y] = itVISITED;
		drawCell(pcur.x, pcur.y, RGB(0, 0, 255));
		// ���� if ����Χͨ·���� openList
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

	// �ҵ����󣬻�ȡ��Ч�����е���㣬���ݵõ����·��
	auto st = cont.getDestNode();
	while (st->parent != nullptr)
	{
		drawCell(st->x, st->y, RGB(255, 255, 0));
		st = st->parent;
		drawCell(st->x, st->y, RGB(255, 0, 0));
	}
}
*/