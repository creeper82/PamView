#include <QtWidgets>
#include "zoomablecanvas.h"

ZoomableCanvas::ZoomableCanvas(QWidget *parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void ZoomableCanvas::wheelEvent(QWheelEvent *event) {
  constexpr qreal zoomFactor = 1.15;

  if (event->angleDelta().y() > 0) {
    scale(zoomFactor, zoomFactor);
  } else {
    scale(1 / zoomFactor, 1 / zoomFactor);
  }
}