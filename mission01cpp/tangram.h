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

void rotatePoint(TPoint* pp, int angle);

typedef enum {
	eTriangleS, eTriangleM, eTriangleL, eSquare, eParallelogramR, eParallelogramL
} TBlockType;

class CBlock
{
protected:
	TBlockType type;
	int m_angle;
	TPoint m_pos;
	COLORREF m_color;
	int m_ptCount;
	TPoint m_points[4];

	void moveBlock(TPoint pos);
	void rotateBlock(int angle);

public:
	CBlock(int angle, TPoint pos, COLORREF color);
	void setColor(COLORREF color);
	void drawBlock(TPoint pos);
};

// 小三角形◣，左下角为原点，边长 1 - 1 - sqrt(2)
class CTriangleS :public CBlock
{
public:
	CTriangleS(int angle, TPoint pos, COLORREF color);
};

// 中三角形◣，左下角为原点，边长 sqrt(2) - sqrt(2) - 2
class CTriangleM :public CBlock
{
public:
	CTriangleM(int angle, TPoint pos, COLORREF color);
};

// 大三角形◣，左下角为原点，边长 2 - 2 - 2*sqrt(2)
class CTriangleL :public CBlock
{
public:
	CTriangleL(int angle, TPoint pos, COLORREF color);
};

// 正方形■，左下角为原点，边长 1 - 1 - 1 - 1
class CSquare :public CBlock
{
public:
	CSquare(int angle, TPoint pos, COLORREF color);
};

// 右向平行四边形◢■◤，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramR :public CBlock
{
public:
	CParallelogramR(int angle, TPoint pos, COLORREF color);
};

// 左向平行四边形◥■◣，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramL :public CBlock
{
public:
	CParallelogramL(int angle, TPoint pos, COLORREF color);
};

class CTangram
{
	CBlock* m_blocks[BLOCK_COUNT];
public:
	CTangram(CBlock* bs[BLOCK_COUNT]);
	~CTangram();
	void drawTangram(TPoint pos);
};


