// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QtWidgets>

#include "mainwindow.h"
#include "bitmap.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Bitmap* bitmap = new Bitmap(2,3);
    MainWindow window(bitmap);
    window.setMinimumSize(1000, 400);
    
    window.show();
    return app.exec();
}



