#pragma once

#define ULEN 50
#define PI 3.14159265359
#define BLOCK_COUNT 7
#define INTR(d) (int)round(d)

// �����͵����꣬Ϊ���ⶪʧ����
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

// С�����Ψ������½�Ϊԭ�㣬�߳� 1 - 1 - sqrt(2)
class CTriangleS :public CBlock
{
public:
	CTriangleS(int angle, TPoint pos, COLORREF color);
};

// �������Ψ������½�Ϊԭ�㣬�߳� sqrt(2) - sqrt(2) - 2
class CTriangleM :public CBlock
{
public:
	CTriangleM(int angle, TPoint pos, COLORREF color);
};

// �������Ψ������½�Ϊԭ�㣬�߳� 2 - 2 - 2*sqrt(2)
class CTriangleL :public CBlock
{
public:
	CTriangleL(int angle, TPoint pos, COLORREF color);
};

// �����Ρ������½�Ϊԭ�㣬�߳� 1 - 1 - 1 - 1
class CSquare :public CBlock
{
public:
	CSquare(int angle, TPoint pos, COLORREF color);
};

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramR :public CBlock
{
public:
	CParallelogramR(int angle, TPoint pos, COLORREF color);
};

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
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


