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
#include "selectionhandlesmgr.h"
#include "selectionhandlebase.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QPainter>

namespace kreen {
namespace ui {

class SelectionHandleGraphicsItem::Impl
{
public:
    SelectionHandlesMgr* manager = nullptr;
    selhandles::PositionEnum posEnum;
    SelectionHandleBase* selHandleBase = nullptr;
    QPointF startPos;
    bool renderVisible = true;

public:
    /**
    * 1   7   2
    * 5   0   6
    * 3   8   4
    */
    QCursor cursorFromPositionEnum(selhandles::PositionEnum posEnum)
    {
        switch (posEnum)
        {
            case selhandles::Position0_Center: return Qt::ArrowCursor; // Qt::OpenHandCursor
            case selhandles::Position1_TopLeft: return Qt::SizeFDiagCursor;
            case selhandles::Position2_TopRight: return Qt::SizeBDiagCursor;
            case selhandles::Position3_BottomLeft: return Qt::SizeBDiagCursor;
            case selhandles::Position4_BottomRight: return Qt::SizeFDiagCursor;
            case selhandles::Position5_Left: return Qt::SizeHorCursor;
            case selhandles::Position6_Right: return Qt::SizeHorCursor;
            case selhandles::Position7_Top: return Qt::SizeVerCursor;
            case selhandles::Position8_Bottom: return Qt::SizeVerCursor;
            case selhandles::Position_LineEnd: return Qt::OpenHandCursor; // Qt::SizeAllCursor
            case selhandles::Position_LineStart: return Qt::OpenHandCursor; // Qt::SizeAllCursor
        }

        qDebug() << "[ERROR] cursorFromPositionEnum. All cases must be handled.";
        Q_ASSERT(false);
        return Qt::ArrowCursor;
    }
};

SelectionHandleGraphicsItem::SelectionHandleGraphicsItem(SelectionHandlesMgr* manager, selhandles::PositionEnum posEnum, SelectionHandleBase* selHandleBase, QRectF rect) : QGraphicsRectItem(rect)
{
    KREEN_PIMPL_INIT(SelectionHandleGraphicsItem);

    d->manager = manager;
    d->posEnum = posEnum;
    d->selHandleBase = selHandleBase;
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

selhandles::PositionEnum SelectionHandleGraphicsItem::posEnum()
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
    d->selHandleBase->_activeHandle = this;
    d->selHandleBase->handleStartDrag();
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
        if (d->selHandleBase->_activeHandle && d->renderVisible) {
            d->manager->setAllHandlesRenderVisible(false); // move a handle => hide handles
        }
        QPointF curPos = pos();
        // the client can get more information using _activeHandle? (TODO for posEnum)
        d->selHandleBase->handlePositionHasChanged(curPos - d->startPos);
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
    d->selHandleBase->_activeHandle = nullptr;
    d->manager->setAllHandlesRenderVisible(true);
}

void SelectionHandleGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false); // this avoids blurring on certain conditions

    if (d->renderVisible) {

        QGraphicsRectItem::paint(painter, option, widget);
    }

    painter->restore();
}

void SelectionHandleGraphicsItem::setRenderVisible(bool visible)
{
    if (d->renderVisible != visible) {
        d->renderVisible = visible;
        update(); // otherwise - when updating all handles -
                  // only the current's item handles disappear
                  // or the handles do not disappear at all when moving items
    }
}

}
}
