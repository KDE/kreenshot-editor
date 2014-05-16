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
#ifndef CORE_OUTPUTFILENAMEMANAGER_H
#define CORE_OUTPUTFILENAMEMANAGER_H

#include <memory>
#include <QString>
#include <QDateTime>

class OutputFilenameManager;
typedef std::shared_ptr<OutputFilenameManager> OutputFilenameManagerPtr;

class OutputFilenameManagerImpl;
typedef std::shared_ptr<OutputFilenameManagerImpl> OutputFilenameManagerImplPtr;

/**
 * Filename pattern similar to greenshot (see greenshot Preferences --> Output)
 * ${YYYY} year, 4 digits
 * ${MM} month, 2 digits
 * ${DD} day, 2 digits
 * ${hh} hour, 2 digits
 * ${mm} minute, 2 digits
 * ${ss} second, 2 digits
 * ${description}: e. g. window title (replaces disallowed characters with _)
 * ${description_}: like ${description_} but replace all spaces with _
 * ${user} system user
 * ${hostname} PC name
 * NOT SUPPORTED YET: ${NUM} incrementing number, 6 digits
 * NOT SUPPORTED: ${domain} Windows domain*
 * NOT SUPPORTED: ${title} Window title
 *
 * The pattern can be used in the complete path to the output file not only in the filename itself.
 */
class OutputFilenameManager
{
public:
    /**
     * sets ${user}, ${hostname}
     */
    OutputFilenameManager();

    virtual ~OutputFilenameManager();

    /**
     * sets ${YYYY}, ${MM}, ${DD}, ${hh}, ${mm}, ${ss}
     * if datetime is not set the current time is used
     */
    void initCaptureTime(QDateTime datetime = QDateTime());

    /**
     * sets ${description}
     */
    void setDescription(QString text);

    /**
     * set pattern or fixed path
     */
    void setFilenamePattern(QString pattern);

    /**
     * location + filename
     * if pattern is not set then the one set by setPattern is used
     */
    QString getFilepath(QString pattern = QString());

public:

private:
    OutputFilenameManagerImplPtr d;
};


#endif // CORE_OUTPUTFILENAMEMANAGER
