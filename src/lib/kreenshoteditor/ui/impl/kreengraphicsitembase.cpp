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
#include "kreengraphicsitembase.h"
#include "kreengraphicsscene.h"
#include "../selectionhandles/selectionhandlesmgr.h"
#include "../selectionhandles/selectionhandlegraphicsitem.h"
#include <util/qtworkarounds.h>
#include <QGraphicsScene>
#include <QDebug>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QMenu>
#include <cmath>
#include <algorithm>

namespace kreen {
namespace ui {

///////////////////////////////////////////////
// TMP
bool showDebugInfo = false;
///////////////////////////////////////////////

KreenGraphicsItemBase::KreenGraphicsItemBase(QGraphicsItem* graphicsItem, kreen::core::KreenItemPtr item)
{
    _graphicsItem = graphicsItem;
    _item = item;

    _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    setSelectableAndMovable(true); // selectable and moveable by default
}

kreen::core::KreenItemPtr KreenGraphicsItemBase::item()
{
    return _item;
}

QGraphicsItem* KreenGraphicsItemBase::graphicsItem()
{
    return _graphicsItem;
}

KreenGraphicsScene* KreenGraphicsItemBase::getScene()
{
    return (KreenGraphicsScene*)graphicsItem()->scene();
}

void KreenGraphicsItemBase::setSelectableAndMovable(bool isSelectableAndMovable)
{
    // qDebug() << "KreenGraphicsItemBase::setMovable: " << isMovable;
    _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, isSelectableAndMovable);
    _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, isSelectableAndMovable);

    if (isSelectableAndMovable) {
        _graphicsItem->setCursor(Qt::SizeAllCursor);
    }
    else {
        _graphicsItem->unsetCursor();
    }
}

void KreenGraphicsItemBase::setSelectable(bool isSelectable)
{
    _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, isSelectable);
}

// NOT TESTED
// bool KreenGraphicsItemBase::isStackedBefore(KreenGraphicsItemBase* rhs)
// {
//     auto list = rhs->getScene()->kreenGraphicsItems();
//     return list.indexOf(this) < list.indexOf(rhs);
// }

QGraphicsItem* KreenGraphicsItemBase::selHandleBaseInstrumentedItem()
{
    return graphicsItem();
}

void KreenGraphicsItemBase::selHandleBase_startDrag()
{
    if (selHandleBase_type() == selhandles::HandleType_ResizeRect) {
        _startRect = _item->rect();
    }
    else if (selHandleBase_type() == selhandles::HandleType_ResizeLine) {
        _startLine = _item->line();
    }
    else {
        qDebug() << "[ERROR] KreenGraphicsItemBase::selHandleBaseStartDrag() not impl.";
        Q_ASSERT(false);
    }
}

void KreenGraphicsItemBase::selHandleBase_positionHasChanged(selhandles::PositionEnum posEnum, QPointF delta)
{
    qreal dx = delta.x();
    qreal dy = delta.y();

    if (selHandleBase_type() == selhandles::HandleType_ResizeRect) {

        QRect newRect; // model

        switch (posEnum)
        {
            // case selhandles::Position0_Center: Q_ASSERT(false); return;
            case selhandles::Position1_TopLeft:
                newRect = _startRect.adjusted(dx, dy, 0, 0);
                break;
            case selhandles::Position2_TopRight:
                newRect = _startRect.adjusted(0, dy, dx, 0);
                break;
            case selhandles::Position3_BottomLeft:
                newRect = _startRect.adjusted(dx, 0, 0, dy);
                break;
            case selhandles::Position4_BottomRight:
                newRect = _startRect.adjusted(0, 0, dx, dy);
                break;
            case selhandles::Position5_Left:
                newRect = _startRect.adjusted(dx, 0, 0, 0);
                break;
            case selhandles::Position6_Right:
                newRect = _startRect.adjusted(0, 0, dx, 0);
                break;
            case selhandles::Position7_Top:
                newRect = _startRect.adjusted(0, dy, 0, 0);
                break;
            case selhandles::Position8_Bottom:
                newRect = _startRect.adjusted(0, 0, 0, dy);
                break;
            default:
                qDebug() << "[ERROR] KreenGraphicsItemBase::selHandleBasePositionHasChanged case not handled: " << posEnum;
                Q_ASSERT(false);
                return;
        }

        newRect = newRect.normalized(); // for when e.g. lower handle is moved over the top edge
        _item->setRect(newRect);
        updateVisualGeometryFromModel();
    }
    else if (selHandleBase_type() == selhandles::HandleType_ResizeLine) {
        QLine newLine; // model

        switch (posEnum)
        {
            // case selhandles::Position0_Center: Q_ASSERT(false); return;
            case selhandles::Position_LineStart:
                newLine = QLine(_startLine.x1() + dx, _startLine.y1() + dy, _startLine.x2(), _startLine.y2());
                break;
            case selhandles::Position_LineEnd:
                newLine = QLine(_startLine.x1(), _startLine.y1(), _startLine.x2() + dx, _startLine.y2() + dy);
                break;
            default:
                qDebug() << "[ERROR] KreenGraphicsItemBase::selHandleBasePositionHasChanged case not handled: " << posEnum;
                Q_ASSERT(false);
                return;
        }

        _item->setLine(newLine);
        updateVisualGeometryFromModel();
    }
    else {
        qDebug() << "[ERROR] KreenGraphicsItemBase::selHandleBasePositionHasChanged() not impl.";
        Q_ASSERT(false);
    }
}

void KreenGraphicsItemBase::configurePen(QAbstractGraphicsShapeItem* grItem)
{
    // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
    Q_ASSERT(_item->lineColor());
    Q_ASSERT(_item->lineStyle());
    grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
}
void KreenGraphicsItemBase::configurePen(QGraphicsLineItem* grItem)
{
    // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
    Q_ASSERT(_item->lineColor());
    Q_ASSERT(_item->lineStyle());
    grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
}

void KreenGraphicsItemBase::configureDropShadow(QPoint offset, qreal blurRadius)
{
    // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
    Q_ASSERT(_item->dropShadow());
    if (_item->dropShadow()->enabled) {
        auto dropShadow = new QGraphicsDropShadowEffect();
        dropShadow->setColor(Qt::black);
        dropShadow->setOffset(offset);
        dropShadow->setBlurRadius(blurRadius);
        _graphicsItem->setGraphicsEffect(dropShadow);
    }
}

QRect KreenGraphicsItemBase::sceneRect()
{
    return _graphicsItem->scene()->sceneRect().toRect();
}

void KreenGraphicsItemBase::connectImageOperationAcceptButton(QPushButton* acceptButton)
{
    qDebug() << "connectImageOperationAcceptButton";
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(slotOperationAccepted()));
}

void KreenGraphicsItemBase::connectImageOperationCancelButton(QPushButton* button)
{
    qDebug() << "connectImageOperationCancelButton";
    connect(button, SIGNAL(clicked()), this, SLOT(slotOperationCanceled()));
}

bool KreenGraphicsItemBase::mousePressEventBaseImpl(QGraphicsSceneMouseEvent* event)
{
    //QPoint origPos = event->scenePos().toPoint();
    //qDebug() << "1. mousePressEvent: " << "origPos: " << origPos << "item rect: " << _item->rect();
    return true;
}

bool KreenGraphicsItemBase::mouseReleaseEventBaseImpl(QGraphicsSceneMouseEvent* event)
{
    //QPoint newPos = event->scenePos().toPoint();

    qDebug() << "mouseReleaseEventImpl: updateModelFromVisualGeometry() for all selected items";
    foreach (auto gritem, _graphicsItem->scene()->selectedItems()) {
        auto gritemBase = dynamic_cast<KreenGraphicsItemBase*>(gritem);
        if (gritemBase != nullptr) { // there might also be other items
            gritemBase->updateModelFromVisualGeometry();
        }
        else {
            qDebug() << "?? who was it";
        }
    }

    //qDebug() << "  2. mouseReleaseEvent:" << "newPos: " << newPos << "item rect: " << _item->rect();

    // test
//         updateVisualGeometryFromModel();
//         qDebug() << "  3. after updateVisualGeometryFromModel: " << "item rect: " << _item->rect();

// //         if (wasMoved()) {
// //             //do something and accept the event
// //         } else {
//             // to the base class
//             // QGraphicsItem::mouseReleaseEvent(event);
// //         }

    return true;
}

void KreenGraphicsItemBase::itemChangeBaseImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    selHandleBase_itemChangeImpl(change, value);
}

QStyleOptionGraphicsItem KreenGraphicsItemBase::copyWithStateSelectedDisabled(const QStyleOptionGraphicsItem* option)
{
    // see http://www.qtcentre.org/threads/15089-QGraphicsView-change-selected-rectangle-style
    QStyleOptionGraphicsItem myOption = (*option);
    myOption.state &= !QStyle::State_Selected;
    return myOption;
}

void KreenGraphicsItemBase::contextMenuEventImpl(QGraphicsSceneContextMenuEvent* contextMenuEvent)
{
    if (!contextMenuEvent->isAccepted()) {
        contextMenuEvent->accept();
        QMenu menu;
        auto a1 = new QAction("Action AAA 1", nullptr);
        auto a2 = new QAction("Action AAA 2", nullptr);
        auto a3 = new QAction("Action AAA 3", nullptr);

        connect(a1, SIGNAL(triggered()), this, SLOT(slotTest()));

        menu.addAction(a1);
        menu.addAction(a2);
        menu.addAction(a3);
        menu.exec(contextMenuEvent->screenPos());

        // with this in place, after dismissing the context menu (by e.g. clicking an menu item)
        // and the mouse is not over the item anymore, the cursor will NOT change back to the
        // "size all" cursor (which only goes away by moving the mouse). So we "move" the mouse
        // and it never appears at the first place.
        WORKAROUNDS::sendFakeMouseEvent(contextMenuEvent);
    }
}

void KreenGraphicsItemBase::afterPaintBaseImpl(QPainter* painter)
{
    if (showDebugInfo) {
        painter->save();
        QString debugInfo = QString("id:%1; selectable:%2; selected:%3; movable:%4")
        .arg(item()->id())
        .arg((_graphicsItem->flags() & QGraphicsItem::ItemIsSelectable) > 0)
        .arg(_graphicsItem->isSelected())
        .arg((_graphicsItem->flags() & QGraphicsItem::ItemIsMovable) > 0);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::white));
        painter->drawRect(0, 0, 300, 20);
        painter->setPen(Qt::black);
        painter->drawText(0, 10, debugInfo);
        painter->restore();
    }
}

void KreenGraphicsItemBase::slotOperationAccepted()
{
    qDebug() << "emit operationAccepted";
    emit operationAcceptedSignal();
}

void KreenGraphicsItemBase::slotOperationCanceled()
{
    qDebug() << "emit operationCanceled";
    emit operationCanceledSignal();
}

}
}
