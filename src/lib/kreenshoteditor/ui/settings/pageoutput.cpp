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
#include "pageoutput.h"
#include "pageoutputimpl.h"
#include "ui_pageoutput.h"
#include <QFileDialog>
#include <QDebug>
#include <QMenu>
#include <QToolBar>
#include "../../core/impl/outputfilenamegenerator.h"
#include "../../ui/widgets/menubuttonutil.h"
#include <core/documentfile.h>

namespace kreen {
namespace ui {
namespace settings {

#define tr(arg) QObject::tr(arg)

class PageOutputImpl
{
public:
    Ui::pageOutput ui;
    PageOutput* owner;

public:
    PageOutputImpl(PageOutput* owner_)
    {
        owner = owner_;
    }

    void instrumentResetFilenamepatternButton()
    {
        MenuButtonUtil util(ui.toolButtonRestoreDefaultFilenamePattern);
        ui.toolButtonRestoreDefaultFilenamePattern->setIcon(QIcon::fromTheme("code-variable"));
        auto action = new QAction(QIcon::fromTheme("edit-undo"), "Reset filename pattern to default", owner);
        owner->connect(action, SIGNAL(triggered()), owner, SLOT(slotResetFilenamePattern()));
        ui.toolButtonRestoreDefaultFilenamePattern->addAction(action);
    }

private:
};

#undef tr

PageOutput::PageOutput(QWidget* parent)
    : QWidget(parent)
{
    KREEN_PIMPL_INIT_THIS(PageOutput);
    setupUi();
}

PageOutput::~PageOutput()
{
}

void PageOutput::setupUi()
{
    d->ui.setupUi(this);
    connect(d->ui.toolButtonChooseDirectory, SIGNAL(clicked()), this, SLOT(slotChooseDefaultOutputDirectory()));
    connect(d->ui.lineEditOutputDirectory, SIGNAL(textChanged(QString)), this, SLOT(slotUpdateFilenamePreview()));
    connect(d->ui.lineEditFilenamePattern, SIGNAL(textChanged(QString)), this, SLOT(slotUpdateFilenamePreview()));
    connect(d->ui.comboBoxImageFileExt, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateFilenamePreview()));

    d->ui.comboBoxImageFileExt->addItems(DocumentFile::supportedImageFormats());

    d->instrumentResetFilenamepatternButton();
}

void PageOutput::setValues(SettingsGroupOutput values)
{
    d->ui.lineEditOutputDirectory->setText(values.defaultOutputDirectory);

    QString filename, ext;
    PageOutputImplHeader::filenameToFilenamePlusExt(values.filenamePattern, &filename, &ext);
    d->ui.lineEditFilenamePattern->setText(filename);
    d->ui.comboBoxImageFileExt->setCurrentText(ext);

    d->ui.checkBoxAfterSaveOpenDefaultViewer->setChecked(values.afterSaveOpenDefaultViewer);
    d->ui.checkBoxAfterSaveShowFileBrowser->setChecked(values.afterSaveOpenFileBrowser);
    d->ui.radioButtonAfterSaveClipboardFilename->setChecked(values.afterSaveClipboardFilename);
    d->ui.radioButtonAfterSaveClipboardImage->setChecked(values.afterSaveClipboardImageData);
}

SettingsGroupOutput PageOutput::values()
{
    SettingsGroupOutput values;
    values.defaultOutputDirectory = d->ui.lineEditOutputDirectory->text();

    values.filenamePattern = PageOutputImplHeader::filenamePlusExtToFilename(
                                 d->ui.lineEditFilenamePattern->text(), d->ui.comboBoxImageFileExt->currentText());

    values.afterSaveOpenDefaultViewer = d->ui.checkBoxAfterSaveOpenDefaultViewer->isChecked();
    values.afterSaveOpenFileBrowser = d->ui.checkBoxAfterSaveShowFileBrowser->isChecked();
    values.afterSaveClipboardFilename = d->ui.radioButtonAfterSaveClipboardFilename->isChecked();
    values.afterSaveClipboardImageData = d->ui.radioButtonAfterSaveClipboardImage->isChecked();
    return values;
}

void PageOutput::slotChooseDefaultOutputDirectory()
{
    QDir currentDir(d->ui.lineEditOutputDirectory->text());
    if (!currentDir.exists())
    {
        currentDir = QDir::home();
    }
    qDebug() << currentDir.canonicalPath();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Default output directory"), currentDir.canonicalPath(), QFileDialog::ShowDirsOnly);
    qDebug() << dir;
    if (!dir.isEmpty())
    {
        d->ui.lineEditOutputDirectory->setText(dir);
    }
}

void PageOutput::slotUpdateFilenamePreview()
{
    OutputFilenameGenerator outputFilenameGenerator;
    outputFilenameGenerator.initCaptureTime();
    outputFilenameGenerator.setDescription("Window 1");
    outputFilenameGenerator.setFilenamePattern(
        QDir(d->ui.lineEditOutputDirectory->text()).filePath(
            PageOutputImplHeader::filenamePlusExtToFilename(d->ui.lineEditFilenamePattern->text(), d->ui.comboBoxImageFileExt->currentText()))
    );
    d->ui.labelPreview->setText(outputFilenameGenerator.resultingFilename());
}

void PageOutput::slotResetFilenamePattern()
{
    SettingsGroupOutput values;
    QString filename, ext;
    PageOutputImplHeader::filenameToFilenamePlusExt(values.filenamePattern_DefaultValue(), &filename, &ext);
    d->ui.lineEditFilenamePattern->setText(filename);
}

}
}
}
