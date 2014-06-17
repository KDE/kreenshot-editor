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
#ifndef CORE_SETTINGSMANAGER_H
#define CORE_SETTINGSMANAGER_H

#include <kreen/util/pimplutil.h>
#include <vector>
#include <memory>
#include <QString>
#include "settingsgroupoutput.h"

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(SettingsManager)

/**
 */
class SettingsManager
{
public:
    /**
     * singleton
     */
    static SettingsManagerPtr instance(QString configId = "global");

public:
    virtual ~SettingsManager();

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
    /**
     * read it, write it or replace it. Only make sure that it only contains values (and no references)
     */
    SettingsGroupOutput output;

private:
    /**
     * TODO: later when kreenshoteditor is used as shared library one might have
     * separate settings per application
     */
    SettingsManager(QString configId);

private:
    static SettingsManagerPtr _instance;

};

}
}

#endif // CORE_SETTINGSMANAGER_H
