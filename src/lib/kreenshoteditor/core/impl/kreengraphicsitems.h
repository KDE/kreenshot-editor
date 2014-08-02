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

namespace kreen {
namespace core {

class KreenQGraphicsRectItem : public QGraphicsRectItem, public KreenQGraphicsItemBase
{
public:
    KreenQGraphicsRectItem(KreenItemPtr item, QGraphicsScene* scene) : KreenQGraphicsItemBase(this, item, scene)
    {
        initAndConfigureFromModel();
    }

    virtual void initAndConfigureFromModel() override
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel() override
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry() override
    {
        QPoint scenePos = this->pos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override
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

    virtual void initAndConfigureFromModel() override
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel() override
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry() override
    {
        QPoint scenePos = this->pos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

//     virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override
//     {
//         painter->setRenderHint(QPainter::Antialiasing);
//         QGraphicsEllipseItem::paint(painter, option, widget);
//         painter->setRenderHint(QPainter::Antialiasing, false);
//     }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override
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

    virtual void initAndConfigureFromModel() override
    {
        configureDropShadow();
        configurePen(this);
    }

    virtual void updateVisualGeometryFromModel() override
    {
        this->setLine(_item->line());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {
        this->setLine(QLine(startPoint, endPoint));
    }

    virtual void updateModelFromVisualGeometry() override
    {
        QPoint scenePos = this->pos().toPoint();
        QLine line = this->line().toLine().translated(scenePos); // translate our line to scenePos
        _item->setLine(line);
        this->setPos(0.0, 0.0); // reset QGraphicsItem pos to 0|0 because our model only relies on the line (and not additionally on the pos)
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
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

    virtual void initAndConfigureFromModel() override
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

    virtual void updateVisualGeometryFromModel() override
    {
        this->setRect(0, 0, _item->rect().width(), _item->rect().height());
        this->setPos(_item->rect().x(), _item->rect().y());
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {

    }

    virtual void updateModelFromVisualGeometry() override
    {
        QPoint scenePos = this->pos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override
    {
        itemChangeImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
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

    virtual void initAndConfigureFromModel() override
    {
        this->setPen(QPen(Qt::black, 1, Qt::DotLine));
        //this-set // set everything else dark
    }

    virtual void updateVisualGeometryFromModel() override
    {
        qDebug() << "KreenGraphicsOperationCropItem: updateVisualGeometryFromModel";

        QRect rect = _item->rect();
        this->setRect(0, 0, rect.width(), rect.height());
        this->setPos(rect.x(), rect.y());

        if (_interactionWidget == nullptr) { // TODO: rename this variable
            std::vector<QGraphicsRectItem*> dimRects;
            // 1  2  4
            // 1     4
            // 1  3  4
            _dimRect1 = new QGraphicsRectItem(0, 0, 0, 0, this);
            _dimRect2 = new QGraphicsRectItem(0, 0, 0, 0, this);
            _dimRect3 = new QGraphicsRectItem(0, 0, 0, 0, this);
            _dimRect4 = new QGraphicsRectItem(0, 0, 0, 0, this);
            dimRects.push_back(_dimRect1); // 1
            dimRects.push_back(_dimRect2); // 2
            dimRects.push_back(_dimRect3); // 3
            dimRects.push_back(_dimRect4); // 4

            // updateDimRects(_item->rect()); // later

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

        updateDimRects(rect);
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {
        this->setRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        this->setPos(QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y())));
    }

    virtual void updateModelFromVisualGeometry() override
    {
        qDebug() << "KreenGraphicsOperationCropItem: updateModelFromVisualGeometry";
        _item->setRect(modelRectFromGraphicsItem());
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override
    {
        if (change == QGraphicsItem::ItemPositionHasChanged) {
            updateDimRects(modelRectFromGraphicsItem());
        }
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

private:
    QRect modelRectFromGraphicsItem()
    {
        QPoint scenePos = this->pos().toPoint();
        QRect grRect = this->rect().toRect();
        return QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height());
    }

    void updateDimRects(QRect itemRect)
    {
        if (_dimRect1 == nullptr) // if the rects are not created yet
            return;

        QRect rect = itemRect;
        // 1  2  4
        // 1     4
        // 1  3  4
        _dimRect1->setRect(-rect.x(), -rect.y(), rect.x(), sceneRect().height()); // 1
        _dimRect2->setRect(0, -rect.y(), rect.width(), rect.y()); // 2
        _dimRect3->setRect(0, rect.height(), rect.width(), sceneRect().height() - rect.height() - rect.y()); // 3
        _dimRect4->setRect(rect.width(), -rect.y(), sceneRect().width() - rect.width() - rect.x(), sceneRect().height()); // 4
    }

private:
    QGraphicsProxyWidget* _interactionWidget = nullptr;
    QGraphicsRectItem* _dimRect1;
    QGraphicsRectItem* _dimRect2;
    QGraphicsRectItem* _dimRect3;
    QGraphicsRectItem* _dimRect4;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
