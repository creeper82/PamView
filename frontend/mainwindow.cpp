// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>
#include <exception>
#include <functional>
#include <fstream>
#include "mainwindow.h"
#include "bitmap.h"
#include "exceptions.h"
#include "transformations.h"
#include "zoomablecanvas.h"
#include "sliderdialog.h"


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
    menu.addAction(openAct);
    menu.addMenu(saveMenu);
    menu.addAction(aboutAct);
    menu.exec(event->globalPos());
}
#endif

void MainWindow::open()
{
    auto filename = QFileDialog::getOpenFileName(this,
        tr("Open image"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        tr("Portable anymap (*.pbm *.pgm *.ppm)")
    );

    if (!filename.isEmpty() && QFile::exists(filename)) {
        std::ifstream stream(filename.toStdString());

        disableTopMenus();

        try {
            getActiveBitmap()->openFromStream(
                stream,
                std::bind(&MainWindow::handleProgress, this, std::placeholders::_1)
            );
        }
        catch (std::exception) {
            handleLoadExceptions();
        }

        enableTopMenus();

        stream.close();
        renderCanvas();
    }
    
}

void MainWindow::saveP3()
{
    showDialogAndSaveAs(P3);
}

void MainWindow::saveP6()
{
    showDialogAndSaveAs(P6);
}

void MainWindow::saveHelp()
{
    QMessageBox::about(
        this,
        tr("File formats"),
        tr(
            "You can save the bitmap either in a raw format or ASCII format.<br><br>"

            "<b>ASCII (P3)</b>:<br>"
            "- larger filesize (up to 3x)<br>"
            "- human readable RGB values<br>"
            "- slightly slower<br><br>"

            "<b>Binary / raw (P6)</b>:<br>"
            "- smaller filesize<br>"
            "- human ureadable values (no visible RGB numbers)<br>"
            "- fastest<br><br>"

            "Both are PPM (portable pixmap) formats, "
            "meaning they store pixels as 24-bit RGB, aka truecolor."
        )
    );
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
    getActiveBitmap()->undoLastChange();
    renderCanvas();
}

void MainWindow::transformBrightness()
{
    SliderDialog dialog(nullptr, "Adjust brightness");

    if (dialog.exec() == QDialog::Accepted) {
        transformActiveBitmapAndRender(PixelTransformations::brightness, dialog.getValue());
    }
}

void MainWindow::transformSaturation()
{
    SliderDialog dialog(nullptr, "Adjust saturation");
    
    if (dialog.exec() == QDialog::Accepted) {
        transformActiveBitmapAndRender(PixelTransformations::saturation, dialog.getValue());
    }
}

void MainWindow::transformNegative()
{
    transformActiveBitmapAndRender(PixelTransformations::negative);
}

void MainWindow::transformGrayscale()
{
    transformActiveBitmapAndRender(PixelTransformations::grayscale);
}

void MainWindow::transformBlackAndWhite()
{
    transformActiveBitmapAndRender(PixelTransformations::blacknwhite);
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
    openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                          tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing bitmap"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveP3Act = new QAction(tr("&ASCII"), this);
    saveP3Act->setStatusTip(tr("Save in a P3 format (ASCII)"));
    connect(saveP3Act, &QAction::triggered, this, &MainWindow::saveP3);

    saveP6Act = new QAction(tr("&Binary (raw)"), this);
    saveP6Act->setShortcuts(QKeySequence::Save);
    saveP6Act->setStatusTip(tr("Save in a P6 format (binary / raw)"));
    connect(saveP6Act, &QAction::triggered, this, &MainWindow::saveP6);

    saveHelpAct = new QAction(
        QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
        tr("&How to pick"), this);
    saveHelpAct->setStatusTip(tr("Display the save help menu"));
    connect(saveHelpAct, &QAction::triggered, this, &MainWindow::saveHelp);

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
    fileMenu->addAction(openAct);
    saveMenu = fileMenu->addMenu("&Save as");
    fileMenu->addAction(closeBitmapAct);
    fileMenu->addAction(exitAct);
    fileMenu->addSeparator();

    saveMenu->addAction(saveP3Act);
    saveMenu->addAction(saveP6Act);
    saveMenu->addAction(saveHelpAct);
    
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
    saveMenu->setEnabled(hasOpenBitmap);
    undoAct->setEnabled(hasOpenBitmap && bitmap->canUndo());

    transformMenu->setEnabled(hasOpenBitmap);

    if (hasOpenBitmap) {
        int width = bitmap->getWidth();
        int height = bitmap->getHeight();
        int totalPixels = width * height;

        statusBar()->showMessage(tr("Rendering... %1\%").arg(0));
        disableTopMenus();
        QCoreApplication::processEvents();

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

        scene->setSceneRect(0, 0, width, height);

        stackedWidget->setCurrentWidget(canvas);

        enableTopMenus();

        size_t memoryUsage = bitmap->getBitmapMemUsage();
        int memoryUsageMb = memoryUsage / (1024 * 1024);
        statusBar()->showMessage(QString("Loaded %1x%2 pixels ~ %3 MB").arg(width).arg(height).arg(memoryUsageMb));
    } else {
        if (pixmapItem) {
           scene->clear();
           pixmapItem = nullptr;
        }
        
        stackedWidget->setCurrentWidget(noBitmapOpenWidget);
        statusBar()->clearMessage();
    }
}

void MainWindow::disableTopMenus() {
    fileMenu->setEnabled(false);
    editMenu->setEnabled(false);
    dualBitmapMenu->setEnabled(false);
}

void MainWindow::enableTopMenus() {
    fileMenu->setEnabled(true);
    editMenu->setEnabled(true);
    dualBitmapMenu->setEnabled(true);
}

void MainWindow::displayError(QString message) {
    QMessageBox::warning(
        this,
        tr("Error"),
        message
    );
}

void MainWindow::handleLoadExceptions() {
    try {
        throw;
    } catch (unsupported_format_exception) {
        displayError(tr("This bitmap format (P-number) is not supported."));
    }
    catch (too_large_exception) {
        displayError(tr("The bitmap is too large! Exceeded 100 000 000 pixels."));
    }
    catch (bad_dimensions_exception) {
        displayError(tr("The bitmap has invalid dimensions"));
    }
    catch (unsupported_maxvalue_exception) {
        displayError(tr("This bitmap maxvalue is not yet supported."));
    }
    catch (stream_corrupt_exception) {
        displayError(tr("Could not load the bitmap. File may be corrupt"));
    }
    catch (std::exception) {
        displayError(tr("Unrecognized error occured. Operation failed."));
    }
}

void MainWindow::handleSaveExceptions() {
    try {
        throw;
    } catch (unsupported_format_exception) {
        displayError(tr("Can't save to this format."));
    } catch (too_large_exception) {
        displayError(tr("The bitmap is too large to be saved."));
    } catch (no_bitmap_open_exception) {
        displayError(tr("No bitmap was open when trying to save."));
    } catch (std::exception) {
        displayError(tr("Unrecognized error occured. Operation failed."));
    }
}

void MainWindow::transformActiveBitmapAndRender(transformType transformFunction)
{
    disableTopMenus();

    getActiveBitmap()->transformImage(
        transformFunction,
        std::bind(&MainWindow::handleProgress, this, std::placeholders::_1)
    );

    enableTopMenus();

    renderCanvas();
}

void MainWindow::transformActiveBitmapAndRender(transformWithLevelType transformFunction, int level)
{
    disableTopMenus();

    getActiveBitmap()->transformImage(
        transformFunction,
        level,
        std::bind(&MainWindow::handleProgress, this, std::placeholders::_1)
    );

    enableTopMenus();
    
    renderCanvas();
}

void MainWindow::showDialogAndSaveAs(FILETYPE filetype)
{
    auto filename = QFileDialog::getSaveFileName(
        this, tr("Save image"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        tr("Portable anymap (*.ppm)"));

    if (!filename.isEmpty()) {
    std::ofstream stream(filename.toStdString());

    disableTopMenus();

    try {
        getActiveBitmap()->saveToStream(
            stream, filetype,
            std::bind(&MainWindow::handleProgress, this, std::placeholders::_1));
    } catch (std::exception) {
        handleSaveExceptions();
    }

    enableTopMenus();

    statusBar()->showMessage("File saved!");
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