// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>
#include "mainwindow.h"


MainWindow::MainWindow() : MainWindow(new Bitmap()) {}

MainWindow::MainWindow(Bitmap* initialBitmap) {
    bitmap1 = initialBitmap;
    bitmap2 = new Bitmap();
    activeBitmap = 1;

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                            "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);

    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("PAMView"));
    setMinimumSize(160, 160);
    resize(480, 320);
}

MainWindow::~MainWindow() {
    delete bitmap1;
    delete bitmap2;
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(newAct);
    menu.addAction(openAct);
    menu.addAction(saveAct);
    menu.exec(event->globalPos());
}
#endif

void MainWindow::newBitmap()
{
    
}

void MainWindow::open()
{
    
}

void MainWindow::save()
{
    
}

void MainWindow::closeBitmap() {
    getActiveBitmap()->closeBitmap();
}

void MainWindow::exit() {
    close();
}

void MainWindow::undo()
{
    
}

void MainWindow::transformBrightness()
{
}

void MainWindow::transformSaturation()
{
}

void MainWindow::transformNegative()
{
}

void MainWindow::transformGrayscale()
{
}

void MainWindow::transformBlackAndWhite()
{
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Bitmap width:"), QStringLiteral("Bitmap width: %1").arg(getActiveBitmap()->getWidth()));
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new blank bitmap"));
    newAct->
    connect(newAct, &QAction::triggered, this, &MainWindow::newBitmap);

    openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                          tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing bitmap"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                          tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the bitmap to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    closeBitmapAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditClear),
                            tr("&Close bitmap"), this);
    closeBitmapAct->setStatusTip(tr("Close the current bitmap, freeing the memory"));
    connect(closeBitmapAct, &QAction::triggered, this, &MainWindow::closeBitmap);

    exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose),
                            tr("&Exit program"), this);
    exitAct->setStatusTip(tr("Exit the window and close bitmaps"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);

    undoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),
                          tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    // brightness
    transformBrightnessAct = new QAction(
        tr("&Brightness"),
        this);
    transformBrightnessAct->setStatusTip(tr("Adjust the image brightness"));
    connect(transformBrightnessAct, &QAction::triggered, this, &MainWindow::transformBrightness);

    // saturation
    transformSaturationAct = new QAction(
        tr("&Saturation"),
        this);
    transformSaturationAct->setStatusTip(tr("Adjust the image saturation"));
    connect(transformSaturationAct, &QAction::triggered, this, &MainWindow::transformSaturation);

    // negative
    transformNegativeAct = new QAction(
        tr("&Negative"),
        this);
    transformNegativeAct->setStatusTip(tr("Invert the image colors"));
    connect(transformNegativeAct, &QAction::triggered, this, &MainWindow::transformNegative);

    // grayscale
    transformGrayscaleAct = new QAction(
        tr("&Grayscale"),
        this);
    transformGrayscaleAct->setStatusTip(tr("Make the image grayscale"));
    connect(transformGrayscaleAct, &QAction::triggered, this, &MainWindow::transformGrayscale);

    // black&white
    transformBlackAndWhiteAct = new QAction(
        tr("&Black and white"),
        this);
    transformBlackAndWhiteAct->setStatusTip(tr("Make the image black and white"));
    connect(transformBlackAndWhiteAct, &QAction::triggered, this, &MainWindow::transformBlackAndWhite);

    aboutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                           tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the bitmap information"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(closeBitmapAct);
    fileMenu->addAction(exitAct);
    fileMenu->addSeparator();
    
    
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    transformMenu = editMenu->addMenu("&Transform");

    transformMenu->addAction(transformBrightnessAct);
    transformMenu->addAction(transformSaturationAct);
    transformMenu->addAction(transformNegativeAct);
    transformMenu->addAction(transformGrayscaleAct);
    transformMenu->addAction(transformBlackAndWhiteAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

Bitmap* MainWindow::getActiveBitmap() {
    return (activeBitmap == 1) ? bitmap1 : bitmap2;
}