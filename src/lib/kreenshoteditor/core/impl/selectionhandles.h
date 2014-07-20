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
#include <map>

class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsScene;

namespace kreen {
namespace core {

KREEN_PIMPL_FORWARD_DECL(SelectionHandles)
KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class MainEditorWidgetImpl;
class SelectionHandleGraphicsItem;

class SelectionHandles
{
public:
    SelectionHandles(QGraphicsScene* scene);

    virtual ~SelectionHandles() { }

public:
    void redrawSelectionHandles(bool createNewHandles);

private:
    SelectionHandlesImplPtr d;
};

}
}

#endif
