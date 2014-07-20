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
#include "mainwindow1.h"
#include <kreen/ui/kreentoolbutton.h>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow1::MainWindow1()
{
    setupUi();
    setupActions();
}

MainWindow1::~MainWindow1()
{

}

void MainWindow1::setupUi()
{
    auto centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    {
        auto testButton1 = new KreenToolButton(this);
        testButton1->setText("KreenToolButton");
        mainLayout->addWidget(testButton1);
    }

    {
        auto testButton2 = new QToolButton(this);
        testButton2->setText("QToolButton");
        mainLayout->addWidget(testButton2);
    }

    {
        auto testButton3 = new QPushButton("QPushButton", this);
        mainLayout->addWidget(testButton3);
    }
}

void MainWindow1::setupActions()
{

}
