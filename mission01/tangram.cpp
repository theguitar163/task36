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
	double radian = PI / 180 * (-angle);	// 旋转弧度（逆时针），为符合直觉改为顺时针
	double x = pp->x, y = pp->y;				    // 生成过程变量，避免在计算中过程变量被修改

	pp->x = x * cos(radian) - y * sin(radian);
	pp->y = x * sin(radian) + y * cos(radian);
}

// 初始化板块各顶点坐标数据
void initBlock(TBlock* pblock)
{
	// 根据板块类型初始化各顶点默认坐标
	switch (pblock->type) {
	// 小三角形◣，左下角为原点，边长 1 - 1 - sqrt(2)
	case eTriangleS:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = 1 * ULEN;
		pblock->points[2].x = 1 * ULEN;
		pblock->points[2].y = 0;
		break;
	// 中三角形◣，左下角为原点，边长 sqrt(2) - sqrt(2) - 2
	case eTriangleM:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = sqrt(2) * ULEN;
		pblock->points[2].x = sqrt(2) * ULEN;
		pblock->points[2].y = 0;
		break;
	// 大三角形◣，左下角为原点，边长 2 - 2 - 2*sqrt(2)
	case eTriangleL:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = 2 * ULEN;
		pblock->points[2].x = 2 * ULEN;
		pblock->points[2].y = 0;
		break;
	// 正方形■，左下角为原点，边长 1 - 1 - 1 - 1
	case eSquare:
		pblock->pcount = 4;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = 1 * ULEN;
		pblock->points[2].x = 1 * ULEN;
		pblock->points[2].y = 1 * ULEN;
		pblock->points[3].x = 1 * ULEN;
		pblock->points[3].y = 0;
		break;
	// 右向平行四边形◢■◤，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
	case eParallelogramR:
		pblock->pcount = 4;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 2 * sqrt(0.5) * ULEN;
		pblock->points[1].y = 0;
		pblock->points[2].x = 3 * sqrt(0.5) * ULEN;
		pblock->points[2].y = sqrt(0.5) * ULEN;
		pblock->points[3].x = sqrt(0.5) * ULEN;
		pblock->points[3].y = sqrt(0.5) * ULEN;
		break;
	// 左向平行四边形◥■◣，左下角为原点，边长sqrt(2) - 1 - sqrt(2) - 1
	case eParallelogramL:
		pblock->pcount = 4;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 2 * sqrt(0.5) * ULEN;
		pblock->points[1].y = 0;
		pblock->points[2].x = sqrt(0.5) * ULEN;		
		pblock->points[2].y = sqrt(0.5) * ULEN;
		pblock->points[3].x = -sqrt(0.5) * ULEN;
		pblock->points[3].y = sqrt(0.5) * ULEN;
		break;
	}
	// 根据板块angle，重新计算旋转后各点坐标(原点保持不动)
	for (int i = 1; i < pblock->pcount; i++)
		rotatePoint(&(pblock->points[i]), pblock->angle);

	// 根据板块偏移量offx/offy，重新计算偏移后各点坐标
	for (int i = 0; i < pblock->pcount; i++) {
		pblock->points[i].x += (pblock->offx * ULEN);
		pblock->points[i].y += (pblock->offy * ULEN);
	}
}
// 绘制板块
void drawBlock(TBlock block, TPoint pos)
{
	// 初始化板块
	initBlock(&block);

	// 按照给定的pos,对所有板块顶点进行位移
	POINT pts[4];	// 重新定义整型顶点数组，主要是为了满足fillpolygon参数要求
	setfillcolor(block.color);
	for (int i = 0; i < block.pcount; i++) {
		// 四舍五入取整操作延迟到最后环节进行，以保证计算的精度
		pts[i].x = INTR(pos.x + block.points[i].x);
		pts[i].y = INTR(pos.y + block.points[i].y);
	}
	fillpolygon((POINT*)pts, block.pcount);
}

// 绘制七巧板
void drawTangram(TTangram tgram, TPoint pos)
{
	// 七巧板每个板块采用统一函数绘制
	for (int i = 0; i < BLOCK_COUNT; i++) {
		drawBlock(tgram[i], pos);
	}
}
