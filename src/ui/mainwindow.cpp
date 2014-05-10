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
#include <QDebug>
#include <QMessageBox>
#include "libkreenshoteditor/kreenshoteditor.h"
#include "libkreenshoteditor/ui/maineditorwidget.h"

class MainWindowImpl
{
public:
    /**
     * define icons at one place (actions in the designer) and assign them here to the toolbox buttons
     */
    void setupIcons()
    {
        ui->pushButtonToolEllipse->setIcon(ui->actionToolEllipse->icon());
        //ui->pushButtonToolHighlight->setIcon(ui->actionToolHighlight->icon());
        ui->pushButtonToolLine->setIcon(ui->actionToolLine->icon());
        ui->pushButtonToolRect->setIcon(ui->actionToolRect->icon());
        ui->pushButtonToolSelect->setIcon(ui->actionToolSelect->icon());
    }

    QAction* toolActionFromId(QString toolId)
    {
        // alphabetically

        if (toolId == "ellipse") {
            return ui->actionToolEllipse;
        }
//         else if (toolId == "highlight") {
//             return ui->pushButtonToolHighlight;
//         }
        else if (toolId == "line") {
            return ui->actionToolLine;
        }
        else if (toolId == "rect") {
            return ui->actionToolRect;
        }
        else if (toolId == "select") {
            return ui->actionToolSelect;
        }
        else {
            qDebug() << "TODO...........";
            return nullptr;
        }
    }

    std::vector<QAction*> allToolActions()
    {
        std::vector<QAction*> list;

        // alphabetically

//         list.push_back(ui->actionAbout);
//         list.push_back(ui->actionNew);
//         list.push_back(ui->actionOpen);
//         list.push_back(ui->actionPreferences);
//         list.push_back(ui->actionQuit);
//         list.push_back(ui->actionRedo);
//         list.push_back(ui->actionSave);
//         list.push_back(ui->actionSaveAs);
//         list.push_back(ui->pushButtonToolEllipse);
//         list.push_back(ui->pushButtonToolHighlight);
//         list.push_back(ui->pushButtonToolLine);
//         list.push_back(ui->pushButtonToolObfuscate);
//         list.push_back(ui->pushButtonToolRect);
//         list.push_back(ui->pushButtonToolSelect);
//         list.push_back(ui->pushButtonToolText);
        list.push_back(ui->actionToolEllipse);
        list.push_back(ui->actionToolLine);
        list.push_back(ui->actionToolRect);
        list.push_back(ui->actionToolSelect);
        return list;
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

    d->kreenshotEditor->requestTool("select");
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

    d->setupIcons();
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
    connect(d->ui->actionToolSelect, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolRect, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolEllipse, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolLine, SIGNAL(triggered()), this, SLOT(requestTool()));

    connect(d->kreenshotEditor.get(), SIGNAL(toolChosen(QString)), this, SLOT(toolChosen(QString)));
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

void MainWindow::requestTool()
{
    QString senderName = QObject::sender()->objectName();
    QString toolId = senderName.replace("actionTool", "").toLower();
    QString message = QString("MainWindow::requestTool: tool id '%1'. Received from action '%2'").arg(toolId).arg(senderName);
    qDebug() << message;

    d->kreenshotEditor->requestTool(toolId);
}

void MainWindow::toolChosen(QString toolId)
{
    qDebug() << "MainWindow::toolChosen: " << toolId;
    foreach (auto toolAction, d->allToolActions()) {
        //qDebug() << "uncheck";
        toolAction->setChecked(false);
    }

     auto toolAction = d->toolActionFromId(toolId);
     toolAction->setChecked(true);
}
