#pragma once

#include <easyx.h>

#define MAX_COL 55
#define MAX_ROW 55

#define itROAD 0
#define itWALL 1
#define itWAIT 2

extern BYTE Maze[MAX_COL][MAX_ROW];

void createMaze_prim();
void createMaze_deepsearch();
void createMaze_recursivedivision();