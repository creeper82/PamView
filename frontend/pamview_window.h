// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef PAMVIEW_WINDOW_H
#define PAMVIEW_WINDOW_H

#include "bitmap.h"
#include "zoomablecanvas.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

// Represents either first or second bitmap in the current window.
enum DUAL_BITMAP { FIRST_BITMAP = 1, SECOND_BITMAP = 2 };

class PamViewWindow : public QMainWindow {
  Q_OBJECT

public:
  PamViewWindow();
  PamViewWindow(Bitmap *initialBitmap);
  ~PamViewWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
  void contextMenuEvent(QContextMenuEvent *event) override;
#endif
private slots:
  void open();
  void saveP3();
  void saveP6();
  void saveHelp();
  void closeBitmap();
  void exit();
  void undo();
  void transformBrightness();
  void transformSaturation();
  void transformNegative();
  void transformGrayscale();
  void transformBlackAndWhite();
  void setFirstBitmap();
  void setSecondBitmap();
  void bitmapDetails();

private:
  void createActions();
  void createMenus();
  void renderCanvas();
  void disableTopMenus();
  void enableTopMenus();
  void displayError(QString message);
  void handleLoadExceptions();
  void handleSaveExceptions();
  void transformActiveBitmapAndRender(pixelTransformFunction transformFunction);
  void transformActiveBitmapAndRender(pixelTransformWithLevelFunction transformFunction,
                                      int level);
  void showDialogAndSaveAs(FILETYPE filetype);
  void setupNoBitmapOpenWidget();
  void handleProgress(int progress);
  void setActiveBitmap(DUAL_BITMAP bitmap);
  Bitmap *getActiveBitmap();
  int activeBitmapNumber;
  Bitmap *bitmap1;
  Bitmap *bitmap2;
  QMenu *fileMenu;
  QMenu *saveMenu;
  QMenu *editMenu;
  QMenu *transformMenu;
  QMenu *dualBitmapMenu;
  QMenu *infoMenu;
  QAction *openAct;
  QAction *saveP3Act;
  QAction *saveP6Act;
  QAction *saveHelpAct;
  QAction *closeBitmapAct;
  QAction *exitAct;
  QAction *undoAct;
  QAction *transformBrightnessAct;
  QAction *transformSaturationAct;
  QAction *transformNegativeAct;
  QAction *transformGrayscaleAct;
  QAction *transformBlackAndWhiteAct;
  QAction *firstBitmapAct;
  QAction *secondBitmapAct;
  QAction *propertiesAct;
  QStackedWidget *stackedWidget = nullptr;
  QWidget *noBitmapOpenWidget = nullptr;
  QLabel *noBitmapLabel = nullptr;
  ZoomableCanvas *canvas = nullptr;
  QGraphicsScene *scene = nullptr;
  QGraphicsPixmapItem *pixmapItem = nullptr;
  QImage image;
};

#endif
