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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QUrl>
#include <QFileDialog>
#include <kreen/kreenshoteditor.h>
#include <kreen/ui/maineditorwidget.h>
#include <kreen/core/documentfile.h>

class MainWindowImpl
{
public:
    void insertSeparator(QAction* before, QWidget* w)
    {
        auto sep = new QAction(w);
        sep->setSeparator(true);
        w->insertAction(before, sep);
    }

    void insertToolsActionsForPlaceholder(QWidget* parent, QAction* placeholder)
    {
        parent->insertAction(placeholder, toolIdToActionMap.value("select"));

        insertSeparator(placeholder, parent);
        foreach (auto action, allToolActions()) {
            if (!actionToToolId(action).startsWith("op-") && !(actionToToolId(action) == "select")) {
                parent->insertAction(placeholder, action);
            }
        }
        insertSeparator(placeholder, parent);
        foreach (auto action, allToolActions()) {
            if (actionToToolId(action).startsWith("op-")) {
                parent->insertAction(placeholder, action);
            }
        }

        parent->removeAction(placeholder);
    }

    void insertUndoActionsForPlaceholder(QWidget* parent, QAction* placeholder)
    {
        foreach (auto action, kreenshotEditor->undoActions()) {
            parent->insertAction(placeholder, action);
        }
        parent->removeAction(placeholder);
    }

    void insertEditActionsForPlaceholder(QWidget* parent, QAction* placeholder)
    {
        foreach (auto action, kreenshotEditor->editActions()) {
            parent->insertAction(placeholder, action);
        }
        parent->removeAction(placeholder);
    }

    void setupActionsMenuAndToolbar()
    {
        foreach(auto action, allToolActions()) {
            toolIdToActionMap.insert(actionToToolId(action), action);
        }

        // fill menuTool and toolBar_Tools
        //
        insertToolsActionsForPlaceholder(ui->menuTool, ui->actionToolsActionsPlaceholder);
        insertToolsActionsForPlaceholder(ui->toolBar_Tools, ui->actionToolsActionsPlaceholder);

        insertUndoActionsForPlaceholder(ui->menuEdit, ui->actionUndoActionsPlaceholder);
        insertUndoActionsForPlaceholder(ui->toolBar_Main, ui->actionUndoActionsPlaceholder);

        insertEditActionsForPlaceholder(ui->menuEdit, ui->actionEditActionsPlaceholder);
        insertEditActionsForPlaceholder(ui->toolBar_Main, ui->actionEditActionsPlaceholder);
    }

    /**
     * define icons at one place (actions in the designer) and assign them here to the toolbox buttons
     * define other properties at one place so they do not have to set by the QT designer
     */
    void setupToolActionRelatedWidgetProperties()
    {
        foreach (auto action, allToolActions()) {

            QString toolId = actionToToolId(action);
            toolboxButtonFromId(toolId)->setIcon(action->icon());
            if (!toolboxButtonFromId(toolId)->isCheckable()) {
                qDebug() << "toolbutton: set isCheckable to true (because it was not set in designer)";
                toolboxButtonFromId(toolId)->setCheckable(true); // to avoid that the button "flips back"
            }
        }
    }

    QAction* toolActionFromId(QString toolId)
    {
        if (toolIdToActionMap.contains(toolId)) {
            return toolIdToActionMap.value(toolId);
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

    QList<QAction*> allToolActions()
    {
        return kreenshotEditor->toolActions()->actions();
    }

    /**
     * from action data
     */
    QString actionToToolId(QAction* action)
    {
        return kreenshotEditor->actionToToolId(action);
    }

    void handleSaveImageError(QMainWindow* parent, ErrorStatus errorStatus)
    {
        if (!errorStatus.isEmpty()) {
            QMessageBox::warning(parent, parent->tr("Error saving image"), errorStatus);
        }
    }

    QString imageFileExtensionsToGetSaveFileNameFilterString(QStringList fileextlist)
    {
        QString result;
        foreach (QString str, fileextlist)
        {
            result += "*." + str + " ";
        }
        return result;
    }

public:
    kreen::KreenshotEditorPtr kreenshotEditor;
    Ui::MainWindow* ui;
    QString baseWindowTitle;
    QMap<QString, QAction*> toolIdToActionMap;
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

    connect(d->kreenshotEditor->documentFile().get(), SIGNAL(fileStatusChanged()), this, SLOT(updateOutputFileStatus()));
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

    d->setupActionsMenuAndToolbar();

    d->setupToolActionRelatedWidgetProperties();
}

void MainWindow::setupActions()
{
    foreach (auto action, d->allToolActions()) {
        QString toolId = d->actionToToolId(action);

        // button click to action
        connect(d->toolboxButtonFromId(toolId), SIGNAL(clicked()), action, SLOT(trigger()));

        // action toogled to button
        connect(action, SIGNAL(toggled(bool)), d->toolboxButtonFromId(toolId), SLOT(setChecked(bool)));
    }

    connect(d->ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(d->ui->actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
    // actionQuit: via Action Editor in designer
    connect(d->ui->actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(d->ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(d->ui->actionPreferences, SIGNAL(triggered()), this, SLOT(editPreferences()));
    connect(d->ui->actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));

    connect(d->kreenshotEditor->mainEditorWidget(), SIGNAL(toolChosen(QString)), this, SLOT(toolChosen(QString)));
}

void MainWindow::editPreferences()
{
    d->kreenshotEditor->showPreferencesDialog();
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
    ErrorStatus errorStatus = d->kreenshotEditor->documentFile()->save();
    d->handleSaveImageError(this, errorStatus);
}

void MainWindow::fileSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file as"),
                                                    d->kreenshotEditor->documentFile()->filename(),
                                                    tr("Images") + " ("
                                                   + d->imageFileExtensionsToGetSaveFileNameFilterString(DocumentFile::supportedImageFormats())
                                                   + ")");
    if (!filename.isEmpty()) {
        ErrorStatus errorStatus = d->kreenshotEditor->documentFile()->saveAs(filename);
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

    setWindowTitle(QString("%1%2 - %3").arg(fileStatusPrefix).arg(d->kreenshotEditor->documentFile()->filename()).arg(d->baseWindowTitle));
}

void MainWindow::toolChosen(QString toolId)
{
    qDebug() << "MainWindow::toolChosen: " << toolId;

    auto toolAction = d->toolActionFromId(toolId);
    toolAction->setChecked(true); // the other items will be unchecked because of QActionGroup
}
