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

namespace kreen {
namespace core {

KreenQGraphicsItemBase::KreenQGraphicsItemBase(QGraphicsItem* graphicsItem, KreenItemPtr item, QGraphicsScene* scene)
{
    _item = item;
    _graphicsItem = graphicsItem;
    _scene = scene;

    _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    setMovable(true); // selectable and moveable by default

    _graphicsItem->setCursor(Qt::SizeAllCursor);
}
KreenItemPtr KreenQGraphicsItemBase::item()
{
    return _item;
}
QGraphicsItem* KreenQGraphicsItemBase::graphicsItem()
{
    return _graphicsItem;
}
void KreenQGraphicsItemBase::setMovable(bool isMovable)
{
    qDebug() << "KreenQGraphicsItemBase::setMovable: " << isMovable;
    //bool isSelectable = _graphicsItem->flags() | QGraphicsItem::ItemIsSelectable;
    //if (isSelectable != isMovable) {
    _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, isMovable);
    //}
    _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
}
bool KreenQGraphicsItemBase::workaroundIsBlurredOnUnevenHandleWidth()
{
    if (_item->lineStyle()->width % 2 == 1) { // uneven
        return true; // special handling for uneven line widths needed
        // there are probably more cases
    }
    return false;
}
void KreenQGraphicsItemBase::setIsCreating(bool creating)
{
    _isCreating = creating;
}
void KreenQGraphicsItemBase::configurePen(QAbstractGraphicsShapeItem* grItem)
{
    // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
    Q_ASSERT(_item->lineColor());
    Q_ASSERT(_item->lineStyle());
    grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
}
void KreenQGraphicsItemBase::configurePen(QGraphicsLineItem* grItem)
{
    // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
    Q_ASSERT(_item->lineColor());
    Q_ASSERT(_item->lineStyle());
    grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
}
void KreenQGraphicsItemBase::configureDropShadow(QPoint offset, qreal blurRadius)
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
QRect KreenQGraphicsItemBase::sceneRect()
{
    return _scene->sceneRect().toRect();
}
void KreenQGraphicsItemBase::connectImageOperationAcceptButton(QPushButton* acceptButton)
{
    qDebug() << "connectImageOperationAcceptButton";
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(operationAcceptedSlot()));
}
void KreenQGraphicsItemBase::connectImageOperationCancelButton(QPushButton* button)
{
    qDebug() << "connectImageOperationAcceptButton";
    connect(button, SIGNAL(clicked()), this, SLOT(operationCanceledSlot()));
}
bool KreenQGraphicsItemBase::mousePressEventImpl(QGraphicsSceneMouseEvent* event)
{
    //QPoint origPos = event->scenePos().toPoint();
    //qDebug() << "1. mousePressEvent: " << "origPos: " << origPos << "item rect: " << _item->rect();
    return true;
}
bool KreenQGraphicsItemBase::mouseReleaseEventImpl(QGraphicsSceneMouseEvent* event)
{
    //QPoint newPos = event->scenePos().toPoint();

    qDebug() << "updateModelFromVisualGeometry() for all selected items";
    foreach (auto gritem, _scene->selectedItems()) {
        auto gritemBase = dynamic_cast<KreenQGraphicsItemBase*>(gritem);
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
void KreenQGraphicsItemBase::itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    // qDebug() << "itemChangeImpl: " << change;
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        // qDebug() << "itemChangeImpl: " << change;
        //QPoint origPos = _graphicsItem->pos().toPoint();
        //QPoint newPos = value.toPoint();
        //updateVisualGeometryFromModel();

        //qDebug() << "emit itemPositionHasChangedSignal();";
        emit itemPositionHasChangedSignal();
    }
}

}
}
