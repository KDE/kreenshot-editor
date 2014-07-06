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
#include <kreen/core/kreenitem.h>
#include "kreengraphicsitembase.h"

using namespace kreen::core;

namespace kreen {
namespace core {

class KreenQGraphicsRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsRectItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
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
    KreenQGraphicsEllipseItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
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
    KreenQGraphicsLineItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
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
    KreenGraphicsTextRectItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
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
    KreenGraphicsOperationCropItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
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
