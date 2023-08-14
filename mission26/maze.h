#pragma once

#include <easyx.h>

#define MAX_COL 15
#define MAX_ROW 15

#define itROAD 0
#define itWALL 1
#define itWAIT 2

extern BYTE Maze[MAX_COL][MAX_ROW];

void createMaze_prim();