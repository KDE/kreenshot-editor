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
#ifndef UI_SETTINGS_PAGEOUTPUT_H
#define UI_SETTINGS_PAGEOUTPUT_H

#include <QWidget>
#include <memory>
#include "../../core/settingsgroupoutput.h"
#include "../../core/outputfilenamemanager.h"

namespace kreen {
namespace ui {
namespace settings {

using namespace kreen::core;

class PageOutputImpl;
typedef std::shared_ptr<PageOutputImpl> PageOutputImplPtr;

class PageOutput : public QWidget
{
    Q_OBJECT

public:
    PageOutput(QWidget* parent, OutputFilenameManagerPtr outputFilenameManager);
    virtual ~PageOutput();

    void setValues(SettingsGroupOutput values);

    SettingsGroupOutput values();

public Q_SLOTS:

private Q_SLOTS:
    void chooseDefaultOutputDirectory();
    void updateFilenamePreview();

private:
    void setupUi();

private:
    PageOutputImplPtr d;

};

}
}
}

#endif // UI_SETTINGS_PAGEOUTPUT_H

// kate: indent-mode cstyle; replace-tabs on;
