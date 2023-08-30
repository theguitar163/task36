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
