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
#ifndef TEST_CORE_H
#define TEST_CORE_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTest>
#include "lib/kreenshoteditor/core/document.h"
#include "lib/kreenshoteditor/core/documentfile.h"
#include "lib/kreenshoteditor/core/outputfilenamemanager.h"

using namespace kreen::core;

class TestCore : public QObject
{
    Q_OBJECT

public slots:
//     void initTestCase();
//     void cleanupTestCase();

private slots:
    void DocumentFile_1()
    {
        auto doc = Document::create();
        QString filename = "./testdata/testcore/docfile1.png";
        DocumentFile docFile(doc, filename);
        QCOMPARE(docFile.document(), doc);
        QCOMPARE(docFile.filename(), filename);
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_NotCreated);
    }

    void OutputFilenameManager_resultingFilepath_description()
    {
        OutputFilenameManager mgr;
        mgr.setDescription("Hallo Test");
        QCOMPARE(mgr.resultingFilepath("aaa${description}.png"), QString("aaaHallo Test.png"));
        QCOMPARE(mgr.resultingFilepath("aaa${description_}.png"), QString("aaaHallo_Test.png"));

        mgr.setDescription("a/b\\c d");
        QCOMPARE(mgr.resultingFilepath("${description}"), QString("a_b_c d"));
    }

    void OutputFilenameManager_resultingFilepath_manual()
    {
        OutputFilenameManager mgr;
        mgr.initCaptureTime();
        mgr.setDescription("Hallo Test");
        qDebug() << "[CHECK VISUALLY]:";
        qDebug() << mgr.resultingFilepath("~/Pictures/screenshots/${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png");
        qDebug() << mgr.resultingFilepath("${description}, ${hostname}, ${user}");

    }
};

#endif // TEST_CORE_H

// kate: indent-mode cstyle; replace-tabs on;
