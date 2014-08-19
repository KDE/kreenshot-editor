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

KreenItemPtr KreenItem::make_shared(QString typeId)
{
    return std::make_shared<KreenItem>(typeId);
}

KreenItem::KreenItem(QString typeId)
{
    this->typeId = typeId;
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
    
    return _id == other->_id
           && typeId == other->typeId
           && _rect == other->_rect
           && _line == other->_line
           && _properties == other->_properties
           ;
}

int KreenItem::id()
{
    return _id;
}

int KreenItem::setId(int id)
{
    int oldId = _id;
    _id = id;
    return oldId;
}

bool KreenItem::isImageOperation()
{
    return typeId.startsWith("op-");
}

KreenItemPtr KreenItem::create(QString typeId)
{
    auto item = KreenItem::make_shared(typeId);

    if (typeId == "rect") {
        item->_properties.push_back(std::make_shared<LineColorProperty>());
        item->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::red;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "ellipse") {
        item->_properties.push_back(std::make_shared<LineColorProperty>());
        item->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::black;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "line") {
        item->_properties.push_back(std::make_shared<LineColorProperty>());
        item->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->_properties.push_back(std::make_shared<DropShadowProperty>());

        item->lineColor()->color = Qt::green;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "text") {
        item->_properties.push_back(std::make_shared<LineColorProperty>());
        item->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->_properties.push_back(std::make_shared<DropShadowProperty>());
        // TODO: fillColor, text

        item->lineColor()->color = Qt::black; // TODO: or transparent for "no border rect"
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        // TODO: fillcolor solid
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "obfuscate") {
        item->_properties.push_back(std::make_shared<LineColorProperty>());
        item->_properties.push_back(std::make_shared<LineStyleProperty>());
        item->_properties.push_back(std::make_shared<DropShadowProperty>());

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
    _rect = rect;
}

QRect KreenItem::rect()
{
    if (_line.isNull())
        return _rect;
    else
        return QRect(_line.p1(), _line.p2());
}

void KreenItem::setLine(QLine line)
{
    qDebug() << "KreenItem::setLine: " << line;
    _line = line;
}

QLine KreenItem::line()
{
    return _line;
}

void KreenItem::translate(int dx, int dy)
{
    if (_line.isNull()) {
        _rect.translate(dx, dy);
    }
    else {
        _line.translate(dx, dy);
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
    return std::static_pointer_cast<LineColorProperty>(propFromVectorOrNull("lineColor", _properties));
}

LineStylePropertyPtr KreenItem::lineStyle()
{
    return std::static_pointer_cast<LineStyleProperty>(propFromVectorOrNull("lineStyle", _properties));
}

DropShadowPropertyPtr KreenItem::dropShadow()
{
    return std::static_pointer_cast<DropShadowProperty>(propFromVectorOrNull("dropShadow", _properties));
}

}
}
