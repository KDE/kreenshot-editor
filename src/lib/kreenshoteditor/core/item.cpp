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
#include "item.h"
#include <QDebug>
#include <algorithm>

Item::Item()
{

}

Item::Item(QString typeId)
{
    this->typeId = typeId;
}

Item::~Item()
{

}

ItemPtr Item::create(QString typeId)
{
    auto item = ItemPtr(new Item(typeId));
    if (typeId == "rect") {
        item->_properties.push_back(LineColorPropertyPtr(new LineColorProperty()));
        item->_properties.push_back(LineStylePropertyPtr(new LineStyleProperty()));
        item->_properties.push_back(DropShadowPropertyPtr(new DropShadowProperty()));

        item->lineColor()->color = Qt::red;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "ellipse") {
        item->_properties.push_back(LineColorPropertyPtr(new LineColorProperty()));
        item->_properties.push_back(LineStylePropertyPtr(new LineStyleProperty()));
        item->_properties.push_back(DropShadowPropertyPtr(new DropShadowProperty()));

        item->lineColor()->color = Qt::black;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else if (typeId == "line") {
        item->_properties.push_back(LineColorPropertyPtr(new LineColorProperty()));
        item->_properties.push_back(LineStylePropertyPtr(new LineStyleProperty()));
        item->_properties.push_back(DropShadowPropertyPtr(new DropShadowProperty()));

        item->lineColor()->color = Qt::green;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = true;
    }
    else {
        qDebug() << "Item::create: !!!!!!!!! TODO for this item";
    }

    return item;
}

void Item::setRect(QRect rect)
{
    _rect = rect;
}

QRect Item::rect()
{
    if (_line.isNull())
        return _rect;
    else
        return QRect(_line.p1(), _line.p2());
}

void Item::setLine(QLine line)
{
    _line = line;
}

QLine Item::line()
{
    return _line;
}

ItemPropertyPtr propFromVectorOrNull(QString propName, std::vector<ItemPropertyPtr> _properties)
{
    auto result = std::find_if(_properties.begin(), _properties.end(),
    [propName](ItemPropertyPtr arg) {
        return arg->name == propName;
    });
    return result != _properties.end() ? *result : nullptr;
}

LineColorPropertyPtr Item::lineColor()
{
    return std::static_pointer_cast<LineColorProperty>(propFromVectorOrNull("lineColor", _properties));
}

LineStylePropertyPtr Item::lineStyle()
{
    return std::static_pointer_cast<LineStyleProperty>(propFromVectorOrNull("lineStyle", _properties));
}

DropShadowPropertyPtr Item::dropShadow()
{
    return std::static_pointer_cast<DropShadowProperty>(propFromVectorOrNull("dropShadow", _properties));
}
