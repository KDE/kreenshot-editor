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

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include <QString>
#include <QRect>
#include <QLine>
#include <QColor>

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(KreenItem)
KREEN_SHAREDPTR_FORWARD_DECL(ItemProperty)
KREEN_SHAREDPTR_FORWARD_DECL(LineColorProperty)
KREEN_SHAREDPTR_FORWARD_DECL(LineStyleProperty)
KREEN_SHAREDPTR_FORWARD_DECL(DropShadowProperty)

class RectStyleProperty;
class FillProperty;
class TextProperty;
class ObfuscateProperty;
class ArrowProperty;
class ImageProperty;

/**
 * One item with it's type, position, size etc.
 */
class KREEN_DECL_EXPORT KreenItem
{
public:
    /**
     * see ctor
     */
    static KreenItemPtr make_shared(QString typeId);

public:
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
    KreenItem(QString typeId);

    virtual ~KreenItem();

    /**
     * returns a deep copy and including the id
     */
    KreenItemPtr deepCopy();

    /**
     * unique id within a Document
     */
    int id();

    /**
     * To be used only internally by the Document. Returns the old id.
     */
    int setId(int id);

    /**
     * set geometry (position and size) of the item
     */
    void setRect(QRect rect);

    /**
     * only used for "line": set geometry (position and length) of the line
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

    /**
     * move the item by the given amount
     */
    void translate(int dx, int dy);

public:
    /**
     * Creates a new Item with default values for all properties available for this item.
     * See ctor for available typeIds.
     */
    static std::shared_ptr<KreenItem> create(QString typeId);

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

private:
    /**
     * see id()
     */
    int _id = -1;

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

#endif
