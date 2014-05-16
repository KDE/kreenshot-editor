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
#include "outputfilenamemanager.h"
#include <QHostInfo>

class OutputFilenameManagerImpl
{
public:
    QDateTime captureTime;
    QString description;
    QString user;
    QString hostname;
};

OutputFilenameManager::OutputFilenameManager()
{
    d = OutputFilenameManagerImplPtr(new OutputFilenameManagerImpl());

    d->hostname = QHostInfo().localHostName();

    d->user = qgetenv("USER"); // get the user name in Linux
}

OutputFilenameManager::~OutputFilenameManager()
{

}

void OutputFilenameManager::initCaptureTime(QDateTime datetime)
{
    if (datetime.isValid()) {
        d->captureTime = datetime;
    }
    else {
        d->captureTime = QDateTime::currentDateTime();
    }
}

void OutputFilenameManager::setDescription(QString text)
{
    d->description = text;
}

QString OutputFilenameManager::getFilepath(QString pattern)
{
    QString result = pattern;
    QString description = d->description;
    result.replace("${hostname}", d->hostname);
    result.replace("${user}", d->user);
    result.replace("${description}", description);
    result.replace("${description_}", description.replace(" ", "_"));
    return result;
}
