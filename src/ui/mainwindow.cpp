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
#include <QDesktopServices>
#include <QUrl>
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
        foreach (auto action, allToolActions()) {
            QString toolId = actionToToolId(action);
            toolboxButtonFromId(toolId)->setIcon(toolActionFromId(toolId)->icon());
        }
//         ui->pushButtonToolSelect->setIcon(ui->actionToolSelect->icon());
//         ui->pushButtonToolRect->setIcon(ui->actionToolRect->icon());
//         ...
    }

    QAction* toolActionFromId(QString toolId)
    {
        if (toolId == "select") {
            return ui->actionToolSelect;
        }
        else if (toolId == "rect") {
            return ui->actionToolRect;
        }
        else if (toolId == "ellipse") {
            return ui->actionToolEllipse;
        }
        else if (toolId == "line") {
            return ui->actionToolLine;
        }
        else if (toolId == "text") {
            return ui->actionToolText;
        }
        else if (toolId == "highlight") {
            return ui->actionToolHighlight;
        }
        else if (toolId == "obfuscate") {
            return ui->actionToolObfuscate;
        }
        else if (toolId == "crop") {
            return ui->actionToolCrop;
        }
        else if (toolId == "ripout") {
            return ui->actionToolRipOut;
        }
        else {
            qDebug() << "toolActionFromId: TODO...........";
            Q_ASSERT(false);
            return nullptr;
        }
    }

    QPushButton* toolboxButtonFromId(QString toolId)
    {
        if (toolId == "select") {
            return ui->pushButtonToolSelect;
        }
        else if (toolId == "rect") {
            return ui->pushButtonToolRect;
        }
        else if (toolId == "ellipse") {
            return ui->pushButtonToolEllipse;
        }
        else if (toolId == "line") {
            return ui->pushButtonToolLine;
        }
        else if (toolId == "text") {
            return ui->pushButtonToolText;
        }
        else if (toolId == "highlight") {
            return ui->pushButtonToolHighlight;
        }
        else if (toolId == "obfuscate") {
            return ui->pushButtonToolObfuscate;
        }
        else if (toolId == "crop") {
            return ui->pushButtonToolCrop;
        }
        else if (toolId == "ripout") {
            return ui->pushButtonToolRipOut;
        }
        else {
            qDebug() << "toolboxButtonFromId: TODO...........";
            Q_ASSERT(false);
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

    QString actionToToolId(QObject* action)
    {
        QString senderName = action->objectName();
        QString toolId = senderName.replace("actionTool", "").toLower();
        return toolId;
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
    foreach (auto action, d->allToolActions()) {
        QString toolId = d->actionToToolId(action);

        // button click to action
        connect(d->toolboxButtonFromId(toolId), SIGNAL(clicked()), d->toolActionFromId(toolId), SLOT(trigger()));

        // action toogled to button
        connect(d->toolActionFromId(toolId), SIGNAL(toggled(bool)), d->toolboxButtonFromId(toolId), SLOT(setChecked(bool)));

        // select action to request
        connect(d->toolActionFromId(toolId), SIGNAL(triggered()), this, SLOT(requestTool()));
    }

    //connect(d->ui->pushButtonToolSelect, SIGNAL(clicked()), d->ui->actionToolSelect, SLOT(trigger()));
    //connect(d->ui->pushButtonToolRect, SIGNAL(clicked()), d->ui->actionToolRect, SLOT(trigger()));
    // ...
    //connect(d->ui->actionToolSelect, SIGNAL(toggled(bool)), d->ui->pushButtonToolSelect, SLOT(setChecked(bool)));
    //connect(d->ui->actionToolRect, SIGNAL(toggled(bool)), d->ui->pushButtonToolRect, SLOT(setChecked(bool)));
    // ..
    //connect(d->ui->actionToolSelect, SIGNAL(triggered()), this, SLOT(requestTool()));
    //connect(d->ui->actionToolRect, SIGNAL(triggered()), this, SLOT(requestTool()));
    // ...

    connect(d->ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(d->ui->actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    // actionQuit via Action Editor
    connect(d->ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(d->ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(d->ui->actionUndo, SIGNAL(triggered()), this, SLOT(editUndo()));
    connect(d->ui->actionRedo, SIGNAL(triggered()), this, SLOT(editRedo()));
    connect(d->ui->actionPreferences, SIGNAL(triggered()), this, SLOT(editPreferences()));
    connect(d->ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));

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
    // TODO: dialog
    QString path = "/tmp/aaa.png";
    d->kreenshotEditor->saveToFile(path);
    QMessageBox::information(this, "Not impl", "saved to" + path);
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MainWindow::helpAbout()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::requestTool()
{
    QString toolId = d->actionToToolId(QObject::sender());
    QString message = QString("MainWindow::requestTool: tool id '%1'. Received from action '%2'").arg(toolId).arg(QObject::sender()->objectName());
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
