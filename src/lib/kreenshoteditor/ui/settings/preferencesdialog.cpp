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
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "pageoutput.h"
#include <kreen/core/settingsmanager.h>

namespace kreen {
namespace ui {
namespace settings {

class PreferencesDialogImpl
{
public:
    SettingsManagerPtr settingsManager;
    OutputFilenameGeneratorPtr outputFilenameGenerator;
    Ui::dialogSettings ui;
    PageOutput* pageOutput;

private:
};

PreferencesDialog::PreferencesDialog(SettingsManagerPtr settingsManager, OutputFilenameGeneratorPtr outputFilenameGenerator)
{
    KREEN_PIMPL_INIT(PreferencesDialog);
    d->settingsManager = settingsManager;
    d->outputFilenameGenerator = outputFilenameGenerator;

    setupUi();
    pushSettingsToUi();
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::setupUi()
{
    d->ui.setupUi(this);

    d->ui.tabWidgetMain->clear();
    d->pageOutput = new PageOutput(this);
    d->ui.tabWidgetMain->addTab(d->pageOutput, d->pageOutput->windowTitle());

    connect(this, SIGNAL(accepted()), this, SLOT(pullSettingsFromUiAndSave()));
}

void PreferencesDialog::pushSettingsToUi()
{
    d->pageOutput->setValues(d->settingsManager->output);
}

void PreferencesDialog::pullSettingsFromUiAndSave()
{
    d->settingsManager->output = d->pageOutput->values();
    d->settingsManager->save();
}

}
}
}