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
#include "desktopservices.h"
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QProcess>

namespace kreen {
namespace core {

DesktopServices::DesktopServices()
{

}

DesktopServices::~DesktopServices()
{

}

void DesktopServices::selectInFilemanager(QString filename)
{
    QStringList args;
    args << "--select" << filename;
    if (!QProcess::startDetached("/usr/bin/dolphin", args))
    {
        QDesktopServices::openUrl(QFileInfo(filename).dir().absolutePath());
    }

//    QString program = ;
//      QStringList arguments;
//      arguments << "-style" << "fusion";
//      auto myProcess = new QProcess(this);
//      connect(myProcess, SIGNAL(started()), this, SLOT(slotProcessStarted()));
//      connect(myProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slotProcessError(QProcess::ProcessError)));
//      myProcess->start(program, arguments);
}

// void DesktopServices::slotProcessStarted()
// {
//     qDebug() << "started";
// }
//
// void DesktopServices::slotProcessError(QProcess::ProcessError error)
// {
//     qDebug() << "error: " << error;
// }


}
}
