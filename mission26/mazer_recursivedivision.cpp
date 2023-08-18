// 迷宫生成算法：递归分割算法（Recursive division）
// 可以用递归分割法创建迷宫，算法的工作原理如下：
// 1.在矩形迷宫中，在任意点建立彼此垂直的两个壁。 这两个壁将大腔室分成由四个壁分开的四个较小腔室。 
// 2.随机选择四个墙壁中的三个，并在三个墙壁的随机点处各打开一个单元格的孔。 
// 继续以这种方式递归，直到每个室在两个方向中的任一个方向上具有一个单元的宽度。
#include <conio.h>
#include "maze.h"
#include "draw_maze.h"

// 随机获(min, max)取区间中的偶数，不含min和max
// 若无法取得合适的偶数，则返回-1
int getEven(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// 取得随机变化量
	int num = rand() % (max - min - 1);
	// 值应大于min
	num = min + num + 1;
	// 值应小于max
	if (num >= max)
		return -1;
	// 若非偶数
	if (num % 2 != 0) {
		// 尝试增1
		if (num + 1 < max)
			return num + 1;
		// 尝试减1
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	}
	else
		return num;
}

// 随机获取(min, max)区间中的奇数，不含min和max
// 若无法取得合适的奇数，则返回-1
int getOdd(int min, int max)
{
	if (max - min <= 1)
		return -1;
	// 取得随机变化量
	int num = rand() % (max - min - 1);
	// 值应大于min
	num = min + num + 1;
	// 值应小于max
	if (num >= max)
		return -1;
	// 若非奇数
	if (num % 2 != 1) {
		// 尝试增1
		if (num + 1 < max)
			return num + 1;
		// 尝试减1
		else if (num - 1 > min)
			return num - 1;
		else
			return -1;
	}
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

 //   drawMaze();
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
	drawMaze();
}
