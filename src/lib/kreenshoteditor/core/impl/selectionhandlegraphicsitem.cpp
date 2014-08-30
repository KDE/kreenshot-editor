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
#include "selectionhandles.h"
#include "kreengraphicsitembase.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

namespace kreen {
namespace core {

// SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(QGraphicsItem* instrumentedItem)
//  : SelectionHandleGraphicsItem(instrumentedItem, QRectF())
// {
//
// }

SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(SelectionHandles* manager, KreenGraphicsItemBase* instrumentedItem, QRectF rect) : QGraphicsRectItem(rect)
{
    _manager = manager;
    _instrumentedItem = instrumentedItem;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setBrush(QBrush(Qt::black));
    setPen(Qt::NoPen);
}

SelectionHandleGraphicsItem::~SelectionHandleGraphicsItem()
{

}

void SelectionHandleGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    qDebug() << "SelectionHandleGraphicsItem::hoverEnterEvent";

    // NOTE, that this event is not called right away after creating a new item and the mouse cursor already stays
    // over the handle; we will use SelectionHandles::isMouseHoveringOnAnyHandle() to compensate for that

    QGraphicsItem::hoverEnterEvent(event);
}

void SelectionHandleGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

void SelectionHandleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "SelectionHandleGraphicsItem::mousePressEvent";

    // The mouse is over a SelectionHandle and the user clicks to move the Handle.
    // during item creating mode (not Select mode; for this see MainEditorWidget::slotFixSelectableAndMovable()).
    // If we would not disable Movement of the instrumentedItem,
    // then the item would move because it is selected
    // and all selected items are moved automatically.
    //
    _manager->setAllSelectedItemsMovable(false);
    _startPos = pos();
    QGraphicsItem::mousePressEvent(event);
    _instrumentedItem->_activeHandle = this;
    _instrumentedItem->slotHandleStartDrag();
}

void SelectionHandleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // see mousePressEvent
    // we do NOT call this here because it would make items unmovable when using handles in Select mode:
    //   _manager->setAllItemsWithHandlesMovable(true);
    // but see MainEditorWidget::slotFixSelectableAndMovable()
    QGraphicsItem::mouseReleaseEvent(event);
    _instrumentedItem->_activeHandle = nullptr;
}

QVariant SelectionHandleGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        //
        // handle movement restrictions; delegate to instrumented Item?
        // --> TODO change: draw selection handles from within item!!!
        //
        QPointF oldPos = pos();
        QPointF curPos = value.toPointF();

        QPointF newPos(oldPos.x(), curPos.y()); // todo

        return newPos;
    }
    else if (change == QGraphicsItem::ItemPositionHasChanged) {
        QPointF curPos = pos();
        _instrumentedItem->slotHandlePositionHasChanged(curPos - _startPos);
    }

    return QGraphicsItem::itemChange(change, value);
}

}
}
