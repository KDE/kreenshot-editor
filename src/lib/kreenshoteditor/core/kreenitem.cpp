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
#include "kreenitem.h"
#include <QDebug>
#include <algorithm>

namespace kreen {
namespace core {

class KreenItem::Impl
{
public:
    /**
     * see id()
     */
    int _id = -1;

    int zValue = -1;

    QRect _rect;
    QLine _line;

    std::vector<ItemPropertyPtr> _properties;
};

KreenItemPtr KreenItem::make_shared(QString typeId)
{
    return std::make_shared<KreenItem>(typeId);
}

KreenItem::KreenItem(QString typeId)
{
    KREEN_PIMPL_INIT(KreenItem)
    this->typeId = typeId;
}

KreenItem::KreenItem(const KreenItem& other) : KreenItem(other.typeId)
{
    d = std::make_shared<KreenItem::Impl>(*other.d); // make copy of d ptr
}

KreenItem::~KreenItem()
{

}

KreenItemPtr KreenItem::deepCopy()
{
    auto obj = *this; // deference and invoke copy ctor (TODO: we have to copy everything!)
    return std::make_shared<KreenItem>(obj);
}

bool KreenItem::deepEquals(kreen::core::KreenItemPtr other)
{
    // TODO: not complete yet
    
    return d->_id == other->d->_id
           && d->zValue == other->d->zValue
           && typeId == other->typeId
           && d->_rect == other->d->_rect
           && d->_line == other->d->_line
           && d->_properties == other->d->_properties
           ;
}

int KreenItem::id()
{
    return d->_id;
}

int KreenItem::setId(int id)
{
    int oldId = d->_id;
    d->_id = id;
    return oldId;
}

int KreenItem::zValue()
{
    return d->zValue;
}

void KreenItem::setZValue(int z)
{
    d->zValue = z;
}

bool KreenItem::isImageOperation()
{
    return typeId.startsWith("op-");
}

KreenItemPtr KreenItem::create(QString typeId)
{
    auto item = KreenItem::make_shared(typeId);

    if (typeId == "rect") {
        item->d->_properties.push_back(std::make_shared<LineColorProperty>());
        item->d->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->d->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::red;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "ellipse") {
        item->d->_properties.push_back(std::make_shared<LineColorProperty>());
        item->d->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->d->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::black;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "line") {
        item->d->_properties.push_back(std::make_shared<LineColorProperty>());
        item->d->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->d->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::green;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "text") {
        item->d->_properties.push_back(std::make_shared<LineColorProperty>());
        item->d->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->d->_properties.push_back(std::make_shared<DropShadowProperty>());
        item->d->_properties.push_back(std::make_shared<TextProperty>());
        // TODO: fillColor

        item->lineColor()->color = Qt::black; // TODO: or transparent for "no border rect"
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        // TODO: fillcolor solid
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "obfuscate") {
        item->d->_properties.push_back(std::make_shared<LineColorProperty>());
        item->d->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->d->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::black;
        item->lineStyle()->width = 1;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = false;
    }
    else if (typeId == "op-crop") {
        // no settings
    }
    else {
        qDebug() << "Item::create: unknown typeId" << typeId;
        Q_ASSERT_X(false, "Item::create", "Unknown typeId");
    }

    return item;
}

void KreenItem::setRect(QRect rect)
{
    d->_rect = rect;
}

QRect KreenItem::rect()
{
    if (d->_line.isNull())
        return d->_rect;
    else
        return QRect(d->_line.p1(), d->_line.p2());
}

void KreenItem::setLine(QLine line)
{
    qDebug() << "KreenItem::setLine: " << line;
    d->_line = line;
}

QLine KreenItem::line()
{
    return d->_line;
}

void KreenItem::translate(int dx, int dy)
{
    if (d->_line.isNull()) {
        d->_rect.translate(dx, dy);
    }
    else {
        d->_line.translate(dx, dy);
    }
}

ItemPropertyPtr propFromVectorOrNull(QString propName, std::vector<ItemPropertyPtr> _properties)
{
    auto result = std::find_if(_properties.begin(), _properties.end(),
    [propName](ItemPropertyPtr arg) {
        return arg->name == propName;
    });
    return result != _properties.end() ? *result : nullptr;
}

LineColorPropertyPtr KreenItem::lineColor()
{
    return std::static_pointer_cast<LineColorProperty>(propFromVectorOrNull("lineColor", d->_properties));
}

LineStylePropertyPtr KreenItem::lineStyle()
{
    return std::static_pointer_cast<LineStyleProperty>(propFromVectorOrNull("lineStyle", d->_properties));
}

DropShadowPropertyPtr KreenItem::dropShadow()
{
    return std::static_pointer_cast<DropShadowProperty>(propFromVectorOrNull("dropShadow", d->_properties));
}

TextPropertyPtr KreenItem::text()
{
    return std::static_pointer_cast<TextProperty>(propFromVectorOrNull("text", d->_properties));
}

}
}
