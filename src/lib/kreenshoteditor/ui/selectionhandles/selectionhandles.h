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
#ifndef UUID_c01b9a44c6643c4eb71e9b4b53b9b71f
#define UUID_c01b9a44c6643c4eb71e9b4b53b9b71f

#include <kreen/util/pimplutil.h>
#include <QString>
#include <QObject>
#include <map>
#include "selectionhandlestypes.h"

class QGraphicsScene;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class SelectionHandleBase;
class SelectionHandleGraphicsItem;

/**
 * GOAL: make this class independent from Kreen-stuff to make it more reusable
 */
class SelectionHandles //: public QObject
{
    //Q_OBJECT

    friend SelectionHandleGraphicsItem;

public:
    SelectionHandles(QGraphicsScene* scene);

    virtual ~SelectionHandles();

public:
    /**
     * True if the user begins to drag a handle.
     * In this case other mouse down actions like create a new item must be prohibited
     */
    bool isAnyHandleUnderMouse();

    void onItemSelectedHasChanged(SelectionHandleBase* selHandleBase);

    void onItemSceneHasChanged(SelectionHandleBase* selHandleBase);

    void onItemPositionHasChanged(SelectionHandleBase* selHandleBase);

    /**
     * createNewHandles == true: create
     * createNewHandles == false: update
     */
    void createOrUpdateHandles(SelectionHandleBase* selHandleBase, bool createNewHandles);

    /**
     * WARN: isVisible is a cached value because we do not want to iterate over all handles all over again
     */
    bool allHandlesRenderVisible();

    /**
     * WARN: isVisible is a cached value
     */
    void setAllHandlesRenderVisible(bool isVisible);

protected:
    void setAllSelectedItemsMovable(bool isMoveable); // used by friend

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
