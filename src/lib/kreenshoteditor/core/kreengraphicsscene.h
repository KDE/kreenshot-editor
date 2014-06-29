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
#ifndef UI_MYQGRAPHICSSCENE_H
#define UI_MYQGRAPHICSSCENE_H

#include <kreen/util/sharedptrutil.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <memory>
#include <kreen/kreenshoteditor.h>
#include <kreen/core/document.h>

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(KreenGraphicsScene)
KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)

/**
 * TODO: move all logic from MyQGraphicsView to here to potentially solve the wrong-drawing-when-scrolling-bug
 * and make other things easier
 */
class KreenGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    KreenGraphicsScene();

    /**
     * When image is saved to file things like crop operation tool or selection rects
     * should not be drawn. This is done by hiding all other elements
     */
    void renderFinalImageOnly(bool finalOnly);

    /**
     * for interactive handling
     */
    void setToolManager(ToolManagerPtr toolManager);

Q_SIGNALS:
    void mouseReleased();

    /**
     * this can be drawn item like rect or an operation op-crop
     */
    void itemCreated(ItemPtr item);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    bool isItemForPointToSceneRestriction(ItemPtr item);

    void restrictPointToScene(QPoint* pt);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    ToolManagerPtr _toolManager;
    QGraphicsItem* _creatingItem;
    QPoint _creatingItemStartPoint;
};

}
}

#endif // UI_MYQGRAPHICSSCENE_H

// kate: indent-mode cstyle; replace-tabs on;
