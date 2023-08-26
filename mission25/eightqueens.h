#pragma once

#include <easyx.h>

#define CELL_WIDTH 64
#define BORDER_WIDTH 32
#define MAX_CELL 8

// �ʹ�ͼ��ṹ -------------------------------------------
typedef struct tagCrown {
    IMAGE image;
    IMAGE mask;
} TCrownImage;

// ���ػʹ�ͼ�������(�ļ�)
void initCrownFile(TCrownImage* pcrown, const TCHAR* fimage, const TCHAR* fmask);
// ���ػʹ�ͼ�������(��Դ�ļ�)
void initCrownRes(TCrownImage* pcrown, const TCHAR* rtype, const TCHAR* rimage, const TCHAR* rmask);
// ���ƻʹ�͸��ͼ��
void drawCrown(TCrownImage* pcrown, int cx, int cy);

// ���̸� ---------------------------------------------
typedef struct tagBoard {
    TCrownImage* pcrown;      // �ʹ�ͼ��ָ��
    int rows[MAX_CELL];       // ʹ��һά���鱣��ʺ�λ�ã�����ֵΪ��cx�������±�Ϊ��cy
    int solvecount;           // 
} TBoard;

// ��ʼ����Ԫ��
void initBoard(TBoard* pboard, TCrownImage* pcrown = NULL);
// ���������Լ������ϵĻʺ�
void drawBoard(TBoard* pboard);
// ��cy����ӻʺ󡣣��ݹ�����㷨��
void addQueen(TBoard* pboard, int cy);