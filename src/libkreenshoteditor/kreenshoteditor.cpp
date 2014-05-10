/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mitsch <codeminister@publicstatic.de>
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
#include "kreenshoteditor.h"
#include <QImage>
#include "ui/maineditorwidget.h"

class KreenshotEditorImpl
{
public:
    QImage baseImage;
};

KreenshotEditor::KreenshotEditor()
{
    d = new KreenshotEditorImpl();
}

KreenshotEditor::~KreenshotEditor()
{
    delete d;
}

void KreenshotEditor::setBaseImage(const QImage& image)
{
    d->baseImage = image;
}

const QImage& KreenshotEditor::getBaseImage()
{
    return d->baseImage;
}

MainEditorWidget* KreenshotEditor::createMainEditorWidget()
{
    auto mainEditorWidget = new MainEditorWidget(this);
    return mainEditorWidget;
}

