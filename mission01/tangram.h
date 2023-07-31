#pragma once
#define ULEN 50
#define PI 3.14159265359
#define BLOCK_COUNT 7
#define INTR(d) (int)round(d)

// 浮点型点坐标，为避免丢失精度
typedef struct {
	double x;
	double y;
} TPoint;

typedef enum {
	eTriangleS, eTriangleM, eTriangleL, eSquare, eParallelogramR, eParallelogramL
} TBlockType;

typedef struct {
	TBlockType type;
	int angle;
	double offx, offy;
	COLORREF color;
	int pcount;
	TPoint points[4];
} TBlock;

typedef TBlock TTangram[BLOCK_COUNT];

void drawTangram(TTangram tgram, TPoint pos);
void drawBlock(TBlock block, TPoint pos);