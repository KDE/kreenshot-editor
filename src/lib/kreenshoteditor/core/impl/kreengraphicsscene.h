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
#include <QList>
#include <memory>
#include <kreen/kreenshoteditor.h>
#include <kreen/core/document.h>

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(KreenGraphicsScene)
KREEN_SHAREDPTR_FORWARD_DECL(Document)
KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)
KREEN_SHAREDPTR_FORWARD_DECL(RenderVisibilityControl)

class KreenGraphicsItemBase;

/**
 * TODO: move all logic from MyQGraphicsView to here to potentially solve the wrong-drawing-when-scrolling-bug
 * and make other things easier
 */
class KreenGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    KREEN_DEFINE_MAKE_SHARED(KreenGraphicsScene)

    KreenGraphicsScene();

    /**
     * to retrieve the base image for the obfuscate tool
     */
    void setDocument(Document* document);

    Document* document();

    /**
     * for interactive handling
     */
    void setToolManager(ToolManagerPtr toolManager);

    /**
     * When image is saved to file things like crop operation tool or selection rects
     * should not be drawn. This is done by hiding all other elements
     */
    void renderFinalImageOnly(bool finalOnly);

    /**
    * All corresponding graphics items for all KreenItems (including op- items like crop or rip out!)
    * Use graphicsItem() on the returned KreenGraphicsItemBase to get the underlying QGraphicsItem.
    */
    QList<KreenGraphicsItemBase*> kreenGraphicsItems();

    /**
     * returns the corresponding KreenGraphicsItemBase* for item or nullptr if not found
     */
    KreenGraphicsItemBase* graphicsItemFromItem(KreenItemPtr item);

    /**
     * retrieves a list of all selected KreenGraphicsItems
     * TODO: what happens on active image operation?
     */
    QList<KreenGraphicsItemBase*> selectedKreenGraphicsItems();

    /**
     * Returns a list of all selected KreenItems
     * INCLUDING the image operation item if exists and selected
     */
    QList<KreenItemPtr> selectedKreenItems();

    /**
     * Remembers all items that are currently selected
     */
    void saveCurrentKreenItemsSelection();

    /**
     * Sets the selection previously saved by saveCurrentKreenItemsSelection().
     * NOTE that the selection is set ONLY. Items that are already selected will NOT be deselected.
     */
    void restoreSavedKreenItemsSelection_1();

    RenderVisibilityControlPtr renderVisibilityControl() { return _renderVisibilityControl; }

Q_SIGNALS:
    void mouseReleasedSignal();

    /**
     * this can be a drawn item like rect or an operation op-crop
     */
    void itemCreated(KreenItemPtr item);

protected:
    bool isItemForPointToSceneRestriction(KreenItemPtr item);

    void restrictPointToScene(QPoint* pt);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;

    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;

    virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;

    virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

private:
    Document* _document;
    ToolManagerPtr _toolManager;
    QGraphicsItem* _creatingItem;
    QPoint _creatingItemStartPoint;
    QList<KreenItemPtr> _savedSelection;
    RenderVisibilityControlPtr _renderVisibilityControl;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
