// 七巧板
// 以小三角形直角边为最小长度单位
// !! 以下函数绘制以修正后的坐标系（y轴向上，x轴向右，原点为左下角）
#include <easyx.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include "tangram.h"

// 围绕原点旋转点
// 参数：pp为需要旋转的POINT指针，angle为旋转角度
void rotatePoint(TPoint* pp, int angle)
{
	double radian = PI / 180 * (-angle);	        // 旋转弧度（逆时针），为符合直觉改为顺时针
	double x = pp->x, y = pp->y;				    // 生成过程变量，避免在计算中过程变量被修改

	pp->x = x * cos(radian) - y * sin(radian);
	pp->y = x * sin(radian) + y * cos(radian);
}


void CBlock::moveBlock(TPoint pos)
{
	m_pos = pos;
	// 根据板块偏移量offx/offy，重新计算偏移后各点坐标
	for (int i = 0; i < m_ptCount; i++) {
		m_points[i].x += (m_pos.x * ULEN);
		m_points[i].y += (m_pos.y * ULEN);
	}
}

void CBlock::rotateBlock(int angle)
{
	m_angle = angle;
	// 根据板块angle，重新计算旋转后各点坐标(原点保持不动)
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
	// 按照给定的pos,对所有板块顶点进行位移
	POINT pts[4];	// 重新定义整型顶点数组，主要是为了满足fillpolygon参数要求
	setfillcolor(m_color);
	for (int i = 0; i < m_ptCount; i++) {
		// 四舍五入取整操作延迟到最后环节进行，以保证计算的精度
		pts[i].x = INTR(pos.x + m_points[i].x);
		pts[i].y = INTR(pos.y + m_points[i].y);
	}
	fillpolygon((POINT*)pts, m_ptCount);
}

// 小三角形◣，左下角为原点，边长 1 - 1 - sqrt(2)
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

// 中三角形◣，左下角为原点，边长 sqrt(2) - sqrt(2) - 2
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

// 大三角形◣，左下角为原点，边长 2 - 2 - 2*sqrt(2)
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

// 正方形■，左下角为原点，边长 1 - 1 - 1 - 1
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

// 右向平行四边形◢■◤，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
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

// 左向平行四边形◥■◣，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
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
	// 七巧板每个板块采用统一函数绘制
	for (int i = 0; i < BLOCK_COUNT; i++) {
		m_blocks[i]->drawBlock(pos);
	}
}

