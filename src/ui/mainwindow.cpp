/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mitsch <codeminister@publicstatic.de>
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include "libkreenshoteditor/kreenshoteditor.h"
#include "libkreenshoteditor/ui/maineditorwidget.h"

MainWindow::MainWindow(KreenshotEditor* kreenshotEditor)
{
    _ui = new Ui::MainWindow();
    _ui->setupUi(this);

    _kreenshotEditor = kreenshotEditor;

    setupUi();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::setupActions()
{
}

void MainWindow::setupUi()
{
//     auto testButton = new QPushButton("Hallo");
//     testButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//     _ui->containerEditor->addWidget(testButton);

    _ui->containerEditor->addWidget(_kreenshotEditor->createMainEditorWidget());
}
