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

namespace kreen {
namespace core {

class DocumentFileImpl
{
public:
    DocumentPtr doc;
    QString filename;
    DocumentFile::FileStatus fileStatus;
};

DocumentFile::DocumentFile(kreen::core::DocumentPtr doc, QString filename)
{
    KREEN_PIMPL_INIT(DocumentFile)

    d->doc = doc;
    d->filename = filename;
    d->fileStatus = FileStatus_NotCreated;
}

DocumentFile::~DocumentFile()
{

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
        return FileStatus_Saved;
    }
    else {
        return FileStatus_Modified;
    }
}

ErrorStatus DocumentFile::save()
{

}

ErrorStatus DocumentFile::saveAs(QString filename)
{

}

}
}
