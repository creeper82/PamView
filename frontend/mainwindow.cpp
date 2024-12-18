// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>
#include "mainwindow.h"
#include "zoomablecanvas.h"


MainWindow::MainWindow() : MainWindow(new Bitmap()) {}

MainWindow::MainWindow(Bitmap* initialBitmap) {
    bitmap1 = initialBitmap;
    bitmap2 = new Bitmap();

    canvas = new ZoomableCanvas(this);
    scene = new QGraphicsScene();
    canvas->setScene(scene);

    setupNoBitmapOpenWidget();

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(canvas);
    stackedWidget->addWidget(noBitmapOpenWidget);

    createActions();
    createMenus();

    statusBar();

    setWindowTitle(tr("PAMview"));
    setMinimumSize(160, 160);
    resize(480, 320);

    setCentralWidget(stackedWidget);

    setActiveBitmap(FIRST_BITMAP);
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
    renderCanvas();
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

void MainWindow::setFirstBitmap() {
    setActiveBitmap(FIRST_BITMAP);
}

void MainWindow::setSecondBitmap() {
    setActiveBitmap(SECOND_BITMAP);
}

void MainWindow::about()
{
    int width = getActiveBitmap()->getWidth();
    int height = getActiveBitmap()->getHeight();
    size_t memoryUsageBitmap = getActiveBitmap()->getBitmapMemUsage();
    size_t memoryUsageUndo = getActiveBitmap()->getUndoStackMemUsage();

    int mbBitmap = memoryUsageBitmap / (1024 * 1024);
    int mbUndo = memoryUsageUndo / (1024 * 1024);

    QMessageBox::about(
        this,
        tr("Bitmap details"),
        QStringLiteral(
            "Details of the visible bitmap:\n\n"
            "Dimensions: %1*%2 px\n"
            "Bitmap memory usage: ~%3MB\n"
            "Undo stack memory usage: ~%4MB"
        ).arg(width).arg(height).arg(mbBitmap).arg(mbUndo)
    );
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new blank bitmap"));
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

    // DualBitmap toggles
    firstBitmapAct = new QAction(
        tr("&First"),
        this);
    firstBitmapAct->setStatusTip(tr("Switch to the first bitmap"));
    connect(firstBitmapAct, &QAction::triggered, this, &MainWindow::setFirstBitmap);

    secondBitmapAct = new QAction(
        tr("&Second"),
        this);
    secondBitmapAct->setStatusTip(tr("Switch to the second bitmap"));
    connect(secondBitmapAct, &QAction::triggered, this, &MainWindow::setSecondBitmap);

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

    dualBitmapMenu = menuBar()->addMenu(tr("&DualBitmap"));
    dualBitmapMenu->addAction(firstBitmapAct);
    dualBitmapMenu->addAction(secondBitmapAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::renderCanvas() {
    Bitmap* bitmap = getActiveBitmap();
    bool hasOpenBitmap = bitmap->hasOpenBitmap();

    closeBitmapAct->setEnabled(hasOpenBitmap);
    saveAct->setEnabled(hasOpenBitmap);
    undoAct->setEnabled(hasOpenBitmap && bitmap->canUndo());

    transformMenu->setEnabled(hasOpenBitmap);

    if (hasOpenBitmap) {
        int width = bitmap->getWidth();
        int height = bitmap->getHeight();
        int totalPixels = width * height;

        statusBar()->showMessage(tr("Rendering... %1\%").arg(0));
        QCoreApplication::processEvents();

        scene->setSceneRect(0, 0, width, height);
        QImage image(width, height, QImage::Format_RGB888);

        int pixelNumber = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++, pixelNumber++) {
                if (pixelNumber % 10000 == 0) {
                    statusBar()->showMessage(tr("Rendering... %1\%").arg((100*pixelNumber) / totalPixels));
                    QCoreApplication::processEvents();
                }

                Pixel pixel = bitmap->getPixelAt(x, y);
                image.setPixel(x, y, qRgb(pixel.r, pixel.g, pixel.b));
            }
        }

        QPixmap pixmap = QPixmap::fromImage(image);

        if (!pixmapItem) {
            pixmapItem = new QGraphicsPixmapItem(pixmap);
            scene->addItem(pixmapItem);
        } else {
            pixmapItem->setPixmap(pixmap);
        }

        stackedWidget->setCurrentWidget(canvas);

        statusBar()->clearMessage();
    } else {
        if (pixmapItem) {
           scene->clear();
           pixmapItem = nullptr;
        }
        
        stackedWidget->setCurrentWidget(noBitmapOpenWidget);
    }
}

void MainWindow::setActiveBitmap(DUAL_BITMAP desiredBitmap) {
    int desiredBitmapNumber = (desiredBitmap == FIRST_BITMAP ? 1 : 2);
    if (desiredBitmapNumber != activeBitmapNumber) {
        activeBitmapNumber = desiredBitmapNumber;
        renderCanvas();
    }

    if (desiredBitmap == FIRST_BITMAP) {
        firstBitmapAct->setText(tr("&First") + " [*]");
        secondBitmapAct->setText(tr("&Second"));
    } else {
        firstBitmapAct->setText(tr("&First"));
        secondBitmapAct->setText(tr("&Second") + " [*]");
    }
}

Bitmap* MainWindow::getActiveBitmap() {
    return (activeBitmapNumber == 1) ? bitmap1 : bitmap2;
}

void MainWindow::setupNoBitmapOpenWidget()
{
    noBitmapOpenWidget = new QWidget;

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    noBitmapLabel = new QLabel(tr("No bitmap open. Create a new bitmap, or open a file."));
    noBitmapLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *noBitmapOpenLayout = new QVBoxLayout;
    noBitmapOpenLayout->setContentsMargins(5, 5, 5, 5);
    noBitmapOpenLayout->addWidget(topFiller);
    noBitmapOpenLayout->addWidget(noBitmapLabel);
    noBitmapOpenLayout->addWidget(bottomFiller);
    noBitmapOpenWidget->setLayout(noBitmapOpenLayout);
}

void MainWindow::handleProgress(int progress)
{
    if (progress == 100) {
        statusBar()->clearMessage();
    } else {
        statusBar()->showMessage(tr("Loading... %1\%").arg(progress));
    }
    QCoreApplication::processEvents();
}