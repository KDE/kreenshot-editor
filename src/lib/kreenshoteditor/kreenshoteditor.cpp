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
#include "kreenshoteditor.h"
#include <QImage>
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "ui/maineditorwidget.h"
#include "core/itemsmanager.h"
#include "core/outputfilenamemanager.h"
#include "core/settingsmanager.h"
#include "ui/settings/preferencesdialog.h"

namespace kreen {

class KreenshotEditorImpl
{
public:
    KreenshotEditorImpl()
    {
        itemsManager = std::make_shared<ItemsManager>();
        outputFilenameManager = std::make_shared<OutputFilenameManager>();
        settingsManager = SettingsManager::instance();
    }

    void settingsToOutputFilenameManager()
    {
        auto outputSettings = settingsManager->output;
        outputFilenameManager->setFilepathPattern(QDir(outputSettings.defaultOutputDirectory).filePath(outputSettings.filenamePattern));
    }

public:
    QImage baseImage;
    ItemsManagerPtr itemsManager;
    OutputFilenameManagerPtr outputFilenameManager;
    SettingsManagerPtr settingsManager;

    MainEditorWidget* mainEditorWidget = nullptr;
};

KreenshotEditor::KreenshotEditor()
{
    d = std::make_shared<KreenshotEditorImpl>();

    d->settingsManager->load();
}

KreenshotEditor::~KreenshotEditor()
{

}

void KreenshotEditor::setBaseImageData(QImage image)
{
    d->baseImage = image;
    d->settingsToOutputFilenameManager();
}

void KreenshotEditor::setBaseImagePath(QString path)
{
    d->baseImage = QImage(path);
    d->outputFilenameManager->setFilepathPattern(path);
}

QImage KreenshotEditor::baseImage()
{
    return d->baseImage;
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

ItemsManagerPtr KreenshotEditor::itemsManager()
{
    return d->itemsManager;
}

void KreenshotEditor::saveToFile(QString filepath)
{
    d->mainEditorWidget->saveToFile(filepath);
}

OutputFilenameManagerPtr KreenshotEditor::outputFilenameManager()
{
    return d->outputFilenameManager;
}

bool KreenshotEditor::isFileNew()
{
    QString filepath = d->outputFilenameManager->resultingFilepath();
    qDebug() << filepath;
    return !QFile::exists(filepath);
}

bool KreenshotEditor::isFileModified()
{
    return true; // TODO
}

void KreenshotEditor::showPreferencesDialog()
{
    ui::settings::PreferencesDialog prefsDialog(d->settingsManager, d->outputFilenameManager);
    if (prefsDialog.exec() == QDialog::Accepted) {
        // only if file is still new (= not stored yet to disk) apply the new file path settings
        if (isFileNew()) {
            qDebug() << "settingsToOutputFilenameManager";
            d->settingsToOutputFilenameManager();
            emit outputFileStatusChanged();
        }
    }
}

}
