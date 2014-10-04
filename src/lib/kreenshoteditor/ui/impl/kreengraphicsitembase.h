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
#ifndef UUID_2acf85fc1a8d11e4aab7002454dd224f
#define UUID_2acf85fc1a8d11e4aab7002454dd224f

#include <kreen/core/kreenitem.h>
#include <QGraphicsItem>
#include <vector>
#include "../selectionhandles/selectionhandlebase.h"

/**
 * to be put in the protected section of derived items
 */
#define KREENGRAPHICSITEMBASE_DEFAULTIMPLS \
virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override \
{ \
    if (mousePressEventBaseImpl(event)) \
        QGraphicsItem::mousePressEvent(event); \
} \
\
virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override \
{ \
    if (mouseReleaseEventBaseImpl(event)) \
        QGraphicsItem::mouseReleaseEvent(event); \
} \
\
virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent) override \
{ \
    QGraphicsItem::contextMenuEvent(contextMenuEvent); \
    contextMenuEventImpl(contextMenuEvent); \
} \

class QPushButton;
class QAbstractGraphicsShapeItem;
class QGraphicsLineItem;
class QGraphicsSceneMouseEvent;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandlesMgr)

class KreenGraphicsScene;

/**
* Visual representation of a KreenItem
*/
class KreenGraphicsItemBase : public QObject, public SelectionHandleBase
{
    // QObject to have signal/slots
    Q_OBJECT

public:

Q_SIGNALS:
    void operationAcceptedSignal();

    void operationCanceledSignal();

public:
    /**
     * ctor
     */
    KreenGraphicsItemBase(QGraphicsItem* graphicsItem, kreen::core::KreenItemPtr item);

    /**
     * associated KreenItem
     */
    kreen::core::KreenItemPtr item();

    /**
     * the underlying QGraphicsItem
     */
    QGraphicsItem* graphicsItem();

    /**
     * return the KreenGraphicsScene* belonging to the grItem by casting the result of scene()
     */
    KreenGraphicsScene* getScene();

    /**
     * selectable and moveable
     */
    void setSelectableAndMovable(bool isSelectableAndMovable);

    /**
     * selectable only (after a new item was created; to be able to resize instantly)
     */
    void setSelectable(bool isSelectable);

    //bool isStackedBefore(KreenGraphicsItemBase* rhs);

    virtual void updateModelFromVisualGeometry() = 0;

    virtual void updateVisualGeometryFromModel() = 0;

    /**
     * used when creating an item
     */
    virtual void updateVisualGeometryFromPoints(QPoint startPoint, QPoint endPoint) = 0;

    virtual QGraphicsItem* selHandleBaseInstrumentedItem() override;

    virtual void selHandleBase_startDrag() override;

    virtual void selHandleBase_positionHasChanged(selhandles::PositionEnum posEnum, QPointF delta) override;
public:

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

    /**
     * returns true when QGraphicsItem::mousePressEvent should be called
     */
    bool mousePressEventBaseImpl(QGraphicsSceneMouseEvent* event);

    /**
     * returns true when QGraphicsItem::mouseReleaseEvent should be called
     */
    bool mouseReleaseEventBaseImpl(QGraphicsSceneMouseEvent* event);

    void contextMenuEventImpl(QGraphicsSceneContextMenuEvent* contextMenuEvent);

    /**
     * handles common changes:
     * - QGraphicsItem::ItemPositionHasChanged
     * - QGraphicsItem::ItemSelectedHasChanged
     * - QGraphicsItem::ItemSceneHasChanged
     */
    void itemChangeBaseImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

    /**
     * to disable built-in marquee rect
     * see http://www.qtcentre.org/threads/15089-QGraphicsView-change-selected-rectangle-style
     */
    QStyleOptionGraphicsItem copyWithStateSelectedDisabled(const QStyleOptionGraphicsItem* option);

    // TMP
    void afterPaintBaseImpl(QPainter* painter);

protected Q_SLOTS:
    void slotOperationAccepted();

    void slotOperationCanceled();

protected:
    kreen::core::KreenItemPtr _item;

private:
    QGraphicsItem* _graphicsItem;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
