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
#include <QScrollArea>
#include <QMessageBox>
#include "ui/maineditorwidget.h"
#include "core/itemsmanager.h"

class KreenshotEditorImpl
{
public:
    KreenshotEditorImpl()
    {
        itemsManager = ItemsManagerPtr(new ItemsManager());
    }

public:
    QImage baseImage;
    ItemsManagerPtr itemsManager;
};

KreenshotEditor::KreenshotEditor()
{
    d = KreenshotEditorImplPtr(new KreenshotEditorImpl());
}

KreenshotEditor::~KreenshotEditor()
{

}

void KreenshotEditor::setBaseImage(const QImage& image)
{
    d->baseImage = image;
}

const QImage& KreenshotEditor::getBaseImage()
{
    return d->baseImage;
}

QWidget* KreenshotEditor::createMainEditorWidget()
{
    auto mainEditorWidget = new MainEditorWidget(this);

    bool oldQScrollAreaLogic = false;
    if (oldQScrollAreaLogic) {
        auto scrollArea = new QScrollArea();
        scrollArea->setBackgroundRole(QPalette::Background);
        //scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(mainEditorWidget);
        return scrollArea;
    }
    else {
        return mainEditorWidget;
    }
}

ItemsManagerPtr KreenshotEditor::itemsManager()
{
    return d->itemsManager;
}

void KreenshotEditor::chooseTool()
{
    QString senderName = QObject::sender()->objectName();

    if (senderName == "actionToolSelect") {
        QMessageBox::information(nullptr, "Action", "Select");
    }
    else if (senderName == "actionToolRect") {
        QMessageBox::information(nullptr, "Action", "Rect");
    }
    else if (senderName == "actionToolEllipse") {
        QMessageBox::information(nullptr, "Action", "Ellipse");
    }
    else {
        QMessageBox::information(nullptr, "Not impl", "Not implemented yet");
    }
}
