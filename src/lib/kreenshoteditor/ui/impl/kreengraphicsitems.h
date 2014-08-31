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
#ifndef UUID_312a07561a8d11e48d2e002454dd224f
#define UUID_312a07561a8d11e48d2e002454dd224f

#include <QGraphicsDropShadowEffect>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QFrame>
#include <QGraphicsScene> // todo: move to cpp file
#include <QDebug> // todo: move to cpp file
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
namespace ui {

class KreenGraphicsRectItem : public QGraphicsRectItem, public KreenGraphicsItemBase
{
public:
    KreenGraphicsRectItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Rect);
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
        itemChangeBaseImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
};

class KreenGraphicsEllipseItem : public QGraphicsEllipseItem, public KreenGraphicsItemBase
{
public:
    KreenGraphicsEllipseItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Rect);
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
        itemChangeBaseImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
};

class KreenGraphicsLineItem : public QGraphicsLineItem, public KreenGraphicsItemBase
{
public:
    KreenGraphicsLineItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Line);
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
        itemChangeBaseImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }
};

class KreenGraphicsTextRectItem : public QGraphicsRectItem, public KreenGraphicsItemBase
{
public:
    KreenGraphicsTextRectItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Rect);
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
        // todo?
    }

    virtual void updateModelFromVisualGeometry() override
    {
        QPoint scenePos = this->pos().toPoint();
        QRect grRect = this->rect().toRect();
        _item->setRect(QRect(scenePos.x(), scenePos.y(), grRect.width(), grRect.height()));
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value) override
    {
        itemChangeBaseImpl(change, value);
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }
};

class KreenGraphicsObfuscateItem : public QGraphicsRectItem, public KreenGraphicsItemBase
{
public:
    bool pixelizeEffect = false; // true if pixelizeEffect else blur        // TODO: make configurable via properties

    KreenGraphicsObfuscateItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Rect);
    }

    virtual void initAndConfigureFromModel() override
    {
        configureDropShadow();
        configurePen(this);

//        // this is just a black semitransparent rect with blurred borders
//         QColor color(Qt::black);
//         color.setAlpha(200);
//         setBrush(QBrush(color));
//         auto effect = new QGraphicsBlurEffect();
//         effect->setBlurRadius(10);
//         setGraphicsEffect(effect);

        if (!pixelizeEffect) { // ggf. auch pixelize und blur
            auto blurEffect = new QGraphicsBlurEffect();
            blurEffect->setBlurRadius(5);
            this->setGraphicsEffect(blurEffect);
        }
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
        itemChangeBaseImpl(change, value);
        if (change == QGraphicsItem::ItemPositionHasChanged) {
            // here we trigger an item update to immediately show the recalcuted
            // obfuscated image area while moving the item around
            update();
        }
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
};

class KreenGraphicsOperationCropItem : public QGraphicsRectItem, public KreenGraphicsItemBase
{
public:
    KreenGraphicsOperationCropItem(KreenItemPtr item) : KreenGraphicsItemBase(this, item)
    {
        initAndConfigureFromModel();
        setSelHandleBaseType(SelectionHandleBase::HandleType_Rect);
    }

    virtual void initAndConfigureFromModel() override
    {
        this->setPen(QPen(Qt::black, 1, Qt::DotLine));
        //this-set // set everything else dark
    }

    virtual void updateVisualGeometryFromModel() override
    {
        qDebug() << "KreenGraphicsOperationCropItem: updateVisualGeometryFromModel";

        QRect itemRect = _item->rect(); // in KreenItems the position is coded in the rect's top/left coordinate
        // here we translate it into QGraphicsItems way:
        this->setRect(0, 0, itemRect.width(), itemRect.height());
        this->setPos(itemRect.x(), itemRect.y());

        if (_interactionWidget == nullptr) { // TODO: rename this variable
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
            _interactionWidget->setPos(itemRect.width(), itemRect.height());
        }

        updateDimRects(itemRect);
    }

    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) override
    {
        //qDebug() << "KreenGraphicsOperationCropItem::updateVisualGeometryFromPoints";

        QRect rect = QRect(0, 0, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
        QPoint pos = QPoint(std::min(startPoint.x(), endPoint.x()), std::min(startPoint.y(), endPoint.y()));
        this->setRect(rect);
        this->setPos(pos);

        // updateDimRects needs the itemRect (with position encoded in top/left of the rect)
        QRect itemRect(pos.x(), pos.y(), rect.width(), rect.height());
        updateDimRects(itemRect);
    }

    virtual void updateModelFromVisualGeometry() override
    {
        qDebug() << "KreenGraphicsOperationCropItem: updateModelFromVisualGeometry";
        _item->setRect(modelRectFromGraphicsItem());
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override
    {
        itemChangeBaseImpl(change, value); // default handling by base impl
        if (change == QGraphicsItem::ItemPositionHasChanged) {
            updateDimRects(modelRectFromGraphicsItem());
        }
        return QGraphicsItem::itemChange(change, value);
    }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mousePressEventBaseImpl(event))
            QGraphicsItem::mousePressEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (mouseReleaseEventBaseImpl(event))
            QGraphicsItem::mouseReleaseEvent(event);
    }

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override
    {
        afterPaintBaseImpl(painter);
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
        if (_dimRect1 == nullptr) {
            // if the rects are not created yet => create them

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

            foreach (auto dimRect, dimRects) {
                dimRect->setBrush(QBrush(QColor(0, 0, 0, 128)));
                dimRect->setPen(Qt::NoPen);
            }
        }

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
    QGraphicsRectItem* _dimRect1 = nullptr;
    QGraphicsRectItem* _dimRect2 = nullptr;
    QGraphicsRectItem* _dimRect3 = nullptr;
    QGraphicsRectItem* _dimRect4 = nullptr;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
