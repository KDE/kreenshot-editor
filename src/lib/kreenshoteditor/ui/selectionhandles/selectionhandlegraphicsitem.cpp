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
#include "../impl/kreengraphicsitembase.h" // TODO: remove this dependency
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

namespace kreen {
namespace ui {

class SelectionHandleGraphicsItem::Impl
{
public:
    SelectionHandles* manager = nullptr;
    SelectionHandles::PositionEnum posEnum;
    KreenGraphicsItemBase* instrumentedItem = nullptr;
    QPointF startPos;
    bool renderVisible = true;

public:
    /**
    * 1   7   2
    * 5   0   6
    * 3   8   4
    */
    QCursor cursorFromPositionEnum(SelectionHandles::PositionEnum posEnum)
    {
        switch (posEnum)
        {
            case SelectionHandles::Position0_Center: return Qt::ArrowCursor; // Qt::OpenHandCursor
            case SelectionHandles::Position1_TopLeft: return Qt::SizeFDiagCursor;
            case SelectionHandles::Position2_TopRight: return Qt::SizeBDiagCursor;
            case SelectionHandles::Position3_BottomLeft: return Qt::SizeBDiagCursor;
            case SelectionHandles::Position4_BottomRight: return Qt::SizeFDiagCursor;
            case SelectionHandles::Position5_Left: return Qt::SizeHorCursor;
            case SelectionHandles::Position6_Right: return Qt::SizeHorCursor;
            case SelectionHandles::Position7_Top: return Qt::SizeVerCursor;
            case SelectionHandles::Position8_Bottom: return Qt::SizeVerCursor;
        }

        qDebug() << "[ERROR] cursorFromPositionEnum. All cases must be handled.";
        Q_ASSERT(false);
        return Qt::ArrowCursor;
    }
};

SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(kreen::ui::SelectionHandles* manager, kreen::ui::SelectionHandles::PositionEnum posEnum, kreen::ui::KreenGraphicsItemBase* instrumentedItem, QRectF rect) : QGraphicsRectItem(rect)
{
    KREEN_PIMPL_INIT(SelectionHandleGraphicsItem);

    d->manager = manager;
    d->posEnum = posEnum;
    d->instrumentedItem = instrumentedItem;
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setBrush(QBrush(Qt::black));
    setPen(Qt::NoPen);

    setCursor(d->cursorFromPositionEnum(posEnum));
}

SelectionHandleGraphicsItem::~SelectionHandleGraphicsItem()
{

}

SelectionHandles::PositionEnum SelectionHandleGraphicsItem::posEnum()
{
    return d->posEnum;
}

void SelectionHandleGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    //qDebug() << "SelectionHandleGraphicsItem::hoverEnterEvent";

    // NOTE, that this event is not triggered right away after creating a new item and the mouse cursor already stays
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
    d->manager->setAllSelectedItemsMovable(false);
    d->startPos = pos();
    QGraphicsItem::mousePressEvent(event);
    d->instrumentedItem->_activeHandle = this;
    d->instrumentedItem->slotHandleStartDrag();
}

QVariant SelectionHandleGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
//     if (change == QGraphicsItem::ItemPositionChange) {
//         //
//         // handle movement restrictions; delegate to instrumented Item?
//         // --> TODO change: draw selection handles from within item!!!
//         //
//         QPointF oldPos = pos();
//         QPointF curPos = value.toPointF();
//
//         QPointF newPos(oldPos.x(), curPos.y()); // todo
//
//         return newPos;
//     }
//     else
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        if (d->instrumentedItem->_activeHandle && d->renderVisible) {
            d->manager->setAllHandlesRenderVisible(false); // move a handle => hide handles
        }
        QPointF curPos = pos();
        // the client can get more information using _activeHandle? (TODO for posEnum)
        d->instrumentedItem->slotHandlePositionHasChanged(curPos - d->startPos);
    }

    return QGraphicsItem::itemChange(change, value);
}

void SelectionHandleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // see mousePressEvent
    // we do NOT call this here because it would make items unmovable when using handles in Select mode:
    //   d->manager->setAllItemsWithHandlesMovable(true);
    // but see MainEditorWidget::slotFixSelectableAndMovable()
    QGraphicsItem::mouseReleaseEvent(event);
    d->instrumentedItem->_activeHandle = nullptr;
    d->manager->setAllHandlesRenderVisible(true);
}

void SelectionHandleGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (d->renderVisible) {
        QGraphicsRectItem::paint(painter, option, widget);
    }
}

void SelectionHandleGraphicsItem::setRenderVisible(bool visible)
{
    d->renderVisible = visible;
}

}
}
