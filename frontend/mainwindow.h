// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bitmap.h"
#include "zoomablecanvas.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

// Represents either first or second bitmap in the current window.
enum DUAL_BITMAP {
    FIRST_BITMAP = 1,
    SECOND_BITMAP = 2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    MainWindow(Bitmap* initialBitmap);
    ~MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif
private slots:
    void open();
    void save();
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
    void about();
private:
    void createActions();
    void createMenus();
    void renderCanvas();
    void disableTopMenus();
    void enableTopMenus();
    void displayError(QString message);
    void handleLoadExceptions();
    void transformActiveBitmapAndRender(transformType transformFunction);
    void transformActiveBitmapAndRender(transformWithLevelType transformFunction, int level);
    void setupNoBitmapOpenWidget();
    void handleProgress(int progress);
    void setActiveBitmap(DUAL_BITMAP bitmap);
    Bitmap* getActiveBitmap();
    int activeBitmapNumber;
    Bitmap* bitmap1;
    Bitmap* bitmap2;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *transformMenu;
    QMenu *dualBitmapMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *saveAct;
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
    QAction *aboutAct;
    QStackedWidget* stackedWidget = nullptr;
    QWidget *noBitmapOpenWidget = nullptr;
    QLabel *noBitmapLabel = nullptr;
    ZoomableCanvas* canvas = nullptr;
    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* pixmapItem = nullptr;
    QImage image;
};

#endif
