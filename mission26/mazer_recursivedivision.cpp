// 迷宫生成算法：递归分割算法（Recursive division）
// 可以用递归分割法创建迷宫，算法的工作原理如下：
// 1.开始创建迷宫，使整个空间没有壁，我们称之为“室”。
// 2.在随机位置生成壁将室分割为两个子室，并在壁上随机开孔，使子室联通。
// 3.重复步骤2，直到所有子室全部不可分割（即子室某一个维度等于1）。
// 例如，在矩形迷宫中，在任意点建立彼此垂直的两个壁。 这两个壁将大腔室分成由四个壁分开的四个较小腔室。 
// 随机选择四个墙壁中的三个，并在三个墙壁的随机点处打开一个单元格的孔。 
// 继续以这种方式递归，直到每个室在两个方向中的任一个方向上具有一个单元的宽度。
// 因为没有起点的概念，所以没有起点位置参数，取而代之的是最小面积参数，
// 这个参数是我用来制造“房间”的，当子空间面积小于这个值就会强制停止分割，
// 这样就可以造出随机的房间，并在一定程度上避免该算法过于弱智的“一条长长的死胡同”问题。
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

// 随机获取区间中的偶数
int getEven(int min, int max) 
{
    return ((rand() % (max - min) + min + 1) | 1) - 1;
}

// 随机获取区间中的奇数
int getOdd(int min, int max) 
{
	return (rand() % (max - min) + min) | 1;
}

// 生成迷宫的函数
void generate(RECT r) 
{
    // 如果没位置了，直接return
    if (r.right - r.left <= 2 || r.bottom - r.top <= 2) { 
        return;
    }
    // 生成十字围墙，要求为偶数
    int wallY = getEven(r.top, r.bottom);
    int wallX = getEven(r.left, r.right);
    for (int i = r.left; i < r.right; i++) {
        Maze[wallY][i] = itWALL;
    }
    for (int i = r.top; i < r.bottom; i++) {
        Maze[i][wallX] = itWALL;
    }
    // 随机在三面墙上开洞，要求开洞位置是奇数
    int direct[4] = { 1, 1, 1, 1 };
    direct[rand() % 4] = 0;
    if (direct[0]) { // 上
        int roadY = getOdd(r.top, wallY);
        Maze[roadY][wallX] = itROAD;
    }
    if (direct[1]) { // 下
        int roadY = getOdd(wallY, r.bottom);
        Maze[roadY][wallX] = itROAD;
    }
    if (direct[2]) { // 左
        int roadX = getOdd(r.left, wallX);
        Maze[wallY][roadX] = itROAD;
    }
    if (direct[3]) { // 右
        int roadX = getOdd(wallX, r.right);
        Maze[wallY][roadX] = itROAD;
    }
    drawMaze();
    // 继续递归
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
	// 间隔大于 1 时可分割
	if (right - left > 1 && bottom - top > 1)
	{
		int i = 0;
		// 要求分割点 px,py 为偶数
		int px = ((rand() % (right - left) + left + 1) | 1) - 1;
		int py = ((rand() % (bottom - top) + top + 1) | 1) - 1;
		while (px + i <= right || px - i >= left || py + i <= bottom || py - i >= top) {
			if (px + i <= right) { Maze[px + i][py] = itWALL; }
			if (px - i >= left) { Maze[px - i][py] = itWALL; }
			if (py + i <= bottom) { Maze[px][py + i] = itWALL; }
			if (py - i >= top) { Maze[px][py - i] = itWALL; }
			++i;
		}
		// 随机在三面墙上开洞，要求开洞位置是奇数
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

		// 递归分割
		Division_generator(left, px - 1, top, py - 1);
		Division_generator(px + 1, right, top, py - 1);
		Division_generator(left, px - 1, py + 1, bottom);
		Division_generator(px + 1, right, py + 1, bottom);
	}
}*/