// �Թ������㷨���ݹ�ָ��㷨��Recursive division��
// �����õݹ�ָ�����Թ����㷨�Ĺ���ԭ�����£�
// 1.��ʼ�����Թ���ʹ�����ռ�û�бڣ����ǳ�֮Ϊ���ҡ���
// 2.�����λ�����ɱڽ��ҷָ�Ϊ�������ң����ڱ���������ף�ʹ������ͨ��
// 3.�ظ�����2��ֱ����������ȫ�����ɷָ������ĳһ��ά�ȵ���1����
// ���磬�ھ����Թ��У�������㽨���˴˴�ֱ�������ڡ� �������ڽ���ǻ�ҷֳ����ĸ��ڷֿ����ĸ���Сǻ�ҡ� 
// ���ѡ���ĸ�ǽ���е���������������ǽ�ڵ�����㴦��һ����Ԫ��Ŀס� 
// ���������ַ�ʽ�ݹ飬ֱ��ÿ���������������е���һ�������Ͼ���һ����Ԫ�Ŀ�ȡ�
// ��Ϊû�����ĸ������û�����λ�ò�����ȡ����֮������С���������
// ������������������조���䡱�ģ����ӿռ����С�����ֵ�ͻ�ǿ��ֹͣ�ָ
// �����Ϳ����������ķ��䣬����һ���̶��ϱ�����㷨�������ǵġ�һ������������ͬ�����⡣
#include <conio.h>
#include "maze.h"
#include "draw_maze.h"

void divide(RECT r)
{
	int w = r.right - r.left;
	int h = r.bottom - r.top;
	if (w < 3 || h < 3)
		return;
	int cx = r.left + rand() % w;
	int cy = r.top + rand() % h;

	for (int y = 0; y <= h; y++)
		Maze[cx][r.top + y] = itWALL;
	for (int x = 0; x <= w; x++)
		Maze[r.left + x][cy] = itWALL;
	drawMaze();
	_getch();
	divide({ r.left, r.top, cx, cy });
//	divide({ cx, r.top, r.right, cy });
}

// �����ȡ�����е�ż��
int getEven(int min, int max) 
{
    return ((rand() % (max - min) + min + 1) | 1) - 1;
}

// �����ȡ�����е�����
int getOdd(int min, int max) 
{
	return (rand() % (max - min) + min) | 1;
}

// �����Թ��ĺ���
void generate(RECT r) 
{
    // ���ûλ���ˣ�ֱ��return
    if (r.right - r.left <= 2 || r.bottom - r.top <= 2) { 
        return;
    }
    // ����ʮ��Χǽ��Ҫ��Ϊż��
    int wallY = getEven(r.top, r.bottom);
    int wallX = getEven(r.left, r.right);
    for (int i = r.left; i < r.right; i++) {
        Maze[wallY][i] = itWALL;
    }
    for (int i = r.top; i < r.bottom; i++) {
        Maze[i][wallX] = itWALL;
    }
    // ���������ǽ�Ͽ�����Ҫ�󿪶�λ��������
    int direct[4] = { 1, 1, 1, 1 };
    direct[rand() % 4] = 0;
    if (direct[0]) { // ��
        int roadY = getOdd(r.top, wallY);
        Maze[roadY][wallX] = itROAD;
    }
    if (direct[1]) { // ��
        int roadY = getOdd(wallY, r.bottom);
        Maze[roadY][wallX] = itROAD;
    }
    if (direct[2]) { // ��
        int roadX = getOdd(r.left, wallX);
        Maze[wallY][roadX] = itROAD;
    }
    if (direct[3]) { // ��
        int roadX = getOdd(wallX, r.right);
        Maze[wallY][roadX] = itROAD;
    }
    drawMaze();
    // �����ݹ�
    generate({ r.left, r.top, wallX, wallY });
    generate({ wallX, r.top, r.right, wallY });
    generate({ r.left, wallY, wallX, r.bottom });
    generate({ wallX, wallY, r.right, r.bottom });
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
    //	divide({ 1, 1, MAX_COL - 1, MAX_ROW - 1 });
    generate({ 1, 1, MAX_COL - 1, MAX_ROW - 1 });
}


/*void Division_generator(int left, int right, int top, int bottom)
{
	// ������� 1 ʱ�ɷָ�
	if (right - left > 1 && bottom - top > 1)
	{
		int i = 0;
		// Ҫ��ָ�� px,py Ϊż��
		int px = ((rand() % (right - left) + left + 1) | 1) - 1;
		int py = ((rand() % (bottom - top) + top + 1) | 1) - 1;
		while (px + i <= right || px - i >= left || py + i <= bottom || py - i >= top) {
			if (px + i <= right) { Maze[px + i][py] = itWALL; }
			if (px - i >= left) { Maze[px - i][py] = itWALL; }
			if (py + i <= bottom) { Maze[px][py + i] = itWALL; }
			if (py - i >= top) { Maze[px][py - i] = itWALL; }
			++i;
		}
		// ���������ǽ�Ͽ�����Ҫ�󿪶�λ��������
		int dir[4] = {0,1,2,3};
		std::random_shuffle(dir.begin(), dir.end());
		for (int i = 0; i < 3; ++i)
		{
			if (dir[i] == 0)
			{
				int xx = (rand() % (px - left) + left) | 1;
	    		Maze[xx][py] = itROAD;
			}
			else if (dir[i] == 1)
			{
				int xx = (rand() % (right - px) + px) | 1;
				Maze[xx][py] = itROAD;
			}
			else if (dir[i] == 2)
			{
				int yy = (rand() % (py - top) + top) | 1;
				Maze[px][yy] = itROAD;
			}
			else if (dir[i] == 3)
			{
				int yy = (rand() % (bottom - py) + py) | 1;
				Maze[px][yy] = itROAD;
			}
		}

		// �ݹ�ָ�
		Division_generator(left, px - 1, top, py - 1);
		Division_generator(px + 1, right, top, py - 1);
		Division_generator(left, px - 1, py + 1, bottom);
		Division_generator(px + 1, right, py + 1, bottom);
	}
}*/