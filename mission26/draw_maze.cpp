// 迷宫生成算法：深度遍历图（Deep traversal graph）
#include <stdio.h>
#include <conio.h>
#include <easyx.h>

#include "maze.h"
#include "draw_maze.h"

void displayMaze()
{
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++)
			printf("%d ", Maze[j][i]);
		printf("\n");
	}
	printf("\n");
	_getch();
}

void drawMaze()
{
	for (int y = 0; y < MAX_ROW; y++) {
		for (int x = 0; x < MAX_COL; x++) {
			if (Maze[x][y] == itWALL) {
				drawCell(x, y, COLOR_WALL);
			}
			else if (Maze[x][y] == itROAD) {
				drawCell(x, y, COLOR_ROAD);
			}
			else if (Maze[x][y] == itWAIT) {
				drawCell(x, y, COLOR_WAIT);
			}
		}
	}
	FlushBatchDraw();
}

void clearScreen(COLORREF color)
{
	for (int y = 0; y < MAX_ROW; y++) {
		for (int x = 0; x < MAX_COL; x++) {
			drawCell(x, y, color);
		}
	}
}

void drawCell(int x, int y, COLORREF color)
{
	setfillcolor(color); 
	setlinecolor(BLACK);
	fillrectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE + x * CELL_SIZE, CELL_SIZE + y * CELL_SIZE);
}
