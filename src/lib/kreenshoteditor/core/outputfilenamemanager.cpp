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
#include "outputfilenamemanager.h"
#include <QHostInfo>

namespace kreen {
namespace core {

class OutputFilenameManagerImpl
{
public:
    QDateTime captureTime;
    QString description;
    QString user;
    QString hostname;
    QString filenamePattern;
};

OutputFilenameManager::OutputFilenameManager()
{
    d = std::make_shared<OutputFilenameManagerImpl>();

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

void OutputFilenameManager::setFilepathPattern(QString pattern)
{
    d->filenamePattern = pattern;
}

QString OutputFilenameManager::resultingFilepath(QString pattern)
{
    QString result = pattern.isNull() ? d->filenamePattern : pattern;
    QString description = d->description;
    result.replace("${hostname}", d->hostname);
    result.replace("${user}", d->user);
    result.replace("${description}", description);
    result.replace("${description_}", description.replace(" ", "_"));
    result.replace("${YYYY}", d->captureTime.toString("yyyy"));
    result.replace("${MM}", d->captureTime.toString("MM"));
    result.replace("${DD}", d->captureTime.toString("dd"));
    result.replace("${hh}", d->captureTime.toString("HH"));
    result.replace("${mm}", d->captureTime.toString("mm"));
    result.replace("${ss}", d->captureTime.toString("ss"));
    return result;
}

}
}
