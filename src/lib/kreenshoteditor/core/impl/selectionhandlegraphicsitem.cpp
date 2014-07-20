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
#include "selectionhandlegraphicsitem.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>

namespace kreen {
namespace core {

SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(QGraphicsItem* instrumentedItem)
 : SelectionHandleGraphicsItem(instrumentedItem, QRectF())
{
}

SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(QGraphicsItem* instrumentedItem, QRectF rect) : QGraphicsRectItem(rect)
{
    _instrumentedItem = instrumentedItem;
    setAcceptHoverEvents(true);
}

SelectionHandleGraphicsItem::~SelectionHandleGraphicsItem()
{

}

void SelectionHandleGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

void SelectionHandleGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

void SelectionHandleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // The mouse is over a SelectionHandle and the user
    // clicks to move the Handle. If we would not disable
    // Movement of the instrumentedItem then the item would move because it is selected
    // and all selected items are moved automatically.
    _instrumentedItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    QGraphicsItem::mousePressEvent(event);
}

void SelectionHandleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // see mousePressEvent
    _instrumentedItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant SelectionHandleGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        emit handleItemPositionHasChangedSignal();
    }
    else if (change == QGraphicsItem::ItemPositionChange) {
        _instrumentedItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    return QGraphicsItem::itemChange(change, value);
}

}
}
