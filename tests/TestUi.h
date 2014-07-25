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
#ifndef TEST_UI_H
#define TEST_UI_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTest>
#include <QFileInfo>
#include "lib/kreenshoteditor/ui/settings/pageoutput.h"
#include "lib/kreenshoteditor/ui/settings/pageoutputimpl.h"

using namespace kreen::ui;
using namespace kreen::ui::settings;

class TestUi : public QObject
{
    Q_OBJECT

private:

public slots:
//     void initTestCase()
//     void cleanupTestCase();

private slots:
    void PageOutputImpl_filenameToFilenamePlusExt_1()
    {
        QString filename, ext;
        PageOutputImplHeader::filenameToFilenamePlusExt("abc.def-ghi.png", &filename, &ext);
        QCOMPARE(filename, QString("abc.def-ghi"));
        QCOMPARE(ext, QString("png"));
    }

    void PageOutputImpl_filenameToFilenamePlusExt_2()
    {
        QString filename, ext;
        PageOutputImplHeader::filenameToFilenamePlusExt("abc.def-ghi_AAA", &filename, &ext);
        QCOMPARE(filename, QString("abc"));
        QCOMPARE(ext, QString("def-ghi_AAA"));
    }

    void PageOutputImpl_filenameToFilenamePlusExt_3()
    {
        QString filename, ext;
        PageOutputImplHeader::filenameToFilenamePlusExt("abc-def-ghi", &filename, &ext);
        QCOMPARE(filename, QString("abc-def-ghi"));
        QCOMPARE(ext, QString(""));
    }

    void PageOutputImpl_filenamePlusExtToFilename()
    {
        QCOMPARE(PageOutputImplHeader::filenamePlusExtToFilename("abc.def-ghi", "png"), QString("abc.def-ghi.png"));
    }

    void ThemeIcons()
    {
        qDebug() << "ThemeIcons()";

        qDebug() << "QIcon::themeName(): " << QIcon::themeName();
        // not breeze because there are currently no icons (2014-07-25)
        // The icon theme can be set using the "Icons" KDE Control Module
        QCOMPARE(QIcon::themeName(), QString("oxygen"));

        foreach (QString path, QIcon::themeSearchPaths()) {
            qDebug() << "path: " << path;
        }

        auto icon1 = QIcon::fromTheme("edit-select");
        QCOMPARE(icon1.isNull(), false);

        auto icon2 = QIcon::fromTheme("system");
        QCOMPARE(icon2.isNull(), false);

        auto icon3 = QIcon::fromTheme("document-new");
        QCOMPARE(icon3.isNull(), false);
    }
};

#endif

// kate: indent-mode cstyle; replace-tabs on;
