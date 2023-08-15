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

#include "list.h"
#include "maze.h"
#include "draw_maze.h"

extern BYTE Maze[MAX_COL][MAX_ROW];

void through(int x, int y, TList* pRoadList)
{
	TList list;
	initList(&list, 4);
	//上
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		append(&list, { x, y - 2 });
	}
	//下
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		append(&list, { x, y + 2 });
	}
	//左
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		append(&list, { x - 2, y });
	}
	//右
	if (x + 2 <= MAX_COL - 1 && Maze[x + 2][y] == itWALL) {
		append(&list, { x + 2, y });
	}
	if (list.size > 0) {
		int idx = rand() % list.size;
		ITEM it = get(&list, idx);
		// 将x，y与B打通
		Maze[it.x][it.y] = itROAD;
		// 将选中路点变为路，并加入待选列表
		Maze[x + (it.x - x) / 2][y + (it.y - y) / 2] = itROAD;
		append(pRoadList, it);
	}
	freeList(&list);
}

bool check(int x, int y)
{
	bool temp = 0;
	//上
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		temp = 1;
	}
	//下
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		temp = 1;
	}
	//左
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		temp = 1;
	}
	//右
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

	/*随机选一个路点，将它变成路*/
	Maze[1][1] = itROAD;
	// 将该点加入待选列表
	append(&roadList, { 1, 1 });

	int done = 0;
	while (roadList.size > 0) {
		// 从待选列表选最后一个路点A
		int idx = roadList.size - 1;
		ITEM road = get(&roadList, idx);
		// 检查是否进入了死胡同,如果进入了死胡同,则回溯
		while (!check(road.x, road.y)) {
			idx--;
			// 如果迷宫走完返回1
			if (idx == 0) {
				done = 1;
				break;
			}
			road = get(&roadList, idx);
		}
		if (done) break;
		// 将A与它四周一个随机的为墙壁的路点打通,
		// 并将选中路点变为路,并加入待选列表
		through(road.x, road.y, &roadList);
	}
	drawMaze();
	freeList(&roadList);
}