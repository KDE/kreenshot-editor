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

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandlesMgr)
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
    friend SelectionHandlesMgr;

public:
    SelectionHandleBase();

    virtual ~SelectionHandleBase();

    void selHandleBase_setType(selhandles::HandleTypeEnum handlesType);

    selhandles::HandleTypeEnum selHandleBase_type();

    void selHandleBase_itemChangeImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value);

public:
    virtual void selHandleBase_startDrag() = 0;
    virtual void selHandleBase_positionHasChanged(selhandles::PositionEnum posEnum, QPointF delta) = 0;
    /**
     * return the line of the line based graphics item,
     * e.g. for QGraphicsLineItem it is line().
     * For QGraphicsLineItem there is a default implementation provided
     */
    virtual QLineF selHandleBase_sceneLine();

private: // see friend classes
    void handleStartDrag();
    void handlePositionHasChanged(QPointF delta);

    /**
     * if the items should support selection handles, this has to be set, see SelectionHandlesMgr::registerItem
     */
    void setSelectionHandlesMgr(SelectionHandlesMgrPtr selectionHandlesMgr);

    virtual QGraphicsItem* selHandleBaseInstrumentedItem() = 0;

protected: // see friend classes
    SelectionHandlesMgrPtr _selectionHandlesMgr;

    /**
     * the selection handles vector to be updated or modified by _selectionHandlesMgr
     * TODO: make not public
     */
    std::vector<SelectionHandleGraphicsItem*> _selectionHandles; // TMP

    SelectionHandleGraphicsItem* _activeHandle = nullptr;

//private: // todo later: type is fixed right now
    // model start for delta calc, assuming the model has int (not float) type:
    QRect _startRect; // for rect-base items
    QLine _startLine; // for line-base items

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
