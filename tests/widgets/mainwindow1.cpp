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
#include <kreen/core/desktopservices.h>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QLabel>
#include <QMessageBox>
#include <QSignalMapper>
#include <QDebug>

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
        addDescriptionLabel(mainLayout, "simple QPushButton");
        auto pushButtonTest6 = new QPushButton("QPushButton", this);
        mainLayout->addWidget(pushButtonTest6);
        signalMapper->setMapping(pushButtonTest6, "clicked() pushButtonTest6");
        connect(pushButtonTest6, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

    {
        addDescriptionLabel(mainLayout, "simple QToolButton");
        auto toolButtonTest2 = new QToolButton(this);
        mainLayout->addWidget(toolButtonTest2);
        toolButtonTest2->setText("QToolButton default");
        signalMapper->setMapping(toolButtonTest2, "clicked() toolButtonTest2");
        //connect(toolButtonTest2, SIGNAL(triggered(QAction*)), signalMapper, SLOT(map()));
        connect(toolButtonTest2, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

    {
        addDescriptionLabel(mainLayout, "QToolButton 1");
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
        addDescriptionLabel(mainLayout, "QToolButton 2");
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
        addDescriptionLabel(mainLayout, "QToolButton 3");
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
        addDescriptionLabel(mainLayout, "KreenToolButton - simple");
        auto kreenToolButtonTest1 = new kreen::ui::KreenToolButton(this);
        mainLayout->addWidget(kreenToolButtonTest1);

        kreenToolButtonTest1->setText("KreenToolButton - no actions");
        signalMapper->setMapping(kreenToolButtonTest1, QString("clicked() kreenToolButtonTest1"));
        connect(kreenToolButtonTest1, SIGNAL(clicked()), signalMapper, SLOT(map()));
    }

    {
        addDescriptionLabel(mainLayout, "KreenToolButton - with big icons");
        auto kreenToolButtonTest2 = new kreen::ui::KreenToolButton(this);
        mainLayout->addWidget(kreenToolButtonTest2);

        kreenToolButtonTest2->setText("KreenToolButton - two actions - InstantPopup");
        kreenToolButtonTest2->setPopupMode(QToolButton::InstantPopup);
        signalMapper->setMapping(kreenToolButtonTest2, QString("clicked() kreenToolButtonTest2"));
        connect(kreenToolButtonTest2, SIGNAL(clicked()), signalMapper, SLOT(map()));

        auto action1 = new QAction(QIcon("/usr/share/icons/oxygen/64x64/devices/input-gaming.png"), "input-gaming.png 64x64", this);
        auto action2 = new QAction(QIcon("/usr/share/icons/oxygen/16x16/devices/media-floppy.png"), "media-floppy.png 16x16", this);
        auto action3 = new QAction(QIcon("/usr/share/icons/oxygen/32x32/devices/media-optical-mixed-cd.png"), "media-optical-mixed-cd.png 32x32", this);

        kreenToolButtonTest2->addMenuAction(action1);
        kreenToolButtonTest2->addMenuAction(action2);
        kreenToolButtonTest2->addMenuAction(action3);

        kreenToolButtonTest2->setMaxIconSize(48);

        qDebug() << kreenToolButtonTest2->menu();
    }

    {
        addDescriptionLabel(mainLayout, "DesktopServices.selectInFilemanager");
        auto button = new QPushButton("select", this);
        mainLayout->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(slotSelectInFilemanager()));
    }
}

void MainWindow1::addDescriptionLabel(QBoxLayout* layout, QString text)
{
    auto label = new QLabel(text + ":", this);
    layout->addWidget(label);
}

void MainWindow1::slotSelectInFilemanager()
{
    kreen::core::DesktopServices::selectInFilemanager("/usr/bin/python");
    kreen::core::DesktopServices::selectInFilemanager("/usr/bin/pythonOFF");
}

void MainWindow1::slotMessageBox(QString message)
{
    QMessageBox::information(this, "slotMessageBox", message);
}
