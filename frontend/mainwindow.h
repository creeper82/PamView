// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif
private slots:
    void newBitmap();
    void open();
    void save();
    void undo();
    void transformBrightness();
    void transformSaturation();
    void transformNegative();
    void transformGrayscale();
    void transformBlackAndWhite();
    void about();
private:
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *transformMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *undoAct;
    QAction *transformBrightnessAct;
    QAction *transformSaturationAct;
    QAction *transformNegativeAct;
    QAction *transformGrayscaleAct;
    QAction *transformBlackAndWhiteAct;
    QAction *aboutAct;
    QLabel *infoLabel;
};

#endif
