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
#include "itemsmanager.h"

namespace kreen {
namespace core {

ItemsManager::ItemsManager()
{

}

ItemsManager::~ItemsManager()
{

}

void ItemsManager::addItem(ItemPtr item)
{
    _items.push_back(item);
}

void ItemsManager::addDemoItems()
{
    //TODO

    {
        auto item = Item::create("line");
        item->setLine(QLine(QPoint(10, 20), QPoint(30, 40)));
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(30, 30, 40, 30));
        item->lineColor()->color = Qt::darkGreen;
        item->lineStyle()->width = 3;
        item->lineStyle()->penStyle = Qt::DotLine;
        _items.push_back(item);
    }

    {
        auto item = Item::create("ellipse");
        item->setRect(QRect(10, 40, 40, 40));
        _items.push_back(item);
    }

    {
        auto item = Item::create("ellipse");
        item->setRect(QRect(10, 80, 70, 40));
        item->lineColor()->color = Qt::blue;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::DashLine;
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(0, 0, 10, 10));
        item->lineStyle()->width = 1;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = false;
        _items.push_back(item);
    }


    {
        auto item = Item::create("text");
        item->setRect(QRect(10, 120, 200, 40));
        item->text = TextProperty::create();
        item->text->text = "Hello From ItemsManager";
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(200, 200, 50, 50));
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(962-10, 556-10, 10, 10));
        _items.push_back(item);
    }
}

const std::vector<ItemPtr>& ItemsManager::items()
{
    return _items;
}

}
}
