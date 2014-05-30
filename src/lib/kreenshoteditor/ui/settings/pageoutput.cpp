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
#include "pageoutput.h"
#include "ui_pageoutput.h"
#include <QFileDialog>
#include <QDebug>

namespace kreen {
namespace ui {
namespace settings {

class PageOutputImpl
{
public:
    Ui::pageOutput ui;

private:
    void exp()
    {
        //ui.toolButtonChooseDirectory
    }
};

PageOutput::PageOutput(QWidget* parent)
 : QWidget(parent)
{
    d = std::make_shared<PageOutputImpl>();
    setupUi();
}

PageOutput::~PageOutput()
{
}

void PageOutput::setupUi()
{
    d->ui.setupUi(this);
    connect(d->ui.toolButtonChooseDirectory, SIGNAL(clicked()), this, SLOT(chooseDefaultOutputDirectory()));
    connect(d->ui.lineEditOutputDirectory, SIGNAL(textChanged(QString)), this, SLOT(updateFilenamePreview()));
    connect(d->ui.lineEditFilenamePattern, SIGNAL(textChanged(QString)), this, SLOT(updateFilenamePreview()));
}

void PageOutput::setValues(SettingsGroupOutput values)
{
    d->ui.lineEditOutputDirectory->setText(values.defaultOutputDirectory);
    d->ui.lineEditFilenamePattern->setText(values.filenamePattern);
}

SettingsGroupOutput PageOutput::values()
{
    SettingsGroupOutput values;
    values.defaultOutputDirectory = d->ui.lineEditOutputDirectory->text();
    values.filenamePattern = d->ui.lineEditFilenamePattern->text();
    return values;
}

void PageOutput::chooseDefaultOutputDirectory()
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

void PageOutput::updateFilenamePreview()
{
    d->ui.labelPreview->setText("aaaaa");
}

}
}
}