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
#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include <vector>
#include <memory>
#include <QString>

namespace kreen {
namespace core {

/**
 */
class SettingsDto
{
public:
    SettingsDto(bool isTest);
    virtual ~SettingsDto();

    /**
     * load settings from disk
     */
    void load();

    /**
     * store settings to disk (~/.config/kreenshoteditor.org/)
     */
    void save();

    /**
     * resets all settings to default, use with care
     */
    void reset();

public:
    QString output_DefaultOutputDirectory;
    QString output_FilenamePattern;

private:

};

}
}

#endif // CORE_SETTINGS_H
