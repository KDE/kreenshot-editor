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
#include <QAction>
#include <QMessageBox>
#include <QSignalMapper>

MainWindow1::MainWindow1()
{
    setupUi();
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

    auto signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(slotMessageBox(QString))) ;

    {
        auto pushButtonTest6 = new QPushButton("QPushButton", this);
        mainLayout->addWidget(pushButtonTest6);
        signalMapper->setMapping(pushButtonTest6, "clicked() pushButtonTest6");
        connect(pushButtonTest6, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

    {
        auto toolButtonTest2 = new QToolButton(this);
        mainLayout->addWidget(toolButtonTest2);
        toolButtonTest2->setText("QToolButton default");
        signalMapper->setMapping(toolButtonTest2, "clicked() toolButtonTest2");
        //connect(toolButtonTest2, SIGNAL(triggered(QAction*)), signalMapper, SLOT(map()));
        connect(toolButtonTest2, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

    {
        auto toolButtonTest3 = new QToolButton(this);
        mainLayout->addWidget(toolButtonTest3);

        toolButtonTest3->setText("QToolButton with action - DelayedPopup (default)");

        signalMapper->setMapping(toolButtonTest3, "action clicked() of toolButtonTest3");
        connect(toolButtonTest3, SIGNAL(clicked()), signalMapper, SLOT(map()));

        auto action = new QAction(QIcon::fromTheme("edit-undo"), "Reset filename pattern to default", this);
        signalMapper->setMapping(action, "action triggered() of toolButtonTest3");
        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        toolButtonTest3->addAction(action);
    }

    {
        auto toolButtonTest4 = new QToolButton(this);
        mainLayout->addWidget(toolButtonTest4);

        toolButtonTest4->setText("QToolButton with action - MenuButtonPopup");
        toolButtonTest4->setPopupMode(QToolButton::MenuButtonPopup);

        signalMapper->setMapping(toolButtonTest4, "action clicked() of toolButtonTest4");
        connect(toolButtonTest4, SIGNAL(clicked()), signalMapper, SLOT(map()));

        auto action = new QAction(QIcon::fromTheme("edit-undo"), "Reset filename pattern to default", this);
        signalMapper->setMapping(action, "action triggered() of toolButtonTest4");
        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        toolButtonTest4->addAction(action);
    }

    {
        auto toolButtonTest5 = new QToolButton(this);
        mainLayout->addWidget(toolButtonTest5);

        toolButtonTest5->setText("QToolButton with action - InstantPopup");
        toolButtonTest5->setPopupMode(QToolButton::InstantPopup);

        signalMapper->setMapping(toolButtonTest5, "action clicked() of toolButtonTest5");
        connect(toolButtonTest5, SIGNAL(clicked()), signalMapper, SLOT(map()));

        auto action = new QAction(QIcon::fromTheme("edit-undo"), "Reset filename pattern to default", this);
        signalMapper->setMapping(action, "action triggered() of toolButtonTest5");
        connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
        toolButtonTest5->addAction(action);
    }

    {
        auto toolButtonTest1 = new KreenToolButton(this);
        mainLayout->addWidget(toolButtonTest1);
        toolButtonTest1->setText("...");
        signalMapper->setMapping(toolButtonTest1, QString("clicked() toolButtonTest1"));
        connect(toolButtonTest1, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }
}

void MainWindow1::slotMessageBox(QString message)
{
    QMessageBox::information(this, "slotMessageBox", message);
}
