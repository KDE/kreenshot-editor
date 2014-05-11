/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codeminister@publicstatic.de>
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
#include "lib/kreenshoteditor/kreenshoteditor.h"
#include "lib/kreenshoteditor/ui/maineditorwidget.h"

class MainWindowImpl
{
public:
    /**
     * define icons at one place (actions in the designer) and assign them here to the toolbox buttons
     */
    void setupIcons()
    {
        ui->pushButtonToolSelect->setIcon(ui->actionToolSelect->icon());
        ui->pushButtonToolRect->setIcon(ui->actionToolRect->icon());
        ui->pushButtonToolEllipse->setIcon(ui->actionToolEllipse->icon());
        ui->pushButtonToolLine->setIcon(ui->actionToolLine->icon());
        ui->pushButtonToolText->setIcon(ui->actionToolText->icon());
        ui->pushButtonToolHighlight->setIcon(ui->actionToolHighlight->icon());
        ui->pushButtonToolObfuscate->setIcon(ui->actionToolObfuscate->icon());
        ui->pushButtonToolCrop->setIcon(ui->actionToolCrop->icon());
        ui->pushButtonToolRipOut->setIcon(ui->actionToolRipOut->icon());
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

        list.push_back(ui->actionToolSelect);
        list.push_back(ui->actionToolRect);
        list.push_back(ui->actionToolEllipse);
        list.push_back(ui->actionToolLine);
        list.push_back(ui->actionToolText);
        list.push_back(ui->actionToolHighlight);
        list.push_back(ui->actionToolObfuscate);
        list.push_back(ui->actionToolCrop);
        list.push_back(ui->actionToolRipOut);
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

    d->kreenshotEditor->getMainEditorWidget()->requestTool("select");
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

    d->ui->containerEditor->addWidget(d->kreenshotEditor->getMainEditorWidget());

    d->setupIcons();
}

void MainWindow::setupActions()
{
    // button to action
    connect(d->ui->pushButtonToolSelect, SIGNAL(clicked()), d->ui->actionToolSelect, SLOT(trigger()));
    connect(d->ui->pushButtonToolRect, SIGNAL(clicked()), d->ui->actionToolRect, SLOT(trigger()));
    connect(d->ui->pushButtonToolEllipse, SIGNAL(clicked()), d->ui->actionToolEllipse, SLOT(trigger()));
    connect(d->ui->pushButtonToolLine, SIGNAL(clicked()), d->ui->actionToolLine, SLOT(trigger()));
    connect(d->ui->pushButtonToolText, SIGNAL(clicked()), d->ui->actionToolText, SLOT(trigger()));
    connect(d->ui->pushButtonToolHighlight, SIGNAL(clicked()), d->ui->actionToolHighlight, SLOT(trigger()));
    connect(d->ui->pushButtonToolObfuscate, SIGNAL(clicked()), d->ui->actionToolObfuscate, SLOT(trigger()));
    connect(d->ui->pushButtonToolCrop, SIGNAL(clicked()), d->ui->actionToolCrop, SLOT(trigger()));
    connect(d->ui->pushButtonToolRipOut, SIGNAL(clicked()), d->ui->actionToolRipOut, SLOT(trigger()));

    // action to button
    connect(d->ui->actionToolSelect, SIGNAL(toggled(bool)), d->ui->pushButtonToolSelect, SLOT(setChecked(bool)));
    connect(d->ui->actionToolRect, SIGNAL(toggled(bool)), d->ui->pushButtonToolRect, SLOT(setChecked(bool)));
    connect(d->ui->actionToolEllipse, SIGNAL(toggled(bool)), d->ui->pushButtonToolEllipse, SLOT(setChecked(bool)));
    connect(d->ui->actionToolLine, SIGNAL(toggled(bool)), d->ui->pushButtonToolLine, SLOT(setChecked(bool)));
    connect(d->ui->actionToolText, SIGNAL(toggled(bool)), d->ui->pushButtonToolText, SLOT(setChecked(bool)));
    connect(d->ui->actionToolHighlight, SIGNAL(toggled(bool)), d->ui->pushButtonToolHighlight, SLOT(setChecked(bool)));
    connect(d->ui->actionToolObfuscate, SIGNAL(toggled(bool)), d->ui->pushButtonToolObfuscate, SLOT(setChecked(bool)));
    connect(d->ui->actionToolCrop, SIGNAL(toggled(bool)), d->ui->pushButtonToolCrop, SLOT(setChecked(bool)));
    connect(d->ui->actionToolRipOut, SIGNAL(toggled(bool)), d->ui->pushButtonToolRipOut, SLOT(setChecked(bool)));

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
    connect(d->ui->actionToolText, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolHighlight, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolObfuscate, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolCrop, SIGNAL(triggered()), this, SLOT(requestTool()));
    connect(d->ui->actionToolRipOut, SIGNAL(triggered()), this, SLOT(requestTool()));

    connect(d->kreenshotEditor->getMainEditorWidget(), SIGNAL(toolChosen(QString)), this, SLOT(toolChosen(QString)));
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

    d->kreenshotEditor->getMainEditorWidget()->requestTool(toolId);
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
