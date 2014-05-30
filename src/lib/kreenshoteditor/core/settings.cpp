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
#include "settings.h"
#include <QCoreApplication>
#include <QSettings>

namespace kreen {
namespace core {

SettingsDto::SettingsDto(bool isTest)
{
    //QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("kreenshoteditor.org");
    if (!isTest)
        QCoreApplication::setApplicationName("kreenshot-editor");
    else
        QCoreApplication::setApplicationName("kreenshot-editor-test");
}

SettingsDto::~SettingsDto()
{

}

void SettingsDto::load()
{
    QSettings settings;
    output_DefaultOutputDirectory = settings.value("output/default-output-directory", "~/Pictures/screenshots").toString();
    output_FilenamePattern = settings.value("output/filename-pattern", "${YYYY}-${MM}-${DD}_${hh}-${mm}-${ss}_${description_}.png").toString();
}

void SettingsDto::save()
{
    QSettings settings;
    settings.setValue("output/default-output-directory", output_DefaultOutputDirectory);
    settings.setValue("output/filename-pattern", output_FilenamePattern);
}

void SettingsDto::reset()
{
    QSettings settings;
    settings.clear();
}


}
}