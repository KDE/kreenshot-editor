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
#include "selectionhandlebase.h"
#include "selectionhandles.h"

namespace kreen {
namespace ui {

class SelectionHandleBase::Impl
{
public:
    HandlesTypeEnum handlesType;

public:

private:
};

SelectionHandleBase::SelectionHandleBase()
{
    KREEN_PIMPL_INIT(SelectionHandleBase);
}

SelectionHandleBase::~SelectionHandleBase()
{
}

void SelectionHandleBase::setSelectionHandlesMgr(SelectionHandlesPtr selectionHandles)
{
    _selectionHandlesMgr = selectionHandles;
}

void SelectionHandleBase::setSelHandleBaseType(SelectionHandleBase::HandlesTypeEnum handlesType)
{
    d->handlesType = handlesType;
}

SelectionHandleBase::HandlesTypeEnum SelectionHandleBase::selHandleBaseType()
{
    return d->handlesType;
}

void SelectionHandleBase::itemChangeSelHandleBaseImpl(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    // qDebug() << "itemChangeImpl: " << change;
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            _selectionHandlesMgr->onItemPositionHasChanged(this);
            if (_selectionHandlesMgr->allHandlesRenderVisible()) {
                _selectionHandlesMgr->setAllHandlesRenderVisible(false); // move an item => hide handles
            }
        }
    }
    else if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            _selectionHandlesMgr->onItemSelectedHasChanged(this);
        }
    }
    else if (change == QGraphicsItem::ItemSceneHasChanged) {
        if (_selectionHandlesMgr) { // only if _selectionHandlesMgr is set (which is not, e.g., for creating items)
            _selectionHandlesMgr->onItemSceneHasChanged(this);
        }
    }
}

void SelectionHandleBase::handleStartDrag()
{
    selHandleBaseStartDrag();
}

void SelectionHandleBase::handlePositionHasChanged(QPointF delta)
{
    selHandleBasePositionHasChanged(delta);
    _selectionHandlesMgr->createOrUpdateHandles(this, false);
}

}
}
