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

	void moveBlock(TPoint pos)
	{
		m_pos = pos;
		// ���ݰ��ƫ����offx/offy�����¼���ƫ�ƺ��������
		for (int i = 0; i < m_ptCount; i++) {
			m_points[i].x += (m_pos.x * ULEN);
			m_points[i].y += (m_pos.y * ULEN);
		}
	};

	void rotateBlock(int angle)
	{
		m_angle = angle;
		// ���ݰ��angle�����¼�����ת���������(ԭ�㱣�ֲ���)
		for (int i = 1; i < m_ptCount; i++)
			rotatePoint(&(m_points[i]), angle);
	}

public:
	CBlock(int angle, TPoint pos, COLORREF color)
	{
		m_angle = angle;
		m_pos = pos;
		m_color = color;
	};

	void setColor(COLORREF color)
	{
		m_color = color;
	}

	void drawBlock(TPoint pos)
	{
		// ���ո�����pos,�����а�鶥�����λ��
		POINT pts[4];	// ���¶������Ͷ������飬��Ҫ��Ϊ������fillpolygon����Ҫ��
		setfillcolor(m_color);
		for (int i = 0; i < m_ptCount; i++) {
			// ��������ȡ�������ӳٵ���󻷽ڽ��У��Ա�֤����ľ���
			pts[i].x = INTR(pos.x + m_points[i].x);
			pts[i].y = INTR(pos.y + m_points[i].y);
		}
		fillpolygon((POINT*)pts, m_ptCount);
	};
};

// С�����Ψ������½�Ϊԭ�㣬�߳� 1 - 1 - sqrt(2)
class CTriangleS :public CBlock
{
public:
	CTriangleS(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = 1 * ULEN;
		m_points[2].x = 1 * ULEN;
		m_points[2].y = 0;
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

// �������Ψ������½�Ϊԭ�㣬�߳� sqrt(2) - sqrt(2) - 2
class CTriangleM :public CBlock
{
public:
	CTriangleM(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = sqrt(2) * ULEN;
		m_points[2].x = sqrt(2) * ULEN;
		m_points[2].y = 0;
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

// �������Ψ������½�Ϊԭ�㣬�߳� 2 - 2 - 2*sqrt(2)
class CTriangleL :public CBlock
{
public:
	CTriangleL(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
	{
		m_ptCount = 3;
		m_points[0].x = 0;
		m_points[0].y = 0;
		m_points[1].x = 0;
		m_points[1].y = 2 * ULEN;
		m_points[2].x = 2 * ULEN;
		m_points[2].y = 0;
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

// �����Ρ������½�Ϊԭ�㣬�߳� 1 - 1 - 1 - 1
class CSquare :public CBlock
{
public:
	CSquare(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramR :public CBlock
{
public:
	CParallelogramR(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
class CParallelogramL :public CBlock
{
public:
	CParallelogramL(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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
		rotateBlock(m_angle);
		moveBlock(m_pos);
	}
};

class CTangram
{
	CBlock* m_blocks[BLOCK_COUNT];

public:
	CTangram(CBlock *bs[BLOCK_COUNT])
	{
		for (int i = 0; i < BLOCK_COUNT; i++)
			m_blocks[i] = bs[i];
	}
	~CTangram()
	{
		for (int i = 0; i < BLOCK_COUNT; i++)
			delete m_blocks[i];
	}

	void drawTangram(TPoint pos)
	{
		// ���ɰ�ÿ��������ͳһ��������
		for (int i = 0; i < BLOCK_COUNT; i++) {
			m_blocks[i]->drawBlock(pos);
		}
	};
};


