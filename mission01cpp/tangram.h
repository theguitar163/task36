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

class CBlock
{
protected:
	TBlockType type;
	int angle;
	TPoint m_pos;
	COLORREF m_color;
	int m_ptCount;
	TPoint m_points[4];

	virtual void initBlock() = 0;

	void moveBlock(TPoint pos)
	{
		m_pos = pos;
	};

	void drawBlock()
	{
		// 按照给定的pos,对所有板块顶点进行位移
		POINT pts[4];	// 重新定义整型顶点数组，主要是为了满足fillpolygon参数要求
		setfillcolor(m_color);
		for (int i = 0; i < m_ptCount; i++) {
			// 四舍五入取整操作延迟到最后环节进行，以保证计算的精度
			pts[i].x = INTR(m_pos.x + m_points[i].x);
			pts[i].y = INTR(m_pos.y + m_points[i].y);
		}
		fillpolygon((POINT*)pts, m_ptCount);
	};
};

class CTriangleS:CBlock
{
	void initBlock() override
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = 1 * ULEN;
		m_points[2].x = 1 * ULEN;
		m_points[2].y = 0;
	};
};

class CTangram
{
	CBlock* blocks[7];
	void drawTangram(TPoint pos)
	{
		// 七巧板每个板块采用统一函数绘制
		for (int i = 0; i < BLOCK_COUNT; i++) {
			blocks.drawBlock(pos);
		}
	};
};


