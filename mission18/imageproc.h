#pragma once

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

void SetPenColor(TPainter* ppainter);
void SetPenLine(TPainter* ppainter);
void SetPenRect(TPainter* ppainter);
void SetPenEllipse(TPainter* ppainter);
void SetPenEraser(TPainter* ppainter);
void SetPenMosaic(TPainter* ppainter);
