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
#ifndef UI_KREENQGRAPHICSITEMS_H
#define UI_KREENQGRAPHICSITEMS_H
#include <QGraphicsDropShadowEffect>

/**
 * multiselection will break the logic
 */
class KreenQGraphicsItemBase
{
public:
    KreenQGraphicsItemBase(QAbstractGraphicsShapeItem* graphicsItem, ItemPtr item)
    {
        _item = item;
        _graphicsItem = graphicsItem;

        _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
        _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable);
        _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    }

    virtual void configureFromModel() = 0;
    virtual void updateGeometryFromModel() = 0;
    virtual void setGeometryToModel() = 0;

protected:
    void configurePen()
    {
        _graphicsItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
    }

    void configureDropShadow()
    {
        if (_item->dropShadow() != nullptr && _item->dropShadow()->enabled) {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(3, 3));
            dropShadow->setBlurRadius(10);
            _graphicsItem->setGraphicsEffect(dropShadow);
        }
    }

    bool mousePressEventImpl(QGraphicsSceneMouseEvent* event)
    {
        // qDebug() << _graphicsItem->scenePos(); // (0, 0)

         QPoint origPos = event->scenePos().toPoint();
         //_graphicsItem->rect();
         _initialMousePosTopLeft = origPos - _item->rect().topLeft();
         qDebug() << "1. mousePressEvent: " << "origPos: " << origPos << " initTopLeft: " << _initialMousePosTopLeft << "item rect: " << _item->rect();

         return true;
    }

    bool mouseReleaseEventImpl(QGraphicsSceneMouseEvent* event)
    {
        QPoint newPos = event->scenePos().toPoint();
        QRect rect = _item->rect();
        rect.moveTopLeft(newPos - _initialMousePosTopLeft);
        qDebug() << "2. mouseReleaseEvent:" << "newPos: " << newPos << "initTopLeft:  " << _initialMousePosTopLeft << "item rect: " << _item->rect();
        updateGeometryFromModel();
        qDebug() << "3. after updateGeometryFromModel: " << "item rect: " << _item->rect();
//
// //         if (wasMoved()) {
// //             //do something and accept the event
// //         } else {
//             // to the base class
//             // QGraphicsItem::mouseReleaseEvent(event);
// //         }

        return true;
    }

    // TMP
    void itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
    {
        //qDebug() << "itemChangeImpl: " << change;

        if (change == QGraphicsItem::ItemPositionChange) {
            //qDebug() << "itemChangeImpl: " << change;

            //QPoint origPos = _graphicsItem->pos().toPoint();
            //QPoint newPos = value.toPoint();

            //updateGeometryFromModel();
        }
    }

protected:
    ItemPtr _item;

private:
    QAbstractGraphicsShapeItem* _graphicsItem;
    QPoint _initialMousePosTopLeft;
};

class KreenQGraphicsRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsRectItem(ItemPtr item) : KreenQGraphicsItemBase(this, item)
    {
        configureFromModel();
    }

    virtual void configureFromModel()
    {
        configureDropShadow();
        configurePen();
    }

    virtual void updateGeometryFromModel()
    {
        this->setRect(_item->rect());
    }

    virtual void setGeometryToModel()
    {
        _item->setRect(this->rect().toRect());
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value)
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        if (mouseReleaseEventImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }
};

class KreenQGraphicsEllipseItem : public QGraphicsEllipseItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsEllipseItem(ItemPtr item) : KreenQGraphicsItemBase(this, item)
    {
        configureFromModel();
    }

    virtual void configureFromModel()
    {
        configureDropShadow();
        configurePen();
    }

    virtual void updateGeometryFromModel()
    {
        this->setRect(_item->rect());
    }

    virtual void setGeometryToModel()
    {
        _item->setRect(this->rect().toRect());
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value)
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        if (mouseReleaseEventImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }
};

#endif // UI_KREENQGRAPHICSITEMS_H

// kate: indent-mode cstyle; replace-tabs on;
