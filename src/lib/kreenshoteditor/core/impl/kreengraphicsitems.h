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
#ifndef UI_KREENQGRAPHICSITEMS_H
#define UI_KREENQGRAPHICSITEMS_H

#include <QGraphicsDropShadowEffect>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QFrame>
#include <QGraphicsScene> // move to cpp file
#include <QDebug> // move to cpp file
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <cmath>
#include <algorithm>
#include <kreen/core/item.h>

using namespace kreen::core;

namespace kreen {
namespace core {

/**
 * multiselection will break the logic
 */
class KreenQGraphicsItemBase : public QObject
{
    Q_OBJECT // to have signal/slots

public:

Q_SIGNALS:
    void operationAccepted();
    void operationCanceled();

    void itemPositionHasChangedSignal();

public:
    /**
     * scene: to get:
     *  - scene rect
     *  - all selected items
     */
    KreenQGraphicsItemBase(QGraphicsItem* graphicsItem, ItemPtr item, QGraphicsScene* scene)
    {
        _item = item;
        _graphicsItem = graphicsItem;
        _scene = scene;

        _graphicsItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges); // needed for itemChange method
        _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, true); // selectable by default
    }

    ItemPtr item()
    {
        return _item;
    }

    void setMovable(bool isMovable)
    {
        _graphicsItem->setFlag(QGraphicsItem::ItemIsSelectable, isMovable);
        _graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
    }

    virtual void updateModelFromVisualGeometry() = 0;

    virtual void updateVisualGeometryFromModel() = 0;

    /**
     * used when creating an item
     */
    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) = 0;

    /**
     * when user is releases the mouse button when creating an item
     */
    virtual void setIsCreating(bool creating)
    {
        _isCreating = creating;
    }

protected:
    /**
     * call this in the ctor of derived class
     */
    virtual void initAndConfigureFromModel() = 0;

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

    QRect sceneRect()
    {
        return _scene->sceneRect().toRect();
    }

    void connectImageOperationAcceptButton(QPushButton* acceptButton)
    {
        qDebug() << "connectImageOperationAcceptButton";
        connect(acceptButton, SIGNAL(clicked()), this, SLOT(operationAcceptedSlot()));
    }

    void connectImageOperationCancelButton(QPushButton* button)
    {
        qDebug() << "connectImageOperationAcceptButton";
        connect(button, SIGNAL(clicked()), this, SLOT(operationCanceledSlot()));
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

    // TMP
    void itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
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

protected Q_SLOTS:
    void operationAcceptedSlot()
    {
        qDebug() << "emit operationAccepted";
        emit operationAccepted();
    }

    void operationCanceledSlot()
    {
        qDebug() << "emit operationCanceled";
        emit operationCanceled();
    }

protected:
    ItemPtr _item;

    /**
     * user is still moving the mouse with pressed button
     */
    bool _isCreating;

private:
    QGraphicsItem* _graphicsItem;
    QGraphicsScene* _scene;
};

class KreenQGraphicsRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsRectItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint)
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry()
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

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0)
    {
        // QGraphicsRectItem::paint(painter, option, widget);
        // see src/qt5/qtbase/src/widgets/graphicsview/qgraphicsitem.cpp

        Q_UNUSED(widget);
        //painter->save();
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawRect(rect());
        //painter->restore();

        // omit the selected rect because we draw it ourselves

        // TODO: idea: round rect
        //painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        //painter.drawRoundRect(100, 200, 100, 200, 10, 10);
    }
};

class KreenQGraphicsEllipseItem : public QGraphicsEllipseItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsEllipseItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint)
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry()
    {
        QPoint scenePos = this->scenePos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

//     virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//     {
//         painter->setRenderHint(QPainter::Antialiasing);
//         QGraphicsEllipseItem::paint(painter, option, widget);
//         painter->setRenderHint(QPainter::Antialiasing, false);
//     }

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

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0)
    {
        // TODO see Rect: reimpl this method to suppress selection marquee
        QGraphicsEllipseItem::paint(painter, option, widget);
    }
};

class KreenQGraphicsLineItem : public QGraphicsLineItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsLineItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel()
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel()
    {
        this->setLine(_item->line());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint)
    {
        this->setLine(QLine(startPoint, endPoint));
    }

    virtual void updateModelFromVisualGeometry()
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
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel()
    {
        setFlag(QGraphicsItem::ItemClipsChildrenToShape); // do not draw text larger than the rect

        configureDropShadow(QPoint(2, 2), 5); // ok?
        configurePen(this);
        // TODO: configure font etc

        auto textGrItem = new QGraphicsTextItem(_item->text->text, this); // create to parent
        textGrItem->setPos(5, 5);

        // later
//         auto proxyTest = new QGraphicsProxyWidget(this);
//         auto lineEdit = new QLineEdit();
//         lineEdit->resize(50, 30);
//         proxyTest->setWidget(lineEdit);
    }

    virtual void updateVisualGeometryFromModel()
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint)
    {

    }

    virtual void updateModelFromVisualGeometry()
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

// ...

class KreenGraphicsOperationCropItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenGraphicsOperationCropItem(ItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel()
    {
        this->setPen(QPen(Qt::black, 1, Qt::DotLine));
        //this-set // set everything else dark
    }

    virtual void updateVisualGeometryFromModel()
    {
        qDebug() << "crop updateVisualGeometryFromModel";

        QRect rect = _item->rect();
        this->setRect(0, 0, rect.width(), rect.height());
        this->setPos(rect.x(), rect.y());

        if (_interactionWidget == nullptr) { // TODO: rename this variable
            std::vector<QGraphicsRectItem*> dimRects;
            // 1  2  4
            // 1     4
            // 1  3  4
            dimRects.push_back(new QGraphicsRectItem(-rect.x(), -rect.y(), rect.x(), sceneRect().height(), this)); // 1
            dimRects.push_back(new QGraphicsRectItem(0, -rect.y(), rect.width(), rect.y(), this)); // 2
            dimRects.push_back(new QGraphicsRectItem(0, rect.height(), rect.width(), sceneRect().height() - rect.height() - rect.y(), this)); // 3
            dimRects.push_back(new QGraphicsRectItem(rect.width(), -rect.y(), sceneRect().width() - rect.width() - rect.x(), sceneRect().height(), this)); // 4

            foreach (auto dimRect, dimRects) {
                dimRect->setBrush(QBrush(QColor(0, 0, 0, 128)));
                dimRect->setPen(Qt::NoPen);
            }

            if (!_isCreating) {
                qDebug() << "crop create proxywidget";
                // TODO: center and make frame transparent
                auto okButton = new QPushButton("Crop (Enter)");
                connectImageOperationAcceptButton(okButton); // from base
                auto cancelButton = new QPushButton("Cancel (Esc)");
                connectImageOperationCancelButton(cancelButton); // from base
                auto hLayout = new QHBoxLayout();
                hLayout->addWidget(okButton);
                hLayout->addWidget(cancelButton);
                auto frame = new QFrame();
                frame->setLayout(hLayout);

                auto widgetProxy = new QGraphicsProxyWidget(this);
                widgetProxy->setWidget(frame);
                _interactionWidget = widgetProxy;

                // causes crash on wild clicking (when interacting with widget) because of model update on mouse release
                // TODO: 2014-06-18: seems not to be an issue anymore...
                //_interactionWidget->setPos(-_interactionWidget->widget()->width(), -_interactionWidget->widget()->height());
                _interactionWidget->setPos(rect.width(), rect.height());
            }
        }
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint)
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry()
    {
        QPoint scenePos = this->scenePos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

private:
    QGraphicsProxyWidget* _interactionWidget = nullptr;
};

}
}

#endif // UI_KREENQGRAPHICSITEMS_H

// kate: indent-mode cstyle; replace-tabs on;
