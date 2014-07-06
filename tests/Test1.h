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
#ifndef TEST_1_H
#define TEST_1_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QImageReader>
#include "lib/kreenshoteditor/core/impl/outputfilenamegenerator.h"

using namespace kreen::core;

class Test1 : public QObject
{
    Q_OBJECT

public slots:
//     void initTestCase();
//     void cleanupTestCase();

private slots:
    void DateTime()
    {
        QDateTime local(QDateTime::currentDateTime());
        qDebug() << local.toString();
        qDebug() << local.toString("yyyy-MM-dd_HH-mm-ss");
    }

    void FormatString()
    {
        QString a = "/home/user/screenshots/";
//         qDebug() << local.toString();
//         qDebug() << local.toString("yyyy-MM-dd_HH-mm-ss");
    }

    void OutputFilenameGenerator_1()
    {
        OutputFilenameGenerator gen;
        gen.initCaptureTime();
        gen.setDescription("Hallo Test");
        qDebug() << gen.resultingFilename("~/Pictures/screenshots/${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png");
        qDebug() << gen.resultingFilename("${description}, ${hostname}, ${user}");
    }

    void QImageReader_1()
    {
        qDebug() << "QImageReader::supportedImageFormats()" << QImageReader::supportedImageFormats();
    }
};

#endif // TEST_1_H

// kate: indent-width 4; replace-tabs on;
