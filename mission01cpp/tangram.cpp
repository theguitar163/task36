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
