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
#include "itemsmanager.h"

ItemsManager::ItemsManager()
{

}

ItemsManager::~ItemsManager()
{

}

void ItemsManager::addDemoItems()
{
    //TODO
    
    {
        Item item("line");
        item.setRect(QRect(10, 20, 30, 40));
        _items.push_back(item);
    }

    {
        Item item("rect");
        item.setRect(QRect(30, 30, 40, 30));
        _items.push_back(item);
    }

    {
        Item item("ellipse");
        item.setRect(QRect(10, 40, 40, 40));
        _items.push_back(item);
    }

    {
        Item item("ellipse");
        item.setRect(QRect(10, 80, 70, 40));
        _items.push_back(item);
    }

    {
        Item item("text");
        item.setRect(QRect(10, 90, 100, 20));
        item.text = TextProperty::create();
        item.text->text = "Hello From ItemsManager";
        _items.push_back(item);
    }
}

const std::vector< Item >& ItemsManager::items()
{
    return _items;
}
