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

// 随机获取区间中的偶数
int getEven(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// Divide both by 2 to ensure the range
	int num = rand() % (max - min - 1);
	num = min + num + 1;
	if (num >= max)
		return -1;
	if (num % 2 != 0)
		if (num + 1 < max)
			return num + 1;
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	else
		return num;
}

// 随机获取区间中的偶数
int getOdd(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// Divide both by 2 to ensure the range
	int num = rand() % (max - min - 1);
	num = min + num + 1;
	if (num >= max)
		return -1;
	if (num % 2 != 1)
		if (num + 1 < max)
			return num + 1;
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	else
		return num;
}

// 生成迷宫的函数
void divide(RECT r)
{
	// 如果参数非法，直接返回
	if (r.left < 0 || r.right < 0 || r.top < 0 || r.bottom < 0) {
		return;
	}
	// 如果没位置了，直接返回
	// 中间空一格以下时，已经没必要再次进行十字切分
	// 若强行切分，无法开出三个口子，会导致死胡同出现
    if (r.right - r.left <= 2 || r.bottom - r.top <= 2) { 
        return;
    }
    // 十字围墙，要求为偶数
    int wallY = getEven(r.top, r.bottom);
    int wallX = getEven(r.left, r.right);

	// 随机在三面墙上开洞，要求开洞位置是奇数
	int direct[4] = { 1, 1, 1, 1 };
	direct[rand() % 4] = 0;

	// 存在纵向墙体
	if (wallX > 0) {
		// 标注纵向墙体
		for (int i = r.top; i <= r.bottom; i++) {
			Maze[wallX][i] = itWALL;
		}
		// 十字墙体上部开口
		// 若wallY为负数，表示无横向墙体，只需在上方开口
		if (direct[0]) { 
			int roadY = getOdd(r.top, (wallY < 0)? r.bottom : wallY);
			Maze[wallX][roadY] = itROAD;
		}
		// 十字墙体下部开口
		// 若wallY为负数，表示无横向墙体，不用在下方开口
		if (direct[1] && wallY >= 0) { 
			int roadY = getOdd(wallY, r.bottom);
			Maze[wallX][roadY] = itROAD;
		}
	}
	// 存在横向墙体
	if (wallY > 0) {
		// 标注纵向墙体
		for (int i = r.left; i <= r.right; i++) {
			Maze[i][wallY] = itWALL;
		}
		// 十字墙体左侧开口
		// 若wallX为负数，表示无纵向墙体，只需在左侧开口
		if (direct[2]) { // 左
			int roadX = getOdd(r.left, (wallX < 0) ? r.right : wallX);
			Maze[roadX][wallY] = itROAD;
		}
		// 十字墙体右侧开口
		// 若wallY为负数，表示无横向墙体，不用在右侧开口
		if (direct[3] && wallX >= 0) { // 右
			int roadX = getOdd(wallX, r.right);
			Maze[roadX][wallY] = itROAD;
		}
	}

    drawMaze();
    // 继续递归
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
}
