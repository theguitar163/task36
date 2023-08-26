#include <easyx.h>
#include <graphics.h>
#include "skyline.h"

// ��ʼ������Vertex
void vertexInit(TVertex* pvt, int linecount)
{
	// ��ʼ��ǰ������
	pvt->m_step.x = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);
	pvt->m_step.y = ((rand() % 2) * 2 - 1) * (rand() % MAXSTEP + 1);

	// ����������ڴ�
	pvt->m_points = (POINT*)malloc(linecount * sizeof(POINT));
	pvt->m_pcount = linecount;

	// ������ɶ����һ���������
	POINT* pp = &(pvt->m_points[0]);
	pp->x = rand() % WIDTH;
	pp->y = rand() % HEIGHT;
	// ���ݵ�һ��������꣬���������������
	for (int i = 1; i < linecount; i++) {
		POINT* pp0 = &(pvt->m_points[i - 1]);		// ȡǰһ�����ָ��
		pp = &(pvt->m_points[i]);					// ȡ��ǰ���ָ��
		// ��ǰһ��������Ϊ��׼�����ƶ��������㵱ǰ�������
		// ע�⣺ʹ�ü���������֤ÿ���㲻�ᳬ��
		pp->x = pp0->x - pvt->m_step.x;
		pp->y = pp0->y - pvt->m_step.y;
	}
}

// �ͷŶ���Vertex����Ҫ���ͷ�m_points������ڴ�
void vertexFree(TVertex* pvt)
{
	if (pvt->m_points != NULL)
		free(pvt->m_points);
}

// ��ȡ��һ��������
POINT vertexGetHead(TVertex* pv)
{
	return pv->m_points[0];
}

// ��ȡ���һ��������
POINT vertexGetTail(TVertex* pvt)
{
	return pvt->m_points[pvt->m_pcount - 1];
}

// �ƶ���������ÿ�����λ�ã�����[0]�����꣬�������һ����[m_count-1]������
void vertexMove(TVertex* pvt)
{
	// �ƶ�������[0,1,2,3,...n-1] => [0,0,1,2,...n-2]���������һ����
	for (int i = pvt->m_pcount - 1; i > 0; i--) {
		pvt->m_points[i] = pvt->m_points[i - 1];
	}

	// ����[0]�����꣬����[0]����λ��
	POINT p;
	p.x = pvt->m_points[0].x + pvt->m_step.x;
	p.y = pvt->m_points[0].y + pvt->m_step.y;
	// �ж�p���Ƿ�Խ��
	if (p.x < 0) {
		p.x = -p.x;
		pvt->m_step.x = rand() % MAXSTEP + 1;
	}
	else if (p.x >= WIDTH) {
		p.x -= p.x - WIDTH + 1;
		pvt->m_step.x = -rand() % MAXSTEP - 1;
	}

	if (p.y < 0) {
		p.y = -p.y;
		pvt->m_step.y = rand() % MAXSTEP + 1;
	}
	else if (p.y >= HEIGHT) {
		p.y -= p.y - HEIGHT + 1;
		pvt->m_step.y = -rand() % MAXSTEP - 1;
	}
	// ����[0]������
	pvt->m_points[0] = p;
}

// ��ʼ�������polygon
// vertexcount: ��������
// linecount: �ߵ������������������������
void polygonInit(TPolygon* ppl, int vertexcount, int linecount)
{
	// ��ʼ����ɫ
	ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);

	// Ϊÿ����������ڴ�
	ppl->m_vcount = vertexcount;
	ppl->m_vertexs = (TVertex*)malloc(ppl->m_vcount * sizeof(TVertex));

	// ��ʼ��ÿ������
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexInit(&(ppl->m_vertexs[i]), linecount);
}

// �ͷŶ����
// ע�⣺��Ҫ�ͷ�ÿ�����㣨ÿ�������ں���Ϊ������������ڴ棩
void polygonFree(TPolygon* ppl)
{
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexFree(&(ppl->m_vertexs[i]));
	free(ppl->m_vertexs);
}

// ���Ʋ��ƶ������
void polygonMove(TPolygon* ppl)
{
	POINT p;
	// 1.�����������������������һ����[m_count-1]��Χ�Ķ����
	setcolor(BLACK);
	p = vertexGetTail(&(ppl->m_vertexs[ppl->m_vcount - 1]));
	moveto(p.x, p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetTail(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 2.�ƶ����������������λ��+1���������һ����[m_count-1]�����µ�һ����[0]����
	for (int i = 0; i < ppl->m_vcount; i++)
		vertexMove(&(ppl->m_vertexs[i]));

	// 3.���ƶ��������������е�һ����[0]��Χ�Ķ���Σ�����λ�ö��㲻�䣩
	setcolor(ppl->m_color);
	p = vertexGetHead(&(ppl->m_vertexs[ppl->m_vcount - 1]));
	moveto(p.x, p.y);
	for (int i = 0; i < ppl->m_vcount; i++) {
		p = vertexGetHead(&(ppl->m_vertexs[i]));
		lineto(p.x, p.y);
	}

	// 4.�� 1% �ĸ��ʸ�����ɫ
	if (rand() % 100 == 0)
		ppl->m_color = HSLtoRGB(float(rand() % 360), 1.0, 0.5);
}