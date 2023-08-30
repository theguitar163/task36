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
		// ���ո�����pos,�����а�鶥�����λ��
		POINT pts[4];	// ���¶������Ͷ������飬��Ҫ��Ϊ������fillpolygon����Ҫ��
		setfillcolor(m_color);
		for (int i = 0; i < m_ptCount; i++) {
			// ��������ȡ�������ӳٵ���󻷽ڽ��У��Ա�֤����ľ���
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
		// ���ɰ�ÿ��������ͳһ��������
		for (int i = 0; i < BLOCK_COUNT; i++) {
			blocks.drawBlock(pos);
		}
	};
};


