// ���ɰ�
// ��С������ֱ�Ǳ�Ϊ��С���ȵ�λ
// !! ���º��������������������ϵ��y�����ϣ�x�����ң�ԭ��Ϊ���½ǣ�
#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include "tangram.h"

// Χ��ԭ����ת��
// ������ppΪ��Ҫ��ת��POINTָ�룬angleΪ��ת�Ƕ�
void rotatePoint(TPoint* pp, int angle)
{
	double radian = PI / 180 * (-angle);	        // ��ת���ȣ���ʱ�룩��Ϊ����ֱ����Ϊ˳ʱ��
	double x = pp->x, y = pp->y;				    // ���ɹ��̱����������ڼ����й��̱������޸�

	pp->x = x * cos(radian) - y * sin(radian);
	pp->y = x * sin(radian) + y * cos(radian);
}


void CBlock::moveBlock(TPoint pos)
{
	m_pos = pos;
	// ���ݰ��ƫ����offx/offy�����¼���ƫ�ƺ��������
	for (int i = 0; i < m_ptCount; i++) {
		m_points[i].x += (m_pos.x * ULEN);
		m_points[i].y += (m_pos.y * ULEN);
	}
}

void CBlock::rotateBlock(int angle)
{
	m_angle = angle;
	// ���ݰ��angle�����¼�����ת���������(ԭ�㱣�ֲ���)
	for (int i = 1; i < m_ptCount; i++)
		rotatePoint(&(m_points[i]), angle);
}

CBlock::CBlock(int angle, TPoint pos, COLORREF color)
{
	m_angle = angle;
	m_pos = pos;
	m_color = color;
}

void CBlock::setColor(COLORREF color)
{
	m_color = color;
}

void CBlock::drawBlock(TPoint pos)
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
}

// С�����Ψ������½�Ϊԭ�㣬�߳� 1 - 1 - sqrt(2)
CTriangleS::CTriangleS(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

// �������Ψ������½�Ϊԭ�㣬�߳� sqrt(2) - sqrt(2) - 2
CTriangleM::CTriangleM(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

// �������Ψ������½�Ϊԭ�㣬�߳� 2 - 2 - 2*sqrt(2)
CTriangleL::CTriangleL(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

// �����Ρ������½�Ϊԭ�㣬�߳� 1 - 1 - 1 - 1
CSquare::CSquare(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
CParallelogramR::CParallelogramR(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
CParallelogramL::CParallelogramL(int angle, TPoint pos, COLORREF color) : CBlock(angle, pos, color)
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

CTangram::CTangram(CBlock* bs[BLOCK_COUNT])
{
	for (int i = 0; i < BLOCK_COUNT; i++)
		m_blocks[i] = bs[i];
}

CTangram::~CTangram()
{
	for (int i = 0; i < BLOCK_COUNT; i++)
		delete m_blocks[i];
}

void CTangram::drawTangram(TPoint pos)
{
	// ���ɰ�ÿ��������ͳһ��������
	for (int i = 0; i < BLOCK_COUNT; i++) {
		m_blocks[i]->drawBlock(pos);
	}
}

