// �Թ������㷨������ķ�㷨�Թ���Prim��
// 1.���Թ�ȫ����ǽ��
// 2.ѡһ����A��Ϊ·������[���ڵĸ���]������ǽ�����б�
// 3.���б��ﻹ��ǽʱ��
//     1..���б������ѡһ����B(ǽ)��
//     2.�������B(ǽ)Ϊ��㣬ɨ���ĸ�����[���ڵĸ���]�������·�����[����]��ӽ���ʱ�б�
//     3.����ʱ�б��������һ����C��
//     4.�����C��ͨ��
//     5.�Ѹ�B��Ϊͨ·������[���ڵĸ���]������ǽ�����б�
//     6.���б����Ƴ���B��
// Prim�㷨���ǲ��ϵش����п�����ͨ·��λ��������ѡһ���ڶ���
// ֱ��û�п���Ϊͨ·��λ�á�����ʵ�ֹ��̻����൱������Ϊ·�߸�Ȩֵ��Prim�㷨��

#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#include "list.h"
#include "maze.h"
#include "draw_maze.h"

BYTE Maze[MAX_COL][MAX_ROW];

// ��(rx��ry)�ĸ�����[���ڵĸ���]����ǽ��״̬��·������ѡ�б�
// ���ҽ���Щ·���ǽ��״̬��Ϊ����״̬����Ҫ���ڹ���չʾ�� 
void addWall(int rx, int ry, TList* pWallList)
{
    if (rx - 2 >= 0 && Maze[rx - 2][ry] == itWALL) {
        append(pWallList, { rx - 2, ry });
        Maze[rx - 2][ry] = itWAIT;
    }
    if (rx + 2 <= MAX_COL - 1 && Maze[rx + 2][ry] == itWALL) {
        append(pWallList, { rx + 2, ry });
        Maze[rx + 2][ry] = itWAIT;
    }
    if (ry - 2 >= 0 && Maze[rx][ry - 2] == itWALL) {
        append(pWallList, { rx, ry - 2 });
        Maze[rx][ry - 2] = itWAIT;
    }
    if (ry + 2 <= MAX_ROW - 1 && Maze[rx][ry + 2] == itWALL) {
        append(pWallList, { rx, ry + 2 });
        Maze[rx][ry + 2] = itWAIT;
    }
}

// ����·��A(rx, ry)����ҵ���ǽ��·��B����A��B·���м��ǽ���ͨ
// ��A(rx, ry)·����������ĸ������и�ǽ���Ѿ���·�㣬�򽫴�ǽ������б���
// ��ǽ���б������ѡ��һ��������ǽ��ͨ������ΪROAD��
void breakWall(int rx, int ry)
{
    TList list;
    initList(&list, 4);
    // �ĸ������ϣ�[����]��[����]�Ѿ���ROAD����[����]��ӽ���ʱ�б�
    if (rx - 2 >= 0 && Maze[rx - 2][ry] == itROAD)
        append(&list, { rx - 1, ry });
    if (rx + 2 <= MAX_COL - 1 && Maze[rx + 2][ry] == itROAD)
        append(&list, { rx + 1, ry });
    if (ry - 2 >= 0 && Maze[rx][ry - 2] == itROAD)
        append(&list, { rx, ry - 1 });
    if (ry + 2 <= MAX_ROW - 1 && Maze[rx][ry + 2] == itROAD)
        append(&list, { rx, ry + 1 });

    // ��ʱlist�д�����ĸ������ǽ[����]
    if (list.size > 0) {
        // �����ѡһ��ǽ
        int idx = rand() % list.size;
        ITEM wall = get(&list, idx);
        // ����ǽ��ͨ����A(rx, ry) - (wall.x, wally) - ��B����ΪROAD���γ�ͨ·
        Maze[wall.x][wall.y] = itROAD;
        freeList(&list);
    }
}

// �Թ������㷨������ķ�㷨�Թ���Prim��
// Prim�㷨���ǲ��ϵش����п�����ͨ·��λ��������ѡһ���ڶ���
// ֱ��û�п���Ϊͨ·��λ�á�����ʵ�ֹ��̻����൱������Ϊ·�߸�Ȩֵ��Prim�㷨��
void createMaze_prim()
{
    TList wallList;
    initList(&wallList, MAX_COL * MAX_ROW);

    // 1.���Թ�ȫ����ǽ
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            Maze[x][y] = itWALL;
        }
    }

    // 2.ѡһ��·�㣬������Ϊͨ·
    int rx = 1, ry = 1;
    Maze[rx][ry] = itROAD;
    // Ȼ��Ѵ�·���[���ڵĸ���]��ȻΪǽ��·������б�
    addWall(rx, ry, &wallList);

    // 3.���б��ﻹ��ǽ״·��ʱ
    while (wallList.size > 0) {
        // ���б������ѡһ��ǽ״·��wall
        int idx = rand() % wallList.size;
        ITEM wall = get(&wallList, idx);

        // ��wall����[���ڵĸ���]�����һ��ͨ··�㣬������֮��ǽ��ͨ
        breakWall(wall.x, wall.y);
        // ����ǽ״·���Ϊͨ·
        Maze[wall.x][wall.y] = itROAD;

        // ��wall[���ڵĸ���]��ȻΪǽ��·������б� (ע�⣬��Ҫ�ظ����·�㣡)
        addWall(wall.x, wall.y, &wallList);
        // ���б���ɾ���Ƴ���ǽ״·��
        remove(&wallList, idx);

//        drawMaze();
    }

    drawMaze();
//    displayMaze();
    freeList(&wallList);
}

