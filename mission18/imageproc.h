#pragma once

void LoadImage(TPainter* ppainter);
void SaveImage(TPainter* ppainter);
void SaveClip(TPainter* ppainter);
void ClearCanvas(TPainter* ppainter);
void ChoosePenColor(TPainter* ppainter);
void ChooseFillColor(TPainter* ppainter);
void ChoosePenThickness(TPainter* ppainter);
void SetFill(TPainter* ppainter);
void UndoAction(TPainter* ppainter);
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

void SetFillColor(TPainter* ppainter);
void SetPenColor(TPainter* ppainter);
void SetPenLine(TPainter* ppainter);
void SetPenRect(TPainter* ppainter);
void SetPenEllipse(TPainter* ppainter);
void SetPenEraser(TPainter* ppainter);
void SetPenMosaic(TPainter* ppainter);
