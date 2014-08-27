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
#ifndef CORE_DOCUMENTFILE_H
#define CORE_DOCUMENTFILE_H

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include "document.h"
#include <kreen/core/errorstatus.h>
#include <memory>
#include <QString>

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(DocumentFile)
KREEN_SHAREDPTR_FORWARD_DECL(SettingsManager)

/**
 * Connects a Document instance with a file on disk
 */
class KREEN_DECL_EXPORT DocumentFile : public QObject
{
    Q_OBJECT

public:
    enum FileStatus
    {
        FileStatus_NotCreated,
        FileStatus_CreatedAndClean,
        FileStatus_CreatedAndModified
    };

    /**
     * returns the supported images format file extensions that can be used for the filename
     */
    static QList<QString> supportedImageFormats();

public:
    /**
     * Inits the DocumentFile with a Document and a filename (i. e. path to file).
     * The initial FileStatus given should be either FileStatus_NotCreated or FileStatus_CreatedAndClean
     */
    DocumentFile(kreen::core::DocumentPtr doc, QString filename, FileStatus fileStatus);

    virtual ~DocumentFile();

    /**
     * used for performing the after save action
     * (todo: could be done via signal/slot)
     */
    void setSettingsManager(SettingsManagerPtr settingsManager);

    /**
     * associated document
     */
    DocumentPtr document();

    /**
     * filename of existing file or proposed filename (i.e. with or without path)
     */
    QString filename();

    /**
     * Only allowed if file is not created yet. Emits fileStatusChanged().
     */
    void setProposedFilename(QString filename);

    /**
     * TODO: make better because only FileStatus_NotCreated is used at the moment
     *          otherwise confusion
     */
    FileStatus fileStatus();

    /**
     * save file to filename() (without asking if file already exists)
     * sets Document clean if saving succeeds
     */
    ErrorStatus save();

    /**
     * save file to filename (without asking if file already exists)
     * sets Document clean if saving succeeds
     * filename() then will return the new filename, if saving succeeds
     */
    ErrorStatus saveAs(QString filename);

    void copyFilenameToClipboard();

    void selectInFilemanager();

    void launchDefaultImageViewer();

Q_SIGNALS:
    /**
     * when status of file has changed (after save() or saveAs() is called, NOT by ctor)
     */
    void fileStatusChanged();

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
