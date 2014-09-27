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
class QGraphicsView;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class SelectionHandleBase;
class SelectionHandleGraphicsItem;

/**
 * TODO: rename to SelectionHandlesMgr
 */
class SelectionHandles : public QObject
{
    Q_OBJECT

    friend SelectionHandleGraphicsItem;
    friend SelectionHandleBase;

public:
    SelectionHandles();

    virtual ~SelectionHandles();

    /**
     * todo later: only one of the two parameters should be enough
     */
    void setSceneAndView(QGraphicsScene* scene, QGraphicsView* view);

    /**
     * True if the user begins to drag a handle.
     * In this case other mouse down actions like "create a new item" must (can) be prohibited
     * by the client code.
     */
    bool isAnyHandleUnderMouse();

    void setHandlesVisible(bool visible);

protected: // see friend classes
    void onItemSelectedHasChanged(SelectionHandleBase* selHandleBase);

    void onItemSceneHasChanged(SelectionHandleBase* selHandleBase);

    void onItemPositionHasChanged(SelectionHandleBase* selHandleBase);

    /**
     * selHandleBase: the instrumented item which the handles should be created or updated for
     *
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

protected Q_SLOTS:
    // to show selection handles when doing a rubber band selection
    void slotRubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint);

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
