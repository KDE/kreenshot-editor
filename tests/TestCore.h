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
#include <QFileInfo>
#include "lib/kreenshoteditor/core/document.h"
#include "lib/kreenshoteditor/core/documentfile.h"
#include "lib/kreenshoteditor/core/impl/outputfilenamegenerator.h"
#include "lib/kreenshoteditor/core/settingsmanager.h"

using namespace kreen::core;

class TestCore : public QObject
{
    Q_OBJECT

private:
    QString inputImage1Filename = "./testdata/image1.png";
    SettingsManagerPtr settingsManager = SettingsManager::instance("unittest");

public slots:
     void initTestCase()
     {
         // do not do any interactive things when saving the document
         settingsManager->output.afterSaveOpenDefaultViewer = false;
         settingsManager->output.afterSaveOpenFileBrowser = false;
         settingsManager->output.afterSaveClipboardFilename = false;
         settingsManager->output.afterSaveClipboardImageData = false;
     }

//     void cleanupTestCase();

private slots:
    void DocumentFile_init_modifydoc_save_modifydoc_save()
    {
        QFileInfo file(inputImage1Filename);
        qDebug() << file.absoluteFilePath(); // this will be relative build/tests

        auto doc = Document::make_shared(QImage(inputImage1Filename));
        QString filename = "./testdata/output_docfile1.png";
        DocumentFile docFile(doc, filename, DocumentFile::FileStatus_NotCreated);
        docFile.setSettingsManager(settingsManager);
        QCOMPARE(docFile.document(), doc);
        QCOMPARE(docFile.filename(), filename);
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_NotCreated);

        doc->addItem(KreenItem::create("line"));
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_NotCreated); // no change because file is not saved

        QCOMPARE(docFile.save(), QString());
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_CreatedAndClean);

        doc->addItem(KreenItem::create("rect"));
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_CreatedAndModified);

        docFile.save();
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_CreatedAndClean);
    }

    void DocumentFile_init_modifydoc_saveas()
    {
        auto doc = Document::make_shared(QImage(inputImage1Filename));
        DocumentFile docFile(doc, inputImage1Filename, DocumentFile::FileStatus_NotCreated);
        docFile.setSettingsManager(settingsManager);

        QString filename = "./testdata/output_docfile2.png";
        QCOMPARE(docFile.saveAs(filename), QString());
        QCOMPARE(docFile.fileStatus(), DocumentFile::FileStatus_CreatedAndClean);
        QCOMPARE(docFile.filename(), filename);
    }

    void DocumentFile_save_create_intermediate_directories()
    {
        qDebug() << "TODO: write test case to show that intermediate dirs are created by save()";
    }

    void DocumentFile_supportedImageFormats()
    {
        QCOMPARE(DocumentFile::supportedImageFormats().first(), QString("bmp"));
    }

    void OutputFilenameManager_resultingFilename_description()
    {
        OutputFilenameGenerator gen;
        gen.setDescription("Hallo Test");
        QCOMPARE(gen.resultingFilename("aaa${description}.png"), QString("aaaHallo Test.png"));
        QCOMPARE(gen.resultingFilename("aaa${description_}.png"), QString("aaaHallo_Test.png"));

        gen.setDescription("a/b\\c d");
        QCOMPARE(gen.resultingFilename("${description}"), QString("a_b_c d"));
    }

    void OutputFilenameManager_resultingFilename_manual()
    {
        OutputFilenameGenerator gen;
        gen.initCaptureTime();
        gen.setDescription("Hallo Test");
        qDebug() << "[CHECK VISUALLY]:";
        qDebug() << gen.resultingFilename("~/Pictures/screenshots/${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png");
        qDebug() << gen.resultingFilename("${description}, ${hostname}, ${user}");

    }
};

#endif

// kate: indent-mode cstyle; replace-tabs on;
