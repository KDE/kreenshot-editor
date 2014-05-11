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
#include <QAbstractGraphicsShapeItem>
#include "../core/item.h"

/**
 * multiselection will break the logic
 */
class KreenQGraphicsItemBase
{
public:
    /**
     * scene: to get all selected items
     */
    KreenQGraphicsItemBase(QGraphicsItem* graphicsItem, ItemPtr item, QGraphicsScene* scene)
    {
        _item = item;
        _graphicsItem = graphicsItem;
        _scene = scene;

        _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable);
        _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable);
        _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
    }

    void setMovable(bool isMovable)
    {
        _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
    }

    virtual void updateGeometryFromModel() = 0;

protected:
    virtual void configureFromModel() = 0;
    virtual void setGeometryToModel() = 0;

    void configurePen(QAbstractGraphicsShapeItem* grItem)
    {
        // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
        Q_ASSERT(_item->lineColor());
        Q_ASSERT(_item->lineStyle());
        grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
    }

    void configurePen(QGraphicsLineItem* grItem)
    {
        // items that want to configure these properties will have them otherwise it is programming error (prototype pattern)
        Q_ASSERT(_item->lineColor());
        Q_ASSERT(_item->lineStyle());
        grItem->setPen(QPen(_item->lineColor()->color, _item->lineStyle()->width, _item->lineStyle()->penStyle, Qt::RoundCap, Qt::RoundJoin));
    }

    void configureDropShadow(QPoint offset = QPoint(3, 3), qreal blurRadius = 10)
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

    bool mousePressEventImpl(QGraphicsSceneMouseEvent* event)
    {
         //QPoint origPos = event->scenePos().toPoint();
         //qDebug() << "1. mousePressEvent: " << "origPos: " << origPos << "item rect: " << _item->rect();
         return true;
    }

    bool mouseReleaseEventImpl(QGraphicsSceneMouseEvent* event)
    {
        //QPoint newPos = event->scenePos().toPoint();

        qDebug() << "setGeometryToModel() for all selected items";
        foreach (auto gritem, _scene->selectedItems()) {
            auto gritemBase = dynamic_cast<KreenQGraphicsItemBase*>(gritem);
            if (gritemBase != nullptr) { // there might also be other items
                gritemBase->setGeometryToModel();
            }
            else {
                qDebug() << "?? who was it";
            }
        }

        //qDebug() << "  2. mouseReleaseEvent:" << "newPos: " << newPos << "item rect: " << _item->rect();

        // test
//         updateGeometryFromModel();
//         qDebug() << "  3. after updateGeometryFromModel: " << "item rect: " << _item->rect();

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
    QGraphicsItem* _graphicsItem;
    QGraphicsScene* _scene;
};

class KreenQGraphicsRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsRectItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        configureFromModel();
    }

    virtual void configureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void setGeometryToModel()
    {
        QPoint scenePos = this->scenePos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
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
    KreenQGraphicsEllipseItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        configureFromModel();
    }

    virtual void configureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void setGeometryToModel()
    {
        QPoint scenePos = this->scenePos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
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

class KreenQGraphicsLineItem : public QGraphicsLineItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsLineItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        configureFromModel();
    }

    virtual void configureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateGeometryFromModel()
    {
        this->setLine(_item->line());
        //this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void setGeometryToModel()
    {
        //QPoint scenePos = this->scenePos().toPoint();
        QLine line = this->line().toLine();
        _item->setLine(line);
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

class KreenGraphicsTextRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenGraphicsTextRectItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        configureFromModel();
        setFlag(QGraphicsItem::ItemClipsChildrenToShape); // do not draw text larger than the rect
    }

    virtual void configureFromModel()
    {
        configureDropShadow(QPoint(2, 2), 5); // ok?
        // configurePen(this); // TODO: configure font etc

        auto textGrItem = new QGraphicsTextItem("With drop shadow TODO: apply attributes; multiline; edit; don't display if outside rect", this);
        textGrItem->setPos(5, 5);
    }

    virtual void updateGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void setGeometryToModel()
    {
        QPoint scenePos = this->scenePos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
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
