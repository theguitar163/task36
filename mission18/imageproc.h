#pragma once

void PaintLine(TPainter* ppainter, int startx, int starty);
void PaintRect(TPainter* ppainter, int startx, int starty);
void PaintEllipse(TPainter* ppainter, int startx, int starty);

void LoadImage(TPainter* ppainter);
// ˮƽ����
void HorizontalMirrorImage(TPainter* ppainter);
// ��ֱ����
void VerticalMirrorImage(TPainter* ppainter);
// �Ҷ�ͼ��
void GrayImage(TPainter* ppainter);
// �ڰ׶�ֵ
void BlackWhiteImage(TPainter* ppainter);
// ��˹ģ��
void GaussImage(TPainter* ppainter);

void SetPenLine(TPainter* ppainter);
void SetPenRect(TPainter* ppainter);
void SetPenEllipse(TPainter* ppainter);
