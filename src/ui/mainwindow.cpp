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
#include <QMessageBox>
#include "libkreenshoteditor/kreenshoteditor.h"
#include "libkreenshoteditor/ui/maineditorwidget.h"

class MainWindowImpl
{
public:
    QAction* toolActionFromId(QString toolId)
    {
        if (toolId == "select") {
            return ui->actionToolSelect;
        }
    }

public:
    KreenshotEditorPtr kreenshotEditor;
    Ui::MainWindow* ui;
};

MainWindow::MainWindow(KreenshotEditorPtr kreenshotEditor)
{
    d = MainWindowImplPtr(new MainWindowImpl());
    d->kreenshotEditor = kreenshotEditor;
    d->ui = new Ui::MainWindow();
    d->ui->setupUi(this);

    setupUi();
    setupActions();
}

MainWindow::~MainWindow()
{
    delete d->ui;
}

void MainWindow::setupUi()
{
//     auto testButton = new QPushButton("Hallo");
//     testButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//     _ui->containerEditor->addWidget(testButton);

    d->ui->containerEditor->addWidget(d->kreenshotEditor->createMainEditorWidget());
}

void MainWindow::setupActions()
{
    // as listed in the Action Editor:
    connect(d->ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(d->ui->actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    // actionQuit via Action Editor
    connect(d->ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(d->ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(d->ui->actionUndo, SIGNAL(triggered()), this, SLOT(editUndo()));
    connect(d->ui->actionRedo, SIGNAL(triggered()), this, SLOT(editRedo()));
    connect(d->ui->actionPreferences, SIGNAL(triggered()), this, SLOT(editPreferences()));
    connect(d->ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    connect(d->ui->actionToolSelect, SIGNAL(triggered()), d->kreenshotEditor.get(), SLOT(requestTool()));
    connect(d->ui->actionToolRect, SIGNAL(triggered()), d->kreenshotEditor.get(), SLOT(requestTool()));
    connect(d->ui->actionToolEllipse, SIGNAL(triggered()), d->kreenshotEditor.get(), SLOT(requestTool()));
}

void MainWindow::editPreferences()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::editRedo()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::editUndo()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::fileNew()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::fileOpen()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::fileSave()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::fileSaveAs()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::helpAbout()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::toolChosen(QString toolId)
{
    // TODO
}
