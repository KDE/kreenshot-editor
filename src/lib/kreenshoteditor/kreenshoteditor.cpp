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
#include "kreenshoteditor.h"
#include <QImage>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "ui/maineditorwidget.h"
#include "core/document.h"
#include "core/documentfile.h"
#include "core/outputfilenamegenerator.h"
#include "core/settingsmanager.h"
#include "ui/settings/preferencesdialog.h"

namespace kreen {

#define tr(arg) QObject::tr(arg)

class KreenshotEditorImpl
{
public:
    KreenshotEditorImpl(KreenshotEditor* owner_)
    {
        owner = owner_;
        outputFilenameGenerator = std::make_shared<OutputFilenameGenerator>();
        settingsManager = SettingsManager::instance();
    }

    void settingsToOutputFilenameManager()
    {
        auto outputSettings = settingsManager->output;
        //                                                                                     == PathCombine
        outputFilenameGenerator->setFilenamePattern(QDir(outputSettings.defaultOutputDirectory).filePath(outputSettings.filenamePattern));
    }

    QAction* newToolAction(QString toolId, QIcon icon, QString text, QObject* parent, QKeySequence key)
    {
        auto action = new QAction(icon, text, parent);
        action->setCheckable(true);
        action->setShortcut(key);
        action->setData(toolId);
        action->setToolTip(text); // todo: more
        return action;
    }

    /**
     * singleton
     */
    QActionGroup* toolActions()
    {
        if (actionGroup != nullptr) {
            return actionGroup;
        }

        actionGroup = new QActionGroup(owner);
        newToolAction("select", QIcon::fromTheme("edit-select"), tr("Select"), actionGroup, QKeySequence(tr("Esc")));
        newToolAction("rect", QIcon::fromTheme("draw-rectangle"), tr("Rectangle"), actionGroup, QKeySequence(tr("R")));
        newToolAction("ellipse", QIcon::fromTheme("draw-circle"), tr("Ellipse or circle"), actionGroup, QKeySequence(tr("E")));
        newToolAction("line", QIcon::fromTheme("draw-arrow-forward"), tr("Line or arrow"), actionGroup, QKeySequence(tr("L")));
        newToolAction("text", QIcon::fromTheme("draw-text"), tr("Text"), actionGroup, QKeySequence(tr("T")));
        newToolAction("highlight", QIcon::fromTheme("im-status-message-edit"), tr("Highlight"), actionGroup, QKeySequence(tr("H")));
        newToolAction("obfuscate", QIcon::fromTheme("edit-delete-shred"), tr("Obfuscate"), actionGroup, QKeySequence(tr("O")));
        newToolAction("op-crop", QIcon::fromTheme("transform-crop"), tr("Crop"), actionGroup, QKeySequence(tr("Ctrl+Shift+X")));
        newToolAction("op-ripout", QIcon::fromTheme("distribute-vertical-equal"), tr("Rip out"), actionGroup, QKeySequence(tr("Ctrl+Shift+R")));

        return actionGroup;
    }

    QList<QAction*> editActions()
    {
        return QList<QAction*>(); // TODO
    }

public:
    KreenshotEditor* owner = nullptr;

    DocumentFilePtr documentFile;
    OutputFilenameGeneratorPtr outputFilenameGenerator;
    SettingsManagerPtr settingsManager;

    MainEditorWidget* mainEditorWidget = nullptr;
    QActionGroup* actionGroup = nullptr;
};

#undef tr

KreenshotEditor::KreenshotEditor()
{
    KREEN_PIMPL_INIT_THIS(KreenshotEditor);

    d->settingsManager->load();

    // connect tool actions
    foreach (auto action, d->toolActions()->actions()) {

        // select action to tool request
        connect(action, SIGNAL(triggered()), this, SLOT(requestToolBySenderAction()));
    }
}

KreenshotEditor::~KreenshotEditor()
{

}

QString KreenshotEditor::actionToToolId(QAction* action)
{
    QString toolId = action->data().toString();
    Q_ASSERT_X(!toolId.isEmpty(), "actionToToolId", "Hint: setupActions must be called once before using this method");
    return toolId;
}

void KreenshotEditor::requestToolBySenderAction()
{
    QString className = QObject::sender()->metaObject()->className();
    qDebug() << className;
    Q_ASSERT(className == "QAction");

    QString toolId = actionToToolId((QAction*)QObject::sender());
    QString message = QString("MainWindow::requestTool: tool id '%1'").arg(toolId);
    qDebug() << message;

    mainEditorWidget()->requestTool(toolId);
}

void KreenshotEditor::setBaseImageData(QImage image)
{
    d->settingsToOutputFilenameManager();

    auto doc = Document::create(image);
    d->documentFile = std::make_shared<DocumentFile>(doc, d->outputFilenameGenerator->resultingFilename(), d->settingsManager);
}

void KreenshotEditor::setBaseImageFromFile(QString filename)
{
    d->outputFilenameGenerator->setFilenamePattern(filename);

    auto doc = Document::create(QImage(filename));
    d->documentFile = std::make_shared<DocumentFile>(doc, filename, d->settingsManager);
}

DocumentFilePtr KreenshotEditor::documentFile()
{
    return d->documentFile;
}

MainEditorWidget* KreenshotEditor::mainEditorWidget()
{
    if (d->mainEditorWidget == nullptr) {
        auto mainEditorWidget = new MainEditorWidget(shared_from_this());
        //auto mainEditorWidget = new MainEditorWidget(this);

//         bool oldQScrollAreaLogic = false;
//         if (oldQScrollAreaLogic) {
//             auto scrollArea = new QScrollArea();
//             scrollArea->setBackgroundRole(QPalette::Background);
//             //scrollArea->setBackgroundRole(QPalette::Dark);
//             scrollArea->setWidget(mainEditorWidget);
//             d->mainEditorWidget = scrollArea;
//         }
//         else {
        d->mainEditorWidget = mainEditorWidget;
//         }
    }

    return d->mainEditorWidget;
}

QActionGroup* KreenshotEditor::toolActions()
{
    return d->toolActions();
}

QList<QAction*> KreenshotEditor::editActions()
{
    return QList<QAction*>(); // TODO
}

void KreenshotEditor::setCaptureTime(QDateTime datetime)
{
    d->outputFilenameGenerator->initCaptureTime(datetime);
}

void KreenshotEditor::setDescription(QString text)
{
    d->outputFilenameGenerator->setDescription(text);
}

bool KreenshotEditor::isFileNew()
{
    return documentFile()->fileStatus() == DocumentFile::FileStatus_NotCreated;
}

bool KreenshotEditor::isFileModified()
{
    return documentFile()->fileStatus() == DocumentFile::FileStatus_Modified;
}

void KreenshotEditor::showPreferencesDialog()
{
    ui::settings::PreferencesDialog prefsDialog(d->settingsManager, d->outputFilenameGenerator);
    if (prefsDialog.exec() == QDialog::Accepted) {
        // only if file is still new (= not stored yet to disk) apply the new file path settings
        if (isFileNew()) {
            qDebug() << "settingsToOutputFilenameManager";
            d->settingsToOutputFilenameManager();
        }
    }
}

}
