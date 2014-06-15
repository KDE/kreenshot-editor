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
#ifndef CORE_ITEM_H
#define CORE_ITEM_H

#include <QString>
#include <QRect>
#include <QLine>
#include <QColor>
#include <memory>

namespace kreen {
namespace core {

class ItemProperty;
typedef std::shared_ptr<ItemProperty> ItemPropertyPtr;

class LineColorProperty;
typedef std::shared_ptr<LineColorProperty> LineColorPropertyPtr;

class LineStyleProperty;
typedef std::shared_ptr<LineStyleProperty> LineStylePropertyPtr;

class DropShadowProperty;
typedef std::shared_ptr<DropShadowProperty> DropShadowPropertyPtr;

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
     * op-crop
     * op-ripout
     */
    Item(QString typeId);
    virtual ~Item();

    void setRect(QRect rect);

    /**
     * only used for "line"
     */
    void setLine(QLine line);

    /**
     * the bounding rect (also for "line")
     */
    QRect rect();

    /**
     * only used for "line"
     */
    QLine line();


public:
    /**
     * Creates a new Item with default values for all properties available for this item.
     * See ctor for available typeIds.
     */
    static std::shared_ptr<Item> create(QString typeId);

public:
    QString typeId;

    // not everything applies for every itemType
    // what is not possible is null

    LineColorPropertyPtr lineColor();
    LineStylePropertyPtr lineStyle(); // dotted, dashed, solid, width
    DropShadowPropertyPtr dropShadow(); // color, blur, offset
    RectStyleProperty* rectStyle; // rounded or not (applies also to the text rect if not null)
    FillProperty* fillColor; // fill color, percent opacity
    std::shared_ptr<TextProperty> text; // text string, autowrap, font, size, color, (for border see lineColor), valign, halign
    ObfuscateProperty* obfuscateStyle; // pixelize size, blur radius
    ArrowProperty* arrowFront; // size of arrow (or start just with mere existence, size controlled by lineStyle)
    ArrowProperty* arrowBack;
    ImageProperty* image; // image data (e. g. the cursor)

private:
    QRect _rect;
    QLine _line;

    std::vector<ItemPropertyPtr> _properties;
};

class TextProperty
{
public:
    static std::shared_ptr<TextProperty> create() {
        return std::make_shared<TextProperty>();
    }

public:
    QString text;
};

typedef std::shared_ptr<Item> ItemPtr;

class ItemProperty
{
public:
    ItemProperty(QString name) {
        this->name = name;
    }
public:
    QString name;
};

class LineColorProperty : public ItemProperty
{
public:
    LineColorProperty() : ItemProperty("lineColor") { }
public:
    QColor color;
};

class LineStyleProperty : public ItemProperty
{
public:
    LineStyleProperty() : ItemProperty("lineStyle") { }
public:
    int width;
    Qt::PenStyle penStyle;
};

class DropShadowProperty : public ItemProperty
{
public:
    DropShadowProperty() : ItemProperty("dropShadow") { }
public:
    bool enabled;
};

}
}

#endif // CORE_ITEM_H
