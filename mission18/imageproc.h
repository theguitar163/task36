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
// 水平镜像
void HorizontalMirrorImage(TPainter* ppainter);
// 垂直镜像
void VerticalMirrorImage(TPainter* ppainter);
// 灰度图像
void GrayImage(TPainter* ppainter);
// 黑白二值
void BlackWhiteImage(TPainter* ppainter);
// 高斯模糊
void GaussImage(TPainter* ppainter);

void SetFillColor(TPainter* ppainter);
void SetPenColor(TPainter* ppainter);
void SetPenLine(TPainter* ppainter);
void SetPenRect(TPainter* ppainter);
void SetPenEllipse(TPainter* ppainter);
void SetPenEraser(TPainter* ppainter);
void SetPenMosaic(TPainter* ppainter);
