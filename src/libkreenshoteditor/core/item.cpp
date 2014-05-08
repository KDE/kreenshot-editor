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
#include "item.h"

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
    return ItemPtr(new Item(typeId));
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
