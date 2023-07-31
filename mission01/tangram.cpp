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
	double radian = PI / 180 * (-angle);	// ��ת���ȣ���ʱ�룩��Ϊ����ֱ����Ϊ˳ʱ��
	double x = pp->x, y = pp->y;				    // ���ɹ��̱����������ڼ����й��̱������޸�

	pp->x = x * cos(radian) - y * sin(radian);
	pp->y = x * sin(radian) + y * cos(radian);
}

// ��ʼ������������������
void initBlock(TBlock* pblock)
{
	// ���ݰ�����ͳ�ʼ��������Ĭ������
	switch (pblock->type) {
	// С�����Ψ������½�Ϊԭ�㣬�߳� 1 - 1 - sqrt(2)
	case eTriangleS:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = 1 * ULEN;
		pblock->points[2].x = 1 * ULEN;
		pblock->points[2].y = 0;
		break;
	// �������Ψ������½�Ϊԭ�㣬�߳� sqrt(2) - sqrt(2) - 2
	case eTriangleM:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = sqrt(2) * ULEN;
		pblock->points[2].x = sqrt(2) * ULEN;
		pblock->points[2].y = 0;
		break;
	// �������Ψ������½�Ϊԭ�㣬�߳� 2 - 2 - 2*sqrt(2)
	case eTriangleL:
		pblock->pcount = 3;
		pblock->points[0].x = 0;
		pblock->points[0].y = 0;
		pblock->points[1].x = 0;
		pblock->points[1].y = 2 * ULEN;
		pblock->points[2].x = 2 * ULEN;
		pblock->points[2].y = 0;
		break;
	// �����Ρ������½�Ϊԭ�㣬�߳� 1 - 1 - 1 - 1
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
	// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
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
	// ����ƽ���ı��Ψ����������½�Ϊԭ�㣬�߳�sqrt(2) - 1 - sqrt(2) - 1
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
	// ���ݰ��angle�����¼�����ת���������(ԭ�㱣�ֲ���)
	for (int i = 1; i < pblock->pcount; i++)
		rotatePoint(&(pblock->points[i]), pblock->angle);

	// ���ݰ��ƫ����offx/offy�����¼���ƫ�ƺ��������
	for (int i = 0; i < pblock->pcount; i++) {
		pblock->points[i].x += (pblock->offx * ULEN);
		pblock->points[i].y += (pblock->offy * ULEN);
	}
}
// ���ư��
void drawBlock(TBlock block, TPoint pos)
{
	// ��ʼ�����
	initBlock(&block);

	// ���ո�����pos,�����а�鶥�����λ��
	POINT pts[4];	// ���¶������Ͷ������飬��Ҫ��Ϊ������fillpolygon����Ҫ��
	setfillcolor(block.color);
	for (int i = 0; i < block.pcount; i++) {
		// ��������ȡ�������ӳٵ���󻷽ڽ��У��Ա�֤����ľ���
		pts[i].x = INTR(pos.x + block.points[i].x);
		pts[i].y = INTR(pos.y + block.points[i].y);
	}
	fillpolygon((POINT*)pts, block.pcount);
}

// �������ɰ�
void drawTangram(TTangram tgram, TPoint pos)
{
	// ���ɰ�ÿ��������ͳһ��������
	for (int i = 0; i < BLOCK_COUNT; i++) {
		drawBlock(tgram[i], pos);
	}
}
