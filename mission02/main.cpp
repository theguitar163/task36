// ������ֱ�߻���һ��������ǡ�
// Ҫ����Ƶ������Ϊ������ǣ��� 5 ��ֱ�߹��ɡ�
// �ѵ㣺��������ǵ� 5 �����������Ǹ���Ŀ���ص㡣
//       5 ��������ȷֲ�������ǵ����Բ�ϣ��������Ǻ��������������Բ���ϵ���ȷֵ����ꡣ
#include <stdio.h>
#include <conio.h>
#include <easyx.h>
#include <math.h>
#define PI 3.14159265359

// ���������
// ������op���㣬r��ֱ����towardΪ����ָ��ĵ�
void drawStar(POINT op, int r, POINT toward)
{
	POINT pts[5];
	circle(op.x, op.y, r);

	// ��toward�㵽op��������x��ļн�(����)
	// ע�ⷴ���к���atain2����Ϊ��y, x��
	double radian = atan2((double)(toward.y - op.y), (double)(toward.x - op.x));

	// �����������ⶨ�����꣬���Լ�Ϊ��Բ�ܷ�Ϊ5�ȷ�
	// �ӻ���0��ʼ����ʱ����Ƕ������ң�������Ϊ0��1/5 * 2�С�2/5 * 2�С�3/5 * 2�С�4/5 * 2�У�
	// ͬʱ����ƫ�Ƽн�radian
	for (int i = 0; i < 5; i++) {
		pts[i].x = r * cos(radian + i * 0.2 * 2 * PI) + op.x;
		pts[i].y = r * sin(radian + i * 0.2 * 2 * PI) + op.y;
	}

	for (int i = 0; i < 5; i++) {
		int from = i * 2 % 5, to = (i * 2 + 2) % 5;
		line(pts[from].x, pts[from].y, pts[to].x, pts[to].y);
	}
	// ��������ǳ���ʾ����
	line(op.x, op.y, toward.x, toward.y);
}

// ������������
// ������op���㣬r��ֱ����towardΪ����ָ��ĵ�
void fillStar(POINT op, int r, POINT toward)
{
	POINT pts_out[5], pts_in[5], pts[10];
	double rr;

	// ��toward�㵽op��������x��ļн�(����)
	// ע�ⷴ���к���atain2����Ϊ��y, x��
	double radian = atan2((double)(toward.y - op.y), (double)(toward.x - op.x));

	// �������������ⶨ�����꣬���Լ�Ϊ��Բ�ܷ�Ϊ5�ȷ�
	// �ӻ���0��ʼ����ʱ����Ƕ������ң�������Ϊ0��1/5 * 2�С�2/5 * 2�С�3/5 * 2�С�4/5 * 2�У�
	// ͬʱ����ƫ�Ƽн�radian
	for (int i = 0; i < 5; i++) {
		pts_out[i].x = r * cos(radian + i * 0.2 * 2 * PI) + op.x;
		pts_out[i].y = r * sin(radian + i * 0.2 * 2 * PI) + op.y;
	}

	// ������Բ�İ뾶
	// rr = r * sin18 / cos36
	rr = r * sin(0.1 * PI) / cos(0.2 * PI);

	// �������������ڶ������꣬���㷽�����ⶥ����ͬ
	// Ψһ������ڣ���ʼ���Ȳ��Ǵ�0��ʼ�����Ǵ�36�ȣ�1/10�У���ʼ
	for (int i = 0; i < 5; i++) {
		pts_in[i].x = rr * cos(radian + (i * 0.2 + 0.1) * 2 * PI) + op.x;
		pts_in[i].y = rr * sin(radian + (i * 0.2 + 0.1) * 2 * PI) + op.y;
	}

	// ���ⶥ����ڶ�������ϲ���������λ���Ҫ�󣬽�������pts������
	for (int i = 0; i < 5; i++) {
		// ��������pts����
		pts[i * 2] = pts_out[i];
		pts[i * 2 + 1] = pts_in[i];
	}
	setfillcolor(YELLOW);
	fillpolygon((POINT*)pts, 10);
}

int main()
{
	initgraph(800, 800);
	setaspectratio(1, -1);
	setorigin(getwidth()/2, getheight()/2);
	int r = 80;

	drawStar({ 0, 0 }, r, { 0, r});
	fillStar({ -200, 0 }, r, { 0, 0 });
	fillStar({ 200, 0 }, r, { 0, 0 });
	fillStar({ 0, 200 }, r, { 0, 0 });
	fillStar({ 0, -200 }, r, { 0, 0 });
	_getch();
	closegraph();
}