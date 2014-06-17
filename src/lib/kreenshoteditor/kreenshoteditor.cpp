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
#include <QUrl>
#include <QDesktopServices>
#include <QClipboard>
#include <QApplication>
#include "ui/maineditorwidget.h"
#include "core/document.h"
#include "core/documentfile.h"
#include "core/outputfilenamegenerator.h"
#include "core/settingsmanager.h"
#include "ui/settings/preferencesdialog.h"

namespace kreen {

class KreenshotEditorImpl
{
public:
    KreenshotEditorImpl()
    {
        outputFilenameGenerator = std::make_shared<OutputFilenameGenerator>();
        settingsManager = SettingsManager::instance();
    }

    void settingsToOutputFilenameManager()
    {
        auto outputSettings = settingsManager->output;
        outputFilenameGenerator->setFilepathPattern(QDir(outputSettings.defaultOutputDirectory).filePath(outputSettings.filenamePattern));
    }

    void afterSaveAction(QString filepath)
    {
        if (settingsManager->output.afterSaveOpenDefaultViewer) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
        }

        if (settingsManager->output.afterSaveOpenFileBrowser) {
            QDesktopServices::openUrl(QFileInfo(filepath).dir().absolutePath());

            // TODO: use QProcess to lookup if dolphin exists and use it, else use the existing method
        }

        if (settingsManager->output.afterSaveClipboardFilepath) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(filepath);
        } else if (settingsManager->output.afterSaveClipboardImageData) {
            QImage image(filepath);
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setImage(image);
        }
    }

public:
    DocumentFilePtr documentFile;
    OutputFilenameGeneratorPtr outputFilenameGenerator;
    SettingsManagerPtr settingsManager;

    MainEditorWidget* mainEditorWidget = nullptr;
};

KreenshotEditor::KreenshotEditor()
{
    KREEN_PIMPL_INIT(KreenshotEditor);

    d->settingsManager->load();
}

KreenshotEditor::~KreenshotEditor()
{

}

void KreenshotEditor::setBaseImageData(QImage image)
{
    d->settingsToOutputFilenameManager();

    auto doc = Document::create(image);
    d->documentFile = std::make_shared<DocumentFile>(doc, d->outputFilenameGenerator->resultingFilepath());
}

void KreenshotEditor::setBaseImageFromFile(QString filename)
{
    d->outputFilenameGenerator->setFilepathPattern(filename);

    auto doc = Document::create(QImage(filename));
    d->documentFile = std::make_shared<DocumentFile>(doc, filename);
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

OutputFilenameGeneratorPtr KreenshotEditor::outputFilenameManager()
{
    return d->outputFilenameGenerator;
}

bool KreenshotEditor::isFileNew()
{
    QString filepath = d->outputFilenameGenerator->resultingFilepath();
    qDebug() << filepath;
    return !QFile::exists(filepath);
}

bool KreenshotEditor::isFileModified()
{
    return true; // TODO
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
