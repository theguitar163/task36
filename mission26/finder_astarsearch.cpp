// �Թ�Ѱ·�㷨��������ȱ���
#include <stdio.h>
#include <conio.h>
#include "maze.h"
#include "list.h"
#include "tree.h"
#include "draw_maze.h"


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
	if (Maze[x][y] == itROAD)
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
	ITEM* pend = NULL;
	ITEM* pcur;
	append(&openList, pstart);
	int found = 0;
	while (openList.size>0) {
		// �� openList �в���Ԥ�����·���Ľڵ�
		int curidx = findMinNode(&openList);
		pcur = get(&openList, curidx);
		Maze[pcur->x][pcur->y] = itVISITED;

		drawCell(pcur->x, pcur->y, YELLOW);
		FlushBatchDraw();
		// �� openList ���Ƴ��ýڵ�
		remove(&openList, curidx);
		// ���ýڵ���� closeList ��
		append(&closeList, pcur);

		if (pcur->x == ex && pcur->y == ey) {
			pend = pcur;
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
				Maze[nx][ny] = itVISITED;

				drawCell(nx, ny, YELLOW);
				FlushBatchDraw();
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
	// �ͷ����д����Ľڵ��ڴ�
	for (int i = 0; i < openList.size; i++)
		free(openList.array[i]);
	for (int i = 0; i < closeList.size; i++)
		free(closeList.array[i]);
	// �ͷ��б���
	freeList(&openList);
	freeList(&closeList);
}
