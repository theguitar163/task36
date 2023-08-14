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

#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "list.h"
#include "draw_maze.h"
#include "maze.h"

int Maze[MAX_COL][MAX_ROW];//0代表空气，1代表墙壁，3代表选中路点（但是并没有用到）

TList list;//临时的列表
TList list2;//临时的列表


bool prim_main()
{

}
void through(int x, int y)
{
	list.clear();
	list2.clear();
	//上
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		block* a = new block(x, y - 1);
		block* aa = new block(x, y - 2);
		list.push_back(a);
		list2.push_back(aa);
	}
	//下
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		block* b = new block(x, y + 1);
		block* bb = new block(x, y + 2);
		list.push_back(b);
		list2.push_back(bb);
	}
	//左
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		block* c = new block(x - 1, y);
		block* cc = new block(x - 2, y);
		list.push_back(c);
		list2.push_back(cc);
	}
	//右
	if (x + 2 <= MAX_COL - 1 && Maze[x + 2][y] == itWALL) {
		block* d = new block(x + 1, y);
		block* dd = new block(x + 2, y);
		list.push_back(d);
		list2.push_back(dd);
	}
	if (list.size() != 0) {
		int BIndexea = rand() % list.size();
		block* B = list[BIndexea];
		block* BB = list2[BIndexea];
		/*将x，y与B打通*/
		Maze[B->x][B->y] = itROAD;
		/*将选中路点变为路，并加入待选列表*/
		Maze[BB->x][BB->y] = itROAD;
		wallList.push_back(BB);

	}
}
bool check(int x, int y) {
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

int main1() {

	TList wallList;
	initList(&wallList, MAX_COL * MAX_ROW);

	for (int y = 0; y < MAX_ROW; y++) {
		for (int x = 0; x < MAX_COL; x++) {
			Maze[x][y] = itWALL;
		}
	}
	/*随机选一个路点，将它变成路*/
	Maze[1][1] = itROAD;
	/*将该点加入待选列表*/
	append(&wallList, { 1, 1 });

	/*开始主循环*/

	while (wallList.size() > 0) {
		/*从待选列表选最后一个路点A*/
		int AIndexea = wallList.size() - 1;
		block* A = wallList[AIndexea];
		/*检查是否进入了死胡同,如果进入了死胡同,则回溯*/
		while (!check(A->x, A->y)) {
			A = wallList[AIndexea];
			if (AIndexea == 0) return 1;//如果迷宫走完返回1
			--AIndexea;
		}
		/*将A与它四周一个随机的为墙壁的路点打通,并将选中路点变为路,并加入待选列表*/
		through(A->x, A->y);
	}

	display();

	free(&wallList);
	list.clear();
	list2.clear();
}