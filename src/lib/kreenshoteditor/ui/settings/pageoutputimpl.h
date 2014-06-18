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
#ifndef UI_SETTINGS_PAGEOUTPUTIMPL_H
#define UI_SETTINGS_PAGEOUTPUTIMPL_H

#include <QString>
#include <QStringRef>

namespace kreen {
namespace ui {
namespace settings {

class PageOutputImplHeader {
public:
    static void filenameToFilenamePlusExt(const QString filename_in, QString* filename_out, QString* ext_out)
    {
        int i = filename_in.lastIndexOf(".");
        if (i == -1) {
            *filename_out = filename_in;
            *ext_out = "";
        }
        else {
            QStringRef ref(&filename_in);
            *filename_out = ref.left(i).toString();
            *ext_out = ref.right(i + 1).toString();
        }
    }

    static QString filenamePlusExtToFilename(QString filename_noext, QString ext)
    {
        return filename_noext + "." + ext;
    }
};

}
}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
