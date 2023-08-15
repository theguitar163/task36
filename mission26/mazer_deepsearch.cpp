// �Թ������㷨����ȱ���ͼ��Deep traversal graph��
// 1.�������Ϊ��ǰ�񲢱��
// 2.��������δ��ǵĸ�ʱ
// ����1.�����ǰ����δ��ǵ��ڸ�
// ��������1.���ѡ��һ��δ��ǵ��ڸ�
// ��������2.����ǰ����ջ
// ��������3.�Ƴ���ǰ�����ڸ��ǽ
// ��������4.����ڸ�������Ϊ��ǰ��
// ����2.��֮�����ջ����
// ��������1.ջ���ĸ��ӳ�ջ
// ��������2.�����Ϊ��ǰ��
// ����3.��֮�����ѡ��һ������Ϊ��ǰ�񲢱��
// ͨ�׵�˵�����Ǵ���㿪ʼ����ߣ��߲�ͨ�˾�ԭ·���أ�����һ�����ߵĵط��ٿ�ʼ����ߡ�

#include "list.h"
#include "maze.h"
#include "draw_maze.h"

extern BYTE Maze[MAX_COL][MAX_ROW];

void through(int x, int y, TList* pRoadList)
{
	TList list;
	initList(&list, 4);
	//��
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		append(&list, { x, y - 2 });
	}
	//��
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		append(&list, { x, y + 2 });
	}
	//��
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		append(&list, { x - 2, y });
	}
	//��
	if (x + 2 <= MAX_COL - 1 && Maze[x + 2][y] == itWALL) {
		append(&list, { x + 2, y });
	}
	if (list.size > 0) {
		int idx = rand() % list.size;
		ITEM it = get(&list, idx);
		// ��x��y��B��ͨ
		Maze[it.x][it.y] = itROAD;
		// ��ѡ��·���Ϊ·���������ѡ�б�
		Maze[x + (it.x - x) / 2][y + (it.y - y) / 2] = itROAD;
		append(pRoadList, it);
	}
	freeList(&list);
}

bool check(int x, int y)
{
	bool temp = 0;
	//��
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		temp = 1;
	}
	//��
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		temp = 1;
	}
	//��
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		temp = 1;
	}
	//��
	if (x + 2 <= MAX_COL - 1 && Maze[x + 2][y] == itWALL) {
		temp = 1;
	}
	return temp;
}

void createMaze_deepsearch()
{
	TList roadList;
	initList(&roadList, MAX_COL * MAX_ROW);

	for (int y = 0; y < MAX_ROW; y++) {
		for (int x = 0; x < MAX_COL; x++) {
			Maze[x][y] = itWALL;
		}
	}

	/*���ѡһ��·�㣬�������·*/
	Maze[1][1] = itROAD;
	// ���õ�����ѡ�б�
	append(&roadList, { 1, 1 });

	int done = 0;
	while (roadList.size > 0) {
		// �Ӵ�ѡ�б�ѡ���һ��·��A
		int idx = roadList.size - 1;
		ITEM road = get(&roadList, idx);
		// ����Ƿ����������ͬ,�������������ͬ,�����
		while (!check(road.x, road.y)) {
			idx--;
			// ����Թ����귵��1
			if (idx == 0) {
				done = 1;
				break;
			}
			road = get(&roadList, idx);
		}
		if (done) break;
		// ��A��������һ�������Ϊǽ�ڵ�·���ͨ,
		// ����ѡ��·���Ϊ·,�������ѡ�б�
		through(road.x, road.y, &roadList);
	}
	drawMaze();
	freeList(&roadList);
}