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

class QGraphicsItem;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class SelectionHandleGraphicsItem;

/**
 *
 */
class SelectionHandleBase //: public QObject
{
    //Q_OBJECT

    friend SelectionHandleGraphicsItem;
    friend SelectionHandles;

public:
    void slotHandleStartDrag(); // TODO: cleanup naming
    void slotHandlePositionHasChanged(QPointF delta); // TODO: cleanup naming
    SelectionHandleGraphicsItem* _activeHandle = nullptr; // TODO: move this to separate class to be derived from

    /**
     * if the items should support selection handles, this has to be set
     */
    void setSelectionHandlesMgr(SelectionHandlesPtr selectionHandles);

public:
    SelectionHandleBase();

    virtual ~SelectionHandleBase();

public:
    virtual void handleStartDrag() = 0;
    virtual void handlePositionHasChanged(QPointF delta) = 0;

    /**
     * the selection handles vector to be updated or modified by _selectionHandlesMgr
     * TODO: make not public
     */
    std::vector<SelectionHandleGraphicsItem*> _selectionHandles; // TMP

protected:
    virtual QGraphicsItem* instrumentedItem() = 0;

protected:
    SelectionHandlesPtr _selectionHandlesMgr;
    QRect _startRect; // for rect-base items, todo: move elsewhere?
    QLine _startLine; // for line-base items, todo: move elsewhere?

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
