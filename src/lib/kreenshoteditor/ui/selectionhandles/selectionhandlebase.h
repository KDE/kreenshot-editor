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
#ifndef UUID_619f1e7e30f111e48c94002454dd224f
#define UUID_619f1e7e30f111e48c94002454dd224f

#include <kreen/util/pimplutil.h>
#include <QString>
#include <QObject>
#include <QRect>
#include <QLine>
#include <QGraphicsItem>
#include "selectionhandlestypes.h"

class QGraphicsItem;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class SelectionHandleGraphicsItem;

/**
 * Base class for "instrumented items", i.e., QGraphicsItems that should
 * be decorated with selection handles.
 * The class is multi-inheritance-friendly.
 */
class SelectionHandleBase //: public QObject
{
    //Q_OBJECT

    friend SelectionHandleGraphicsItem;
    friend SelectionHandles;

public:
    SelectionHandleBase();

    virtual ~SelectionHandleBase();

    /**
     * if the items should support selection handles, this has to be set
     */
    void setSelectionHandlesMgr(SelectionHandlesPtr selectionHandles);

    void setSelHandleBaseType(selhandles::HandleTypeEnum handlesType);

    selhandles::HandleTypeEnum selHandleBaseType();

    void itemChangeSelHandleBaseImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

public:
    virtual void selHandleBaseStartDrag() = 0;
    virtual void selHandleBasePositionHasChanged(selhandles::PositionEnum posEnum, QPointF delta) = 0;

protected:
    void handleStartDrag();
    void handlePositionHasChanged(QPointF delta);

protected:
    virtual QGraphicsItem* selHandleBaseInstrumentedItem() = 0;

protected:
    SelectionHandlesPtr _selectionHandlesMgr;

    /**
     * the selection handles vector to be updated or modified by _selectionHandlesMgr
     * TODO: make not public
     */
    std::vector<SelectionHandleGraphicsItem*> _selectionHandles; // TMP

    SelectionHandleGraphicsItem* _activeHandle = nullptr;

//private: // todo later: type is fixed right now
    QRect _startRect; // for rect-base items
    QLine _startLine; // for line-base items

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
