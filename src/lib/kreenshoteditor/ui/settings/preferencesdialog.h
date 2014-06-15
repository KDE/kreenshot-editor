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
#ifndef UI_SETTINGS_PREFERENCES_H
#define UI_SETTINGS_PREFERENCES_H

#include <QDialog>
#include <memory>
#include "kreen/core/settingsmanager.h"
#include "kreen/core/outputfilenamemanager.h"

namespace kreen {
namespace ui {
namespace settings {

using namespace kreen::core;

class PreferencesDialogImpl;
typedef std::shared_ptr<PreferencesDialogImpl> PreferencesDialogImplPtr;

/**
 * The accepted signal is handled internally and then the settings are stored
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * settingsManager: read and store the settings
     * outputFilenameManager: for preview
     */
    PreferencesDialog(SettingsManagerPtr settingsManager, OutputFilenameManagerPtr outputFilenameManager);
    virtual ~PreferencesDialog();

private Q_SLOTS:
    void pullSettingsFromUiAndSave();

private:
    void setupUi();

    void pushSettingsToUi();

private:
    PreferencesDialogImplPtr d;
};

}
}
}

#endif // UI_SETTINGS_PREFERENCES_H

// kate: indent-mode cstyle; replace-tabs on;
