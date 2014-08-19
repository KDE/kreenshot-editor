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
#include <QCloseEvent>
#include <kreen/kreenshoteditor.h>
#include <kreen/ui/maineditorwidget.h>
#include <kreen/core/documentfile.h>

class MainWindowImpl
{
public:
    void setupActionsMenuAndToolbar()
    {
        // NOTE: to get a list of all available actions, see KreenshotEditor::allActionIds()
        //

        //
        // fill file actions
        //
        QStringList fileActions;
        // all document and file actions
        fileActions << "document-new" << "document-open"
            << "---"
            << "document-save" << "document-save-as"
            << "---"
            << "document-image-to-clipboard"
            << "document-filename-to-clipboard"
            << "document-file-select-in-filemanager"
            << "document-launch-default-viewer"
            ;
        insertEditActionsForPlaceholder(ui->menuFile, ui->actionFileActionsPlaceholder, fileActions);
        // in the toolbar we don't want to have all items
        fileActions.removeAll("document-new");
        fileActions.removeAll("document-image-to-clipboard");
        fileActions.removeAll("document-filename-to-clipboard");
        fileActions.removeAll("document-file-select-in-filemanager");
        fileActions.removeAll("document-launch-default-viewer");
        fileActions.removeLast(); // "---"
        insertEditActionsForPlaceholder(ui->toolBar_Main, ui->actionFileActionsPlaceholder, fileActions);

        //
        // fill edit actions
        //
        QStringList editActions;
        editActions << "edit-objects-select-all" << "edit-objects-delete"; // all edit actions
        insertEditActionsForPlaceholder(ui->menuEdit, ui->actionEditActionsPlaceholder, editActions);
        editActions.removeAll("edit-objects-select-all"); // in the toolbar we don't want to have all items, so we remove this one
        insertEditActionsForPlaceholder(ui->toolBar_Main, ui->actionEditActionsPlaceholder, editActions);

        //
        // fill menuTool and toolBar_Tools
        //
        foreach(auto action, kreenshotEditor->toolActions()) {
            toolIdToActionMap.insert(actionToToolId(action), action);
        }

        insertToolsActionsForPlaceholder(ui->menuTool, ui->actionToolsActionsPlaceholder);
        insertToolsActionsForPlaceholder(ui->toolBar_Tools, ui->actionToolsActionsPlaceholder);

        insertUndoActionsForPlaceholder(ui->menuEdit, ui->actionUndoActionsPlaceholder);
        insertUndoActionsForPlaceholder(ui->toolBar_Main, ui->actionUndoActionsPlaceholder);
    }

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
        foreach (auto action, kreenshotEditor->toolActions()) {
            if (!actionToToolId(action).startsWith("op-") && !(actionToToolId(action) == "select")) {
                parent->insertAction(placeholder, action);
            }
        }
        insertSeparator(placeholder, parent);
        foreach (auto action, kreenshotEditor->toolActions()) {
            if (actionToToolId(action).startsWith("op-")) {
                parent->insertAction(placeholder, action);
            }
        }

        parent->removeAction(placeholder);
    }

    void insertUndoActionsForPlaceholder(QWidget* parent, QAction* placeholder)
    {
        parent->insertAction(placeholder, kreenshotEditor->actionFromId("edit-undo"));
        parent->insertAction(placeholder, kreenshotEditor->actionFromId("edit-redo"));
        parent->removeAction(placeholder);
    }

    /**
     * special case: "---" is to add a separator
     */
    void insertEditActionsForPlaceholder(QWidget* parent, QAction* placeholder, QStringList actionIds)
    {
        foreach (auto actionId, actionIds) {
            if (actionId == "---") {
                insertSeparator(placeholder, parent);
            }
            else {
                parent->insertAction(placeholder, kreenshotEditor->actionFromId(actionId));
            }
        }
        parent->removeAction(placeholder);
    }

    /**
     * define icons at one place (actions in the designer) and assign them here to the toolbox buttons
     * define other properties at one place so they do not have to set by the QT designer
     */
    void setupToolActionRelatedWidgetProperties()
    {
        foreach (auto action, kreenshotEditor->toolActions()) {

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

    /**
     * from action data
     */
    QString actionToToolId(QAction* action)
    {
        return kreenshotEditor->actionToToolId(action);
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

    connect(d->kreenshotEditor.get(), SIGNAL(documentFileStatusChangedSignal()), this, SLOT(slotUpdateDocumentFileStatus()));
    slotUpdateDocumentFileStatus(); // initial call. todo: still needed?

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

    setAcceptDrops(true);
}

void MainWindow::setupActions()
{
    foreach (auto action, d->kreenshotEditor->toolActions()) {
        QString toolId = d->actionToToolId(action);

        // button click to action
        connect(d->toolboxButtonFromId(toolId), SIGNAL(clicked()), action, SLOT(trigger()));

        // action toogled to button
        connect(action, SIGNAL(toggled(bool)), d->toolboxButtonFromId(toolId), SLOT(setChecked(bool)));
    }

    connect(d->ui->actionPreferences, SIGNAL(triggered()), this, SLOT(slotEditPreferences()));
    connect(d->ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotHelpAbout()));
    // actionQuit: connected via Action Editor in designer

    connect(d->kreenshotEditor->mainEditorWidget(), SIGNAL(toolChosenSignal(QString)), this, SLOT(slotToolChosen(QString)));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (d->kreenshotEditor->warnIfDocumentIsNotClean_shouldContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    d->kreenshotEditor->dragEnterEventMainWindow(event);
}

void MainWindow::dropEvent(QDropEvent* event)
{
    d->kreenshotEditor->dropEventMainWindow(event);
}

void MainWindow::slotEditPreferences()
{
    d->kreenshotEditor->showPreferencesDialog();
}

void MainWindow::slotHelpAbout()
{
    QMessageBox::information(this, "Not impl", "Not implemented yet");
}

void MainWindow::slotUpdateDocumentFileStatus()
{
    qDebug() << "MainWindow::slotUpdateDocumentFileStatus()";
    
    QString fileStatusPrefix;
    if (d->kreenshotEditor->isFileNew()) {
        fileStatusPrefix = tr("[NEW] ");
    }
    else if (d->kreenshotEditor->isFileModified()) {
        fileStatusPrefix = tr("* ");
    }

    setWindowTitle(QString("%1%2 - %3").arg(fileStatusPrefix).arg(d->kreenshotEditor->documentFile()->filename()).arg(d->baseWindowTitle));
}

void MainWindow::slotToolChosen(QString toolId)
{
    qDebug() << "MainWindow::slotToolChosen: " << toolId;

    auto toolAction = d->toolActionFromId(toolId);
    toolAction->setChecked(true); // the other items will be unchecked because of QActionGroup
}
