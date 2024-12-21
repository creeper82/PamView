// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QtWidgets>

#include "pamview_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    PamViewWindow window;
    
    window.show();
    return app.exec();
}



