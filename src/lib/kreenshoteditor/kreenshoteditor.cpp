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
        //                                                                                     == PathCombine
        outputFilenameGenerator->setFilenamePattern(QDir(outputSettings.defaultOutputDirectory).filePath(outputSettings.filenamePattern));
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

OutputFilenameGeneratorPtr KreenshotEditor::outputFilenameGenerator()
{
    return d->outputFilenameGenerator;
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
