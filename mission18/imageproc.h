#pragma once

void onLoadImage(TPainter* ppainter);
void onSaveImage(TPainter* ppainter);
void onSaveClip(TPainter* ppainter);
void onClearCanvas(TPainter* ppainter);
void onChoosePenColor(TPainter* ppainter);
void onChooseFillColor(TPainter* ppainter);
void onChoosePenThickness(TPainter* ppainter);
void onSetFill(TPainter* ppainter);
void onUndoAction(TPainter* ppainter);
void onUnselect(TPainter* ppainter);
// ˮƽ����
void onHorizontalMirrorImage(TPainter* ppainter);
// ��ֱ����
void onVerticalMirrorImage(TPainter* ppainter);
// �Ҷ�ͼ��
void onGrayImage(TPainter* ppainter);
// �ڰ׶�ֵ
void onBlackWhiteImage(TPainter* ppainter);
// ��˹ģ��
void onGaussImage(TPainter* ppainter);

void onSetFillColor(TPainter* ppainter);
void onSetPenColor(TPainter* ppainter);
void onSetPenLine(TPainter* ppainter);
void onSetPenRect(TPainter* ppainter);
void onSetPenEllipse(TPainter* ppainter);
void onSetPenEraser(TPainter* ppainter);
void onSetPenMosaic(TPainter* ppainter);
void onSetPenSelect(TPainter* ppainter);

