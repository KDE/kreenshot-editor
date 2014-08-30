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
#ifndef UUID_83dcfe08_2c1d_11e4_b317_002454dd224f
#define UUID_83dcfe08_2c1d_11e4_b317_002454dd224f

#include "../../util/sharedptrutil.h"
#include <QSet>
#include <QStack>

class QGraphicsItem;
class KreenGraphicsItemBase;

namespace kreen {
namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(RenderVisibilityControl)

/**
* class which is passed around to be used in the paint method of KreenGraphicsItems
*/
class RenderVisibilityControl
{
public:
    /**
     * see ctor
     */
    static RenderVisibilityControlPtr make_shared();

public:
    RenderVisibilityControl();

    virtual ~RenderVisibilityControl();

    /**
     * this must be called prior to the first item is painted
     */
    void initPainting();

    /**
     * All items up to the given one (exclusivly) will be painted. The others will not be painted
     */
    void pushPaintUpToItem(QGraphicsItem* grItem);

    void popPaintUpToItem();

    /**
     * returns true if the calling paint method should not render, but return instead
     */
    bool onPaintEnter(QGraphicsItem* grItem);

private:
    QStack<QGraphicsItem*> _paintUpToItemStack;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
