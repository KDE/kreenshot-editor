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
#ifndef TEST_SETTINGS_H
#define TEST_SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTest>
#include "lib/kreenshoteditor/core/settingsmanager.h"

using namespace kreen::core;

class TestSettings : public QObject
{
    Q_OBJECT

public slots:
//     void initTestCase();
//     void cleanupTestCase();

private slots:
    void Reset()
    {
        auto settings = SettingsManager::instance("unittest");
        settings->reset();
        settings->load();
        QCOMPARE(settings->output.defaultOutputDirectory, QString("~/Pictures/screenshots"));
        QCOMPARE(settings->output.filenamePattern, QString("${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png"));

        settings->output.defaultOutputDirectory = "aaa";
        settings->output.filenamePattern = "bbb";
        settings->save();
    }

    void SaveLoad()
    {
        {
            auto settings = SettingsManager::instance("unittest");
            settings->output.defaultOutputDirectory = "aaa";
            settings->output.filenamePattern = "bbb";
            settings->save();
        }

        {
            auto settings = SettingsManager::instance("unittest");
            settings->load();
            QCOMPARE(settings->output.defaultOutputDirectory, QString("aaa"));
            QCOMPARE(settings->output.filenamePattern, QString("bbb"));
        }
    }
};

#endif // TEST_SETTINGS_H

// kate: indent-mode cstyle; replace-tabs on;
