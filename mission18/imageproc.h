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
// 水平镜像
void onHorizontalMirrorImage(TPainter* ppainter);
// 垂直镜像
void onVerticalMirrorImage(TPainter* ppainter);
// 灰度图像
void onGrayImage(TPainter* ppainter);
// 黑白二值
void onBlackWhiteImage(TPainter* ppainter);
// 高斯模糊
void onGaussImage(TPainter* ppainter);

void onSetFillColor(TPainter* ppainter);
void onSetPenColor(TPainter* ppainter);
void onSetPenLine(TPainter* ppainter);
void onSetPenRect(TPainter* ppainter);
void onSetPenEllipse(TPainter* ppainter);
void onSetPenEraser(TPainter* ppainter);
void onSetPenMosaic(TPainter* ppainter);
void onSetPenSelect(TPainter* ppainter);

