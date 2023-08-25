// 生命游戏。
// 生命游戏也叫康威生命游戏、细胞自动机、元胞自动机等。
// 生命游戏不需要任何玩家，它是一个二维矩形世界，这个世界中的每个方格居住着一个活细胞或为空。
// 一个细胞在下一个时刻的生死取决于相邻八个方格中的活细胞数量，
// 使整个生命世界不至于太过荒凉或拥挤，达成一种动态的平衡。
// 当一个方格没有活细胞时：
//    若周围有 3 个活细胞，则该方格产生一个活细胞（模拟繁殖）。
//  当一个方格存在活细胞时：
//    若周围少于 2 个活细胞，则该方格的细胞死亡（模拟人口稀疏）。
//    若周围有 2 个或 3 个活细胞，保持原样。
//    若周围多于 3 个活细胞，则该方格的细胞死亡（模拟极度拥挤）。
// 约定：世界为 100 x 100 大小。初始状态为世界周边有一圈活细胞。
// 写一个程序，模拟这个细胞世界的繁衍更迭。
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <time.h>

#define MAX_COL 102
#define MAX_ROW 102

int cells[MAX_COL][MAX_ROW];
int cellsTmp[MAX_COL][MAX_ROW];  // 每次迭代临时缓存

void drawCells()
{
	int w = getwidth() / MAX_COL;
	int h = getheight() / MAX_ROW;
	setfillcolor(GREEN);
	// 上下行左右列均不更新
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			// 存活的细胞填充绿色
			if (cells[i][j] == 1)
				fillrectangle(i * w, j * h, i * w + w, j * h + h);
			else
				rectangle(i * w, j * h, i * w + w, j * h + h);
		}
	}
}
// 初始化细胞
void initCells()
{
	// 初始化
	memset(cells, 0, MAX_COL * MAX_ROW * sizeof(int));
	// 随机在内圈撒播存货的细胞，概率为50%
	srand(time(NULL));
	for (int i = 1; i < MAX_COL - 1; i++) {
		for (int j = 1; j < MAX_ROW - 1; j++) {
			cells[i][j] = rand() % 2;
		}
	}
}

// 计算当前细胞周围存活细胞个数
// 为计算简便，上下行、左右列不参与计算
int livecount(int x, int y)
{
	// 超界返回0
	if (x < 1 || x >= MAX_COL - 1 || y < 1 || y >= MAX_ROW - 1)
		return 0;
	// 周围8各单元格状态，1为存活、0为死亡。累加即为周边存活细胞个数
	int livecount = cells[x - 1][y - 1] + cells[x][y - 1] + cells[x + 1][y - 1]
		          + cells[x - 1][y]                       + cells[x + 1][y]
		          + cells[x - 1][y + 1] + cells[x][y + 1] + cells[x + 1][y + 1];
	return livecount;
}
// 繁殖细胞
void breedCells()
{
	for (int x = 1; x < MAX_COL - 1; x++) {
		for (int y = 1; y < MAX_ROW - 1; y++) {
			// 未避免在cells中跟新存活状态相互影响，先更新cellsTmp缓存
			switch (livecount(x, y)) {
			case 3:  // 周边存活细胞为3，则当前细胞必为存活
				cellsTmp[x][y] = 1;
				break;
			case 2:  // 周边存活细胞为2，则当前细胞状态不变
				cellsTmp[x][y] = cells[x][y];
				break;
			default: // 其他情况，则当前细胞死亡
				cellsTmp[x][y] = 0;
			}
		}
	}
	// 将缓存复制到cells
	memcpy(cells, cellsTmp, MAX_COL * MAX_ROW * sizeof(int));
}

int main()
{
	initgraph(MAX_COL*6, MAX_ROW*6);
	initCells();
	BeginBatchDraw();
	ExMessage m;
	while (!peekmessage(&m, EX_KEY)) {
		cleardevice();      // ！！若不清屏则无法看出其繁殖状态
		drawCells();        // 绘制当前细胞世界状态
		FlushBatchDraw();   // 批量绘制结果
		Sleep(100);
		breedCells();       // 繁殖细胞
	}
	EndBatchDraw();
	closegraph();
}