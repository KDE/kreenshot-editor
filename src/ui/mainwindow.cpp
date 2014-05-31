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
#include <QUrl>
#include <QFileDialog>
#include "lib/kreenshoteditor/kreenshoteditor.h"
#include "lib/kreenshoteditor/ui/maineditorwidget.h"
#include "lib/kreenshoteditor/core/outputfilenamemanager.h"

class MainWindowImpl
{
public:
    void setupActionIds()
    {
        ui->actionToolSelect->setData("select");
        ui->actionToolRect->setData("rect");
        ui->actionToolEllipse->setData("ellipse");
        ui->actionToolLine->setData("line");
        ui->actionToolText->setData("text");
        ui->actionToolHighlight->setData("highlight");
        ui->actionToolObfuscate->setData("obfuscate");
        ui->actionToolCrop->setData("op-crop");
        ui->actionToolRipOut->setData("op-ripout");
    }

    /**
     * define icons at one place (actions in the designer) and assign them here to the toolbox buttons
     * define other properties at one place so they do not have to set by the QT designer
     */
    void setupToolActionRelatedWidgetProperties()
    {
        foreach (auto action, allToolActions()) {

            action->setCheckable(true);

            QString toolId = actionToToolId(action);
            toolboxButtonFromId(toolId)->setIcon(action->icon());
            if (!toolboxButtonFromId(toolId)->isCheckable()) {
                qDebug() << "forgot to set checkable true... let me do it for you";
                toolboxButtonFromId(toolId)->setCheckable(true); // to avoid that the button "flips back"
            }
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
        else if (toolId == "op-crop") {
            return ui->actionToolCrop;
        }
        else if (toolId == "op-ripout") {
            return ui->actionToolRipOut;
        }
        else {
            qDebug() << "toolActionFromId: TODO  ..." << toolId;
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
        else if (toolId == "op-crop") {
            return ui->pushButtonToolCrop;
        }
        else if (toolId == "op-ripout") {
            return ui->pushButtonToolRipOut;
        }
        else {
            qDebug() << "toolboxButtonFromId: TODO..." << toolId;
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

    /**
     * by action object name
     */
//     QString actionToTool(QObject* action)
//     {
//         QString senderName = action->objectName();
//         QString toolId = senderName.replace("actionTool", "").toLower();
//         return toolId;
//     }

    /**
     * by action data
     */
    QString actionToToolId(QAction* action)
    {
        QString toolId = action->data().toString();
        Q_ASSERT_X(!toolId.isEmpty(), "actionToToolId", "Hint: setupActions must be called once before using this method");
        return toolId;
    }

    void handleSaveImageError(QMainWindow* parent, ErrorStatus errorStatus)
    {
        if (!errorStatus.isEmpty()) {
            QMessageBox::warning(parent, parent->tr("Error saving image"), errorStatus);
        }
    }

public:
    kreen::KreenshotEditorPtr kreenshotEditor;
    Ui::MainWindow* ui;
    QString baseWindowTitle;
};

MainWindow::MainWindow(kreen::KreenshotEditorPtr kreenshotEditor)
{
    d = std::make_shared<MainWindowImpl>();
    d->kreenshotEditor = kreenshotEditor;
    d->ui = new Ui::MainWindow();
    d->ui->setupUi(this);
    d->baseWindowTitle = windowTitle();

    setupUi();
    setupActions();


    connect(d->kreenshotEditor.get(), SIGNAL(outputFileStatusChanged()), this, SLOT(updateOutputFileStatus()));
    updateOutputFileStatus();

    d->kreenshotEditor->mainEditorWidget()->requestTool("select");
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

    d->ui->containerEditor->addWidget(d->kreenshotEditor->mainEditorWidget());

    d->setupActionIds();

    d->setupToolActionRelatedWidgetProperties();
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
    connect(d->ui->actionItemDelete, SIGNAL(triggered()), this, SLOT(selectedItemsDelete()));
    connect(d->ui->actionPreferences, SIGNAL(triggered()), this, SLOT(editPreferences()));
    connect(d->ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));

    connect(d->kreenshotEditor->mainEditorWidget(), SIGNAL(toolChosen(QString)), this, SLOT(toolChosen(QString)));
}

void MainWindow::editPreferences()
{
    d->kreenshotEditor->showPreferencesDialog();
}

void MainWindow::editRedo()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::selectedItemsDelete()
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
    ErrorStatus errorStatus = d->kreenshotEditor->saveToFile();
    d->handleSaveImageError(this, errorStatus);
}

void MainWindow::fileSaveAs()
{
    // TODO: file format handling!
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save file as"), "", tr("Images (*.png)"));
    if (!filepath.isEmpty()) {
        ErrorStatus errorStatus = d->kreenshotEditor->saveToFile(filepath);
        d->handleSaveImageError(this, errorStatus);
    }
}

void MainWindow::helpAbout()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::updateOutputFileStatus()
{
    QString fileStatusPrefix;
    if (d->kreenshotEditor->isFileNew()) {
        fileStatusPrefix = tr("[NEW] ");
    }
    else if (d->kreenshotEditor->isFileModified()) {
        fileStatusPrefix = tr("* ");
    }
    setWindowTitle(QString("%1%2 - %3").arg(fileStatusPrefix).arg(d->kreenshotEditor->outputFilenameManager()->resultingFilepath()).arg(d->baseWindowTitle));
}

void MainWindow::requestTool()
{
    QString className = QObject::sender()->metaObject()->className();
    qDebug() << className;
    Q_ASSERT(className == "QAction");

    QString toolId = d->actionToToolId((QAction*)QObject::sender());
    QString message = QString("MainWindow::requestTool: tool id '%1'").arg(toolId);
    qDebug() << message;

    d->kreenshotEditor->mainEditorWidget()->requestTool(toolId);
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
