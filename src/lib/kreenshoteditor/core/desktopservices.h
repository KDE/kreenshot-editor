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
#ifndef UUID_b000eb02_145d_11e4_bace_002454dd224f
#define UUID_b000eb02_145d_11e4_bace_002454dd224f

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include <QString>
#include <QUrl>
#include <QProcess>
#include <QObject>

namespace kreen {
namespace core {

/**
 * additions to QDesktopServices
 */
class KREEN_DECL_EXPORT DesktopServices : public QObject
{
    Q_OBJECT
public:
    DesktopServices();

    virtual ~DesktopServices();

public:
    /**
     * Selects the given file in the filemanager (works only for dolphin).
     * If the url is a directory it is also selected instead of entered.
     */
    static void selectInFilemanager(QString filename);

protected Q_SLOTS:
//     void slotProcessStarted();
//     void slotProcessError(QProcess::ProcessError error);
};

}
}

#endif
