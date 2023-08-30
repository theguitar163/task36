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
	int m_angle;
	TPoint m_pos;
	COLORREF m_color;
	int m_ptCount;
	TPoint m_points[4];

	virtual void initBlock() = 0;

	void moveBlock(TPoint pos)
	{
		m_pos = pos;
		// 根据板块偏移量offx/offy，重新计算偏移后各点坐标
		for (int i = 0; i < m_ptCount; i++) {
			m_points[i].x += (m_pos.x * ULEN);
			m_points[i].y += (m_pos.y * ULEN);
		}
	};

	void rotateBlock(int angle)
	{
		m_angle = angle;
		// 根据板块angle，重新计算旋转后各点坐标(原点保持不动)
		for (int i = 1; i < m_ptCount; i++)
			rotatePoint(&(m_points[i]), angle);
	}

public:
	CBlock(int angle, TPoint pos, COLORREF color)
	{
		m_angle = angle;
		m_pos = pos;
		m_color = color;
		initBlock();
	};

	void setColor(COLORREF color)
	{
		m_color = color;
	}

	void drawBlock(TPoint pos)
	{
		// 按照给定的pos,对所有板块顶点进行位移
		POINT pts[4];	// 重新定义整型顶点数组，主要是为了满足fillpolygon参数要求
		setfillcolor(m_color);
		for (int i = 0; i < m_ptCount; i++) {
			// 四舍五入取整操作延迟到最后环节进行，以保证计算的精度
			pts[i].x = INTR(pos.x + m_points[i].x);
			pts[i].y = INTR(pos.y + m_points[i].y);
		}
		fillpolygon((POINT*)pts, m_ptCount);
	};
};

// 小三角形◣，左下角为原点，边长 1 - 1 - sqrt(2)
class CTriangleS :CBlock
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

// 中三角形◣，左下角为原点，边长 sqrt(2) - sqrt(2) - 2
class CTriangleM :CBlock
{
	void initBlock() override
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = sqrt(2) * ULEN;
		m_points[2].x = sqrt(2) * ULEN;
		m_points[2].y = 0;
	}
};

// 大三角形◣，左下角为原点，边长 2 - 2 - 2*sqrt(2)
class CTriangleL :CBlock
{
	void initBlock() override
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = 2 * ULEN;
		m_points[2].x = 2 * ULEN;
		m_points[2].y = 0;
	}
};

// 正方形■，左下角为原点，边长 1 - 1 - 1 - 1
class CSquare :CBlock
{
	void initBlock() override
	{
		m_ptCount = 4;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = 1 * ULEN;
		m_points[2].x = 1 * ULEN;
		m_points[2].y = 1 * ULEN;
		m_points[3].x = 1 * ULEN;
		m_points[3].y = 0;
	}
};

// 右向平行四边形◢■◤，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramR : CBlock
{
	void initBlock() override
	{
		m_ptCount = 4;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 2 * sqrt(0.5) * ULEN;
		m_points[1].y = 0;
		m_points[2].x = 3 * sqrt(0.5) * ULEN;
		m_points[2].y = sqrt(0.5) * ULEN;
		m_points[3].x = sqrt(0.5) * ULEN;
		m_points[3].y = sqrt(0.5) * ULEN;
	}
};

// 左向平行四边形◥■◣，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramL : CBlock
{
	void initBlock() override
	{
		m_ptCount = 4;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 2 * sqrt(0.5) * ULEN;
		m_points[1].y = 0;
		m_points[2].x = sqrt(0.5) * ULEN;
		m_points[2].y = sqrt(0.5) * ULEN;
		m_points[3].x = -sqrt(0.5) * ULEN;
		m_points[3].y = sqrt(0.5) * ULEN;
	}
};

class CTangram
{
	CBlock* m_blocks[7];

public:
	CTangram(CBlock *bs[7])
	{
		for (int i = 0; i < 7; i++)
			m_blocks[i] = bs[i];
	}

	void drawTangram(TPoint pos)
	{
		// 七巧板每个板块采用统一函数绘制
		for (int i = 0; i < BLOCK_COUNT; i++) {
			m_blocks[i]->drawBlock(pos);
		}
	};
};


