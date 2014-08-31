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
#ifndef UUID_c10df3b230f111e4b8f5002454dd224f
#define UUID_c10df3b230f111e4b8f5002454dd224f

#include <kreen/util/pimplutil.h>
#include <QString>
#include <QGraphicsRectItem>
#include "selectionhandles.h"

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandleGraphicsItem)

class SelectionHandleGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT // QObject to have signal/slots

    friend SelectionHandles;

public:
    SelectionHandleGraphicsItem(SelectionHandles* manager, SelectionHandles::PositionEnum posEnum, SelectionHandleBase* selHandleBase, QRectF rect);

    virtual ~SelectionHandleGraphicsItem();

    SelectionHandles::PositionEnum posEnum();

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

protected:
    void setRenderVisible(bool visible); // used by friend

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
