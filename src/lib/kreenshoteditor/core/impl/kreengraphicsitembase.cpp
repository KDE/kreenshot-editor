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
#include <cmath>
#include <algorithm>

namespace kreen {
namespace core {

KreenGraphicsItemBase::KreenGraphicsItemBase(QGraphicsItem* graphicsItem, KreenItemPtr item)
{
    _item = item;
    _graphicsItem = graphicsItem;

    _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    setSelectableAndMovable(true); // selectable and moveable by default
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

bool KreenGraphicsItemBase::mousePressEventImpl(QGraphicsSceneMouseEvent* event)
{
    //QPoint origPos = event->scenePos().toPoint();
    //qDebug() << "1. mousePressEvent: " << "origPos: " << origPos << "item rect: " << _item->rect();
    return true;
}

bool KreenGraphicsItemBase::mouseReleaseEventImpl(QGraphicsSceneMouseEvent* event)
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

void KreenGraphicsItemBase::itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    // qDebug() << "itemChangeImpl: " << change;
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        // qDebug() << "itemChangeImpl: " << change;
        //QPoint origPos = _graphicsItem->pos().toPoint();
        //QPoint newPos = value.toPoint();
        //updateVisualGeometryFromModel();

        //qDebug() << "EMIT itemPositionHasChangedSignal(item());";
        emit itemPositionHasChangedSignal(item());
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
