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
#ifndef UUID_2acf85fc_1a8d_11e4_aab7_002454dd224f
#define UUID_2acf85fc_1a8d_11e4_aab7_002454dd224f

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

namespace kreen {
namespace core {

/**
* Visual representation of a KreenItem
*/
class KreenGraphicsItemBase : public QObject
{
    // QObject to have signal/slots
    Q_OBJECT

public:

Q_SIGNALS:
    void operationAccepted();
    void operationCanceled();

    void itemPositionHasChangedSignal(KreenItemPtr item);

public:
    /**
     * scene: to get:
     *  - scene rect
     *  - all selected items
     */
    KreenGraphicsItemBase(QGraphicsItem* graphicsItem, KreenItemPtr item, QGraphicsScene* scene);

    KreenItemPtr item();

    /**
     * the underlying QGraphicsItem
     */
    QGraphicsItem* graphicsItem();

    /**
     * selectable and moveable
     */
    void setMovable(bool isMovable);

    /**
     * WORKAROUND:
     */
    bool workaroundIsBlurredOnUnevenHandleWidth();

    virtual void updateModelFromVisualGeometry() = 0;

    virtual void updateVisualGeometryFromModel() = 0;

    /**
     * used when creating an item
     */
    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) = 0;

    /**
     * when user is releases the mouse button when creating an item
     */
    virtual void setIsCreating(bool creating);

protected:
    /**
     * call this in the ctor of derived class
     */
    virtual void initAndConfigureFromModel() = 0;

    void configurePen(QAbstractGraphicsShapeItem* grItem);

    void configurePen(QGraphicsLineItem* grItem);

    void configureDropShadow(QPoint offset = QPoint(3, 3), qreal blurRadius = 10);

    QRect sceneRect();

    void connectImageOperationAcceptButton(QPushButton* acceptButton);

    void connectImageOperationCancelButton(QPushButton* button);

    bool mousePressEventImpl(QGraphicsSceneMouseEvent* event);

    bool mouseReleaseEventImpl(QGraphicsSceneMouseEvent* event);

    // TMP (TODO)
    void itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

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
    KreenItemPtr _item;

    /**
     * user is still moving the mouse with pressed button
     */
    bool _isCreating;

private:
    QGraphicsItem* _graphicsItem;
    QGraphicsScene* _scene;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
