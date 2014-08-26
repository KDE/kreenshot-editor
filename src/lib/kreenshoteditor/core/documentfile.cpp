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
#include "documentfile.h"
#include <QDebug>
#include <QImageReader>
#include <QPainter>
#include <QImageWriter>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include "core/settingsmanager.h"
#include "core/desktopservices.h"

#define tr(arg) QObject::tr(arg)

namespace kreen {
namespace core {

class DocumentFile::Impl
{
public:
    Impl(DocumentFile* owner_)
    {
        owner = owner_;
    }

public:
    DocumentFile* owner;
    DocumentPtr doc;
    SettingsManagerPtr settingsManager;
    QString filename;
    /**
     * WARN: the corresponding getter method also used the document's status to calc the returned value!
     */
    DocumentFile::FileStatus fileStatus;

public:
    /**
     * returns error message or empty if succeeded
     */
    ErrorStatus saveToFile(QString filename)
    {
        qDebug() << "DocumentFileImpl.saveToFile: " << filename;

        QImage image = doc->renderToImage();

        // create intermediate directories
        QFileInfo fileInfo(filename);
        fileInfo.dir().mkpath(".");

        QImageWriter writer(filename);
        writer.write(image);
        qDebug() << "error code: " << writer.error();
        if (writer.error() != 0) {
            qDebug() << "error: " << writer.errorString();
            // throw std::runtime_error(
            return tr("Error saving image ('%1'): '%2' (code %3)")
                   .arg(filename)
                   .arg(writer.errorString())
                   .arg(writer.error());
        }

        return ErrorStatus();
    }

    /**
     * filename can be with out without path
     */
    void afterSaveAction(QString filename)
    {
        if (!settingsManager) {
            qDebug() << "[INFO] DocumentFile.afterSaveAction will not work because settingsManager is nullptr.";
            Q_ASSERT(false);
            return;
        }

        if (settingsManager->output.afterSaveOpenDefaultViewer) {
            owner->launchDefaultImageViewer();
        }

        if (settingsManager->output.afterSaveOpenFileBrowser) {
            owner->selectInFilemanager();
        }

        if (settingsManager->output.afterSaveClipboardFilename) {
            owner->copyFilenameToClipboard();
        } else if (settingsManager->output.afterSaveClipboardImageData) {
            doc->copyImageToClipboard();
        }
    }

    void afterSaveToFile(ErrorStatus errorStatus)
    {
        if (errorStatus.isEmpty()) {
            afterSaveAction(filename);
        }

        owner->emit fileStatusChanged();
    }
};

QList<QString> DocumentFile::supportedImageFormats()
{
    auto origList = QImageReader::supportedImageFormats();
    QList<QString> list;
    foreach (auto item, origList) {
        list << QString(item);
    }
    return list;
}

DocumentFile::DocumentFile(kreen::core::DocumentPtr doc, QString filename, FileStatus fileStatus)
{
    KREEN_PIMPL_INIT_THIS(DocumentFile)

    d->doc = doc;
    d->filename = filename;
    d->fileStatus = fileStatus;
}

DocumentFile::~DocumentFile()
{

}

void DocumentFile::setSettingsManager(SettingsManagerPtr settingsManager)
{
    d->settingsManager = settingsManager;
}

DocumentPtr DocumentFile::document()
{
    return d->doc;
}

QString DocumentFile::filename()
{
    return d->filename;
}

DocumentFile::FileStatus DocumentFile::fileStatus()
{
    if (d->fileStatus == FileStatus_NotCreated) {
        return d->fileStatus;
    }
    else if (d->doc->isClean()) {
        return FileStatus_CreatedAndClean;
    }
    else {
        return FileStatus_CreatedAndModified;
    }
}

ErrorStatus DocumentFile::save()
{
    qDebug() << "DocumentFile::save()";

    auto errorStatus = d->saveToFile(filename());
    if (errorStatus.isEmpty()) {
        d->fileStatus = FileStatus_CreatedAndClean;
        d->doc->setClean();
    }
    else {
        qDebug() << errorStatus;
    }

    d->afterSaveToFile(errorStatus);
    return errorStatus;
}

ErrorStatus DocumentFile::saveAs(QString filename)
{
    auto errorStatus = d->saveToFile(filename);
    if (errorStatus.isEmpty()) {
        d->filename = filename;
        d->fileStatus = FileStatus_CreatedAndClean;
        d->doc->setClean();
    }
    else {
        qDebug() << errorStatus;
    }

    d->afterSaveToFile(errorStatus);
    return errorStatus;
}

void DocumentFile::copyFilenameToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(d->filename);
}

void DocumentFile::selectInFilemanager()
{
    kreen::core::DesktopServices::selectInFilemanager(d->filename);
}

void DocumentFile::launchDefaultImageViewer()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(d->filename));
}

}
}
