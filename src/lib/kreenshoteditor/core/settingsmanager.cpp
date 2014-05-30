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
#include "settingsmanager.h"
#include <QCoreApplication>
#include <QSettings>

namespace kreen {
namespace core {

SettingsManagerPtr SettingsManager::instance(QString configId)
{
    if (!_instance)
    {
        _instance = SettingsManagerPtr(new SettingsManager(configId));
    }

    return _instance;
}

SettingsManagerPtr SettingsManager::_instance;

SettingsManager::SettingsManager(QString configId)
{
    //QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("kreenshoteditor.org");
    QCoreApplication::setApplicationName("kreenshot-editor-" + configId);
}

SettingsManager::~SettingsManager()
{

}

void SettingsManager::load()
{
    QSettings settings;
    output.defaultOutputDirectory = settings.value("output/default-output-directory", "~/Pictures/screenshots").toString();
    output.filenamePattern = settings.value("output/filename-pattern", "${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png").toString();
}

void SettingsManager::save()
{
    QSettings settings;
    settings.setValue("output/default-output-directory", output.defaultOutputDirectory);
    settings.setValue("output/filename-pattern", output.filenamePattern);
}

void SettingsManager::reset()
{
    QSettings settings;
    settings.clear();
}


}
}