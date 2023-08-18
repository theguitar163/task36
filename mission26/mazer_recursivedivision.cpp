// �Թ������㷨���ݹ�ָ��㷨��Recursive division��
// �����õݹ�ָ�����Թ����㷨�Ĺ���ԭ�����£�
// 1.�ھ����Թ��У�������㽨���˴˴�ֱ�������ڡ� �������ڽ���ǻ�ҷֳ����ĸ��ڷֿ����ĸ���Сǻ�ҡ� 
// 2.���ѡ���ĸ�ǽ���е���������������ǽ�ڵ�����㴦����һ����Ԫ��Ŀס� 
// ���������ַ�ʽ�ݹ飬ֱ��ÿ���������������е���һ�������Ͼ���һ����Ԫ�Ŀ�ȡ�
#include <conio.h>
#include "maze.h"
#include "draw_maze.h"

// �����(min, max)ȡ�����е�ż��������min��max
// ���޷�ȡ�ú��ʵ�ż�����򷵻�-1
int getEven(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// ȡ������仯��
	int num = rand() % (max - min - 1);
	// ֵӦ����min
	num = min + num + 1;
	// ֵӦС��max
	if (num >= max)
		return -1;
	// ����ż��
	if (num % 2 != 0) {
		// ������1
		if (num + 1 < max)
			return num + 1;
		// ���Լ�1
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	}
	else
		return num;
}

// �����ȡ(min, max)�����е�����������min��max
// ���޷�ȡ�ú��ʵ��������򷵻�-1
int getOdd(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// ȡ������仯��
	int num = rand() % (max - min - 1);
	// ֵӦ����min
	num = min + num + 1;
	// ֵӦС��max
	if (num >= max)
		return -1;
	// ��������
	if (num % 2 != 1) {
		// ������1
		if (num + 1 < max)
			return num + 1;
		// ���Լ�1
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	}
	else
		return num;
}

// �����Թ��ĺ���
void divide(RECT r)
{
	// ��������Ƿ���ֱ�ӷ���
	if (r.left < 0 || r.right < 0 || r.top < 0 || r.bottom < 0) {
		return;
	}
	// ���ûλ���ˣ�ֱ�ӷ���
	// �м��һ������ʱ���Ѿ�û��Ҫ�ٴν���ʮ���з�
	// ��ǿ���з֣��޷������������ӣ��ᵼ������ͬ����
    if (r.right - r.left <= 2 || r.bottom - r.top <= 2) { 
        return;
    }
    // ʮ��Χǽ��Ҫ��Ϊż��
    int wallY = getEven(r.top, r.bottom);
    int wallX = getEven(r.left, r.right);

	// ���������ǽ�Ͽ�����Ҫ�󿪶�λ��������
	int direct[4] = { 1, 1, 1, 1 };
	direct[rand() % 4] = 0;

	// ��������ǽ��
	if (wallX > 0) {
		// ��ע����ǽ��
		for (int i = r.top; i <= r.bottom; i++) {
			Maze[wallX][i] = itWALL;
		}
		// ʮ��ǽ���ϲ�����
		// ��wallYΪ��������ʾ�޺���ǽ�壬ֻ�����Ϸ�����
		if (direct[0]) { 
			int roadY = getOdd(r.top, (wallY < 0)? r.bottom : wallY);
			Maze[wallX][roadY] = itROAD;
		}
		// ʮ��ǽ���²�����
		// ��wallYΪ��������ʾ�޺���ǽ�壬�������·�����
		if (direct[1] && wallY >= 0) { 
			int roadY = getOdd(wallY, r.bottom);
			Maze[wallX][roadY] = itROAD;
		}
	}
	// ���ں���ǽ��
	if (wallY > 0) {
		// ��ע����ǽ��
		for (int i = r.left; i <= r.right; i++) {
			Maze[i][wallY] = itWALL;
		}
		// ʮ��ǽ����࿪��
		// ��wallXΪ��������ʾ������ǽ�壬ֻ������࿪��
		if (direct[2]) { // ��
			int roadX = getOdd(r.left, (wallX < 0) ? r.right : wallX);
			Maze[roadX][wallY] = itROAD;
		}
		// ʮ��ǽ���Ҳ࿪��
		// ��wallYΪ��������ʾ�޺���ǽ�壬�������Ҳ࿪��
		if (direct[3] && wallX >= 0) { // ��
			int roadX = getOdd(wallX, r.right);
			Maze[roadX][wallY] = itROAD;
		}
	}

 //   drawMaze();
    // �����ݹ�
	divide({ r.left, r.top, wallX, wallY });
	divide({ wallX, r.top, r.right, wallY });
	divide({ r.left, wallY, wallX, r.bottom });
	divide({ wallX, wallY, r.right, r.bottom });
}

void createMaze_recursivedivision()
{
    for (int y = 0; y < MAX_ROW; y++) {
        for (int x = 0; x < MAX_COL; x++) {
            if (y == 0 || y == MAX_ROW - 1 || x == 0 || x == MAX_COL - 1)
                Maze[x][y] = itWALL;
            else
                Maze[x][y] = itROAD;
        }
    }
	divide({ 0, 0, MAX_COL - 1, MAX_ROW - 1 });
	drawMaze();
}
