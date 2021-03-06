/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codestruct@posteo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <QObject>
#include <QDebug>
#include <QString>
#include <QApplication>
#include <QThread>
#include <QGraphicsView>
#include "widgets/mainwindow1.h"
#include "trial/graphicsview1.h"
#include <array>

/**
 * opens a window that lets you interactively test custom widgets
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow1 w;
    w.show();

    auto graphicsView = createTrialGraphicsView();
    graphicsView->show();

    app.exec();
};
