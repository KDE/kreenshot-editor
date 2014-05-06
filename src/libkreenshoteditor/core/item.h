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
#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QRect>
#include <memory>

class LineColorProperty;
class LineStyleProperty;
class DropShadowProperty;
class RectStyleProperty;
class FillProperty;
class TextProperty;
class ObfuscateProperty;
class ArrowProperty;
class ImageProperty;

/**
 * One item with it's type, position, size etc.
 */
class Item
{
public:
    Item();
    /**
     * line (= arrow)
     * rect
     * ellipse
     * text
     * highlight (should affect only background)
     * obfuscate
     * image
     */
    Item(QString typeId);
    virtual ~Item();

    void setRect(QRect rect);
    QRect rect();

public:
    QString typeId;

    // not everything applies for every itemType
    // what is not possible is null

    LineColorProperty* lineColor;
    LineStyleProperty* lineStyle; // dotted, dashed, solid, width
    DropShadowProperty* dropShadow; // color, blur, offset
    RectStyleProperty* rectStyle; // rounded or not (applies also to the text rect if not null)
    FillProperty* fillColor; // fill color, percent opacity
    std::shared_ptr<TextProperty> text; // text string, autowrap, font, size, color, (for border see lineColor)
    ObfuscateProperty* obfuscateStyle; // pixelize size, blur radius
    ArrowProperty* arrowFront; // size of arrow (or start just with mere existence, size controlled by lineStyle)
    ArrowProperty* arrowBack;
    ImageProperty* image; // image data (e. g. the cursor)

private:
    QRect _rect;

};

class TextProperty
{
public:
    static std::shared_ptr<TextProperty> create() {
        return std::shared_ptr<TextProperty>(new TextProperty);
    }

public:
    QString text;
};

#endif // ITEM_H
