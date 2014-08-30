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
#include "selectionhandles.h"
#include "selectionhandlegraphicsitem.h"
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
#include <QPainter>
#include <cmath>
#include <algorithm>

namespace kreen {
namespace core {

KreenGraphicsItemBase::KreenGraphicsItemBase(QGraphicsItem* graphicsItem, kreen::core::KreenItemPtr item)
{
    _graphicsItem = graphicsItem;
    _item = item;

    _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    setSelectableAndMovable(true); // selectable and moveable by default
}

void KreenGraphicsItemBase::setSelectionHandlesMgr(SelectionHandlesPtr selectionHandles)
{
    _selectionHandlesMgr = selectionHandles;
}

KreenItemPtr KreenGraphicsItemBase::item()
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

void KreenGraphicsItemBase::slotHandleStartDrag()
{
    handleStartDrag();
}

void KreenGraphicsItemBase::slotHandlePositionHasChanged(QPointF delta)
{
    handlePositionHasChanged(delta);
    _selectionHandlesMgr->createOrUpdateHandles(this, false);
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

bool KreenGraphicsItemBase::workaroundIsBlurredOnUnevenHandleWidth()
{
    if (_item->lineStyle() != nullptr && _item->lineStyle()->width % 2 == 1) { // uneven
        return true; // special handling for uneven line widths needed
        // there are probably more cases
    }
    return false;
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

void KreenGraphicsItemBase::handleStartDragRectImpl(QRectF rect)
{
    _startRect = _item->rect();
}

void KreenGraphicsItemBase::handlePositionHasChangedRectImpl(QPointF delta)
{
    _item->setRect(_startRect.adjusted(0, 0, 0, delta.y()));
    updateVisualGeometryFromModel();
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
    // qDebug() << "itemChangeImpl: " << change;
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            //qDebug() << "_selectionHandlesMgr->allHandlesRenderVisible()" << _selectionHandlesMgr->allHandlesRenderVisible();
            _selectionHandlesMgr->onItemPositionHasChanged(this);
            if (_selectionHandlesMgr->allHandlesRenderVisible()) {
                _selectionHandlesMgr->setAllHandlesRenderVisible(false);
            }
        }

        //qDebug() << "EMIT itemPositionHasChangedSignal(item());";
        emit itemPositionHasChangedSignal(item());
    }
    else if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            _selectionHandlesMgr->onItemSelectedHasChanged(this);
        }
    }
    else if (change == QGraphicsItem::ItemSceneHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            _selectionHandlesMgr->onItemSceneHasChanged(this);
        }
    }
}

// TMP
bool showDebugInfo = false;

void KreenGraphicsItemBase::afterPaintBaseImpl(QPainter* painter)
{
    if (showDebugInfo) {
        QString debugInfo = QString("selectable:%1; selected:%2; movable:%3")
        .arg((_graphicsItem->flags() & QGraphicsItem::ItemIsSelectable) > 0)
        .arg(_graphicsItem->isSelected())
        .arg((_graphicsItem->flags() & QGraphicsItem::ItemIsMovable) > 0);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::white));
        painter->drawRect(0, 0, 200, 20);
        painter->setPen(Qt::black);
        painter->drawText(0, 10, debugInfo);
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
