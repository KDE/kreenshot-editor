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
#ifndef CORE_SETTINGSGROUPOUTPUT_H
#define CORE_SETTINGSGROUPOUTPUT_H

#include <kreen/util/exportutil.h>
#include <vector>
#include <memory>
#include <QString>

namespace kreen {
namespace core {

/**
 */
class KREEN_DECL_EXPORT SettingsGroupOutput
{
public:
    SettingsGroupOutput() { }
    virtual ~SettingsGroupOutput() { }

public:
    QString defaultOutputDirectory;
    QString filenamePattern;

    bool afterSaveOpenDefaultViewer;
    bool afterSaveOpenFileBrowser;

    // either or
    bool afterSaveClipboardFilename;
    bool afterSaveClipboardImageData;

private:

};

}
}

#endif // CORE_SETTINGSGROUPOUTPUT_H
