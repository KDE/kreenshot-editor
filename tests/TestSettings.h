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
#ifndef TEST_SETTINGS_H
#define TEST_SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTest>
#include "../src/lib/kreenshoteditor/core/settings.h"

using namespace kreen::core;

class TestSettings : public QObject
{
    Q_OBJECT

public slots:
//     void initTestCase();
//     void cleanupTestCase();

private slots:
    void testReset()
    {
        SettingsDto dto(true);
        dto.reset();
        dto.load();
        QCOMPARE(dto.output_DefaultOutputDirectory, QString("~/Pictures/screenshots"));
        QCOMPARE(dto.output_FilenamePattern, QString("${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png"));

        dto.output_DefaultOutputDirectory = "aaa";
        dto.output_FilenamePattern = "bbb";
        dto.save();
    }

    void testSaveLoad()
    {
        {
            SettingsDto dto(true);
            dto.output_DefaultOutputDirectory = "aaa";
            dto.output_FilenamePattern = "bbb";
            dto.save();
        }

        {
            SettingsDto dto(true);
            dto.load();
            QCOMPARE(dto.output_DefaultOutputDirectory, QString("aaa"));
            QCOMPARE(dto.output_FilenamePattern, QString("bbb"));
        }
    }
};

#endif // TEST_SETTINGS_H

// kate: indent-mode cstyle; replace-tabs on;
