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
#ifndef CORE_ITEMSMANAGER_H
#define CORE_ITEMSMANAGER_H

#include "item.h"

#include <vector>
#include <memory>

/**
 * The "document"
 * Contains all items like Rects, Ellipses, Text, Blurring etc.
 * Manages Z-Order.
 */
class ItemsManager
{
public:
    ItemsManager();
    virtual ~ItemsManager();

    void addItem(ItemPtr item);

    void addDemoItems();

    const std::vector<ItemPtr>& items();

private:
    std::vector<ItemPtr> _items;

};

typedef std::shared_ptr<ItemsManager> ItemsManagerPtr;

#endif // CORE_ITEMSMANAGER_H
