#pragma once
#include <QtWidgets>
// A QGraphicsView, which can be zoomed in and panned with mouse by default.
class ZoomableCanvas : public QGraphicsView {
public:
  ZoomableCanvas(QWidget *parent = nullptr);

protected:
  void wheelEvent(QWheelEvent *event) override;
};