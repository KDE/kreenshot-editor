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
#ifndef KREENSHOTEDITOR_H
#define KREENSHOTEDITOR_H

class MainEditorWidget;
class QImage;
class QWidget;
class KreenshotEditorImpl;

class KreenshotEditor
{
public:
    KreenshotEditor();
    virtual ~KreenshotEditor();

    // TODO: is QImage reference counted?
    void setBaseImage(const QImage& image);
    const QImage& getBaseImage();

    QWidget* createMainEditorWidget();

private:
    KreenshotEditorImpl* d;
};

#endif // KREENSHOTEDITOR_H
