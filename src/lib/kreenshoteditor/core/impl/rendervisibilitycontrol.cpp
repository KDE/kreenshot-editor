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
#include "rendervisibilitycontrol.h"
#include <QDebug>

namespace kreen {
namespace core {

RenderVisibilityControlPtr RenderVisibilityControl::make_shared()
{
    return std::make_shared<RenderVisibilityControl>();
}

RenderVisibilityControl::RenderVisibilityControl()
{

}

RenderVisibilityControl::~RenderVisibilityControl()
{

}

// static int count = 0;

void RenderVisibilityControl::initPainting()
{
    if (!_paintUpToItemStack.empty()) { // do not init if within painting
        return;
    }

    //_paintUpToItemReached = false;
//     count = 0;
}

void RenderVisibilityControl::pushPaintUpToItem(QGraphicsItem* grItem)
{
    _paintUpToItemStack.push(grItem);
    _paintUpToItemReachedStack.push(false);
}

void RenderVisibilityControl::popPaintUpToItem()
{
    _paintUpToItemStack.pop();
    _paintUpToItemReachedStack.pop();
}


bool RenderVisibilityControl::onPaintEnter(QGraphicsItem* grItem)
{
    if (_paintUpToItemStack.empty()) {
        return true;
    }

//     count++;
//     if (count > 6) {
//         paintUpToItemReached = true;
//     }

    if (_paintUpToItemReachedStack.top()) {
        return false;
    }

    if (grItem == _paintUpToItemStack.top()) {
        _paintUpToItemReachedStack.top() = true;
        return false;
    }

    return true;
}

}
}
