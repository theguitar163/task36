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

#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "list.h"
#include "draw_maze.h"
#include "maze.h"

int Maze[MAX_COL][MAX_ROW];//0���������1����ǽ�ڣ�3����ѡ��·�㣨���ǲ�û���õ���

TList list;//��ʱ���б�
TList list2;//��ʱ���б�


bool prim_main()
{

}
void through(int x, int y)
{
	list.clear();
	list2.clear();
	//��
	if (y - 2 >= 0 && Maze[x][y - 2] == itWALL) {
		block* a = new block(x, y - 1);
		block* aa = new block(x, y - 2);
		list.push_back(a);
		list2.push_back(aa);
	}
	//��
	if (y + 2 <= MAX_ROW - 1 && Maze[x][y + 2] == itWALL) {
		block* b = new block(x, y + 1);
		block* bb = new block(x, y + 2);
		list.push_back(b);
		list2.push_back(bb);
	}
	//��
	if (x - 2 >= 0 && Maze[x - 2][y] == itWALL) {
		block* c = new block(x - 1, y);
		block* cc = new block(x - 2, y);
		list.push_back(c);
		list2.push_back(cc);
	}
	//��
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
		/*��x��y��B��ͨ*/
		Maze[B->x][B->y] = itROAD;
		/*��ѡ��·���Ϊ·���������ѡ�б�*/
		Maze[BB->x][BB->y] = itROAD;
		wallList.push_back(BB);

	}
}
bool check(int x, int y) {
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

int main1() {

	TList wallList;
	initList(&wallList, MAX_COL * MAX_ROW);

	for (int y = 0; y < MAX_ROW; y++) {
		for (int x = 0; x < MAX_COL; x++) {
			Maze[x][y] = itWALL;
		}
	}
	/*���ѡһ��·�㣬�������·*/
	Maze[1][1] = itROAD;
	/*���õ�����ѡ�б�*/
	append(&wallList, { 1, 1 });

	/*��ʼ��ѭ��*/

	while (wallList.size() > 0) {
		/*�Ӵ�ѡ�б�ѡ���һ��·��A*/
		int AIndexea = wallList.size() - 1;
		block* A = wallList[AIndexea];
		/*����Ƿ����������ͬ,�������������ͬ,�����*/
		while (!check(A->x, A->y)) {
			A = wallList[AIndexea];
			if (AIndexea == 0) return 1;//����Թ����귵��1
			--AIndexea;
		}
		/*��A��������һ�������Ϊǽ�ڵ�·���ͨ,����ѡ��·���Ϊ·,�������ѡ�б�*/
		through(A->x, A->y);
	}

	display();

	free(&wallList);
	list.clear();
	list2.clear();
}