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
#include "selectionhandles.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "selectionhandlegraphicsitem.h"
#include "selectionhandlebase.h"

namespace kreen {
namespace ui {

class SelectionHandles::Impl
{
public:
    QGraphicsScene* scene;
    QGraphicsView* view;
    bool allRenderVisible = true;

public:
    Impl(SelectionHandles* owner)
    {
        _owner = owner;
    }

    void clearHandlesFromScene(SelectionHandleBase* selHandleBase)
    {
        foreach (auto handleItem, selHandleBase->_selectionHandles) {
            scene->removeItem(handleItem);
        }
        selHandleBase->_selectionHandles.clear();
    }

    QList<SelectionHandleBase*> selectedSelHandleItems()
    {
        QList<SelectionHandleBase*> result;

        foreach (auto grItem, scene->selectedItems()) {

            auto item = dynamic_cast<SelectionHandleBase*>(grItem);
            if (item != nullptr) {
                result.append(item);
            }
        }

        return result;
    }

private:
    SelectionHandles* _owner = nullptr;
};


SelectionHandles::SelectionHandles(QGraphicsScene* scene, QGraphicsView* view) {
    qDebug() << "SelectionHandles::ctor";
    KREEN_PIMPL_INIT_THIS(SelectionHandles);
    d->scene = scene;
    d->view = view;

    connect(view, SIGNAL(rubberBandChanged(QRect, QPointF, QPointF)), this, SLOT(slotRubberBandChanged(QRect, QPointF, QPointF)));
}

SelectionHandles::~SelectionHandles()
{
    qDebug() << "SelectionHandles destructor";
}

void SelectionHandles::onItemSelectedHasChanged(SelectionHandleBase* selHandleBase)
{
    if (selHandleBase->selHandleBaseInstrumentedItem()->isSelected()) {
        createOrUpdateHandles(selHandleBase, true);

        foreach (auto handleItem, selHandleBase->_selectionHandles) {
            d->scene->addItem(handleItem);
        }
    }
    else {
        d->clearHandlesFromScene(selHandleBase);
    }
}

void SelectionHandles::onItemSceneHasChanged(SelectionHandleBase* selHandleBase)
{
    if (!selHandleBase->selHandleBaseInstrumentedItem()->scene()) { // check whether item is part of a scene or not
        d->clearHandlesFromScene(selHandleBase); // remove handles if instrumentedItem was removed from scene
    }
}

void SelectionHandles::onItemPositionHasChanged(SelectionHandleBase* selHandleBase)
{
    if (selHandleBase->_selectionHandles.size() > 0) { // update if handles are present
        createOrUpdateHandles(selHandleBase, false);
    }
}

void SelectionHandles::createOrUpdateHandles(SelectionHandleBase* selHandleBase, bool createNewHandles)
{
    qreal hw = 8.0; // handleWidth;

    auto grItem = selHandleBase->selHandleBaseInstrumentedItem();

    // WORKAROUND:
    // handle width, TODO: why? even or uneven numbers: these or those items will have blurred rects
    //                     the underlying item is also blurred
    //                     WTF-->the black selection rects get also blurred

    // TODO: bring back in later
//     if (grItem->workaroundIsBlurredOnUnevenHandleWidth()) {
//         qDebug() << "INFO: workaround used";
//         hw--;
//     }

    qreal hw2 = hw / 2.0;

    /**
    * 1   7   2  (for rects)
    * 5   0   6
    * 3   8   4
    */
    std::vector<selhandles::PositionEnum> positions; // positions that will be used; the ORDER is important when updating existing handles
    QMap<selhandles::PositionEnum, QRectF> posRectMap; // map posistion --> rect of the handle at this position

    if (selHandleBase->selHandleBaseType() == selhandles::HandleType_ResizeRect) {
        auto baseRect = grItem->sceneBoundingRect(); // TODO: how to do it with lines? Make extra class?
        qreal x = baseRect.x();
        qreal y = baseRect.y();
        qreal w = baseRect.width();
        qreal h = baseRect.height();
        qreal w2 = floor(w / 2.0);
        qreal h2 = floor(h / 2.0);
        auto rect = QRectF(0, 0, hw, hw);

        positions.push_back(selhandles::Position1_TopLeft);
        positions.push_back(selhandles::Position2_TopRight);
        positions.push_back(selhandles::Position3_BottomLeft);
        positions.push_back(selhandles::Position4_BottomRight);
        positions.push_back(selhandles::Position5_Left);
        positions.push_back(selhandles::Position6_Right);
        positions.push_back(selhandles::Position7_Top);
        positions.push_back(selhandles::Position8_Bottom);

        posRectMap.insert(selhandles::Position1_TopLeft, rect.translated(x - hw2, y - hw2));
        posRectMap.insert(selhandles::Position2_TopRight, rect.translated(x + w - hw2, y - hw2));
        posRectMap.insert(selhandles::Position3_BottomLeft, rect.translated(x - hw2, y + h - hw2));
        posRectMap.insert(selhandles::Position4_BottomRight, rect.translated(x + w - hw2, y + h - hw2));
        posRectMap.insert(selhandles::Position5_Left, rect.translated(x - hw2, y + h2 - hw2));
        posRectMap.insert(selhandles::Position6_Right, rect.translated(x + w - hw2, y + h2 - hw2));
        posRectMap.insert(selhandles::Position7_Top, rect.translated(x + w2 - hw2, y - hw2));
        posRectMap.insert(selhandles::Position8_Bottom, rect.translated(x + w2 - hw2, y + h - hw2));
    }
    else if (selHandleBase->selHandleBaseType() == selhandles::HandleType_ResizeLine) {
        positions.push_back(selhandles::Position_LineStart);
        positions.push_back(selhandles::Position_LineEnd);

        // TMP
        auto baseRect = grItem->sceneBoundingRect();
        qreal x = baseRect.x();
        qreal y = baseRect.y();
        qreal w = baseRect.width();
        qreal h = baseRect.height();
        auto rect = QRectF(0, 0, hw, hw);

        posRectMap.insert(selhandles::Position_LineStart, rect.translated(x - hw2, y - hw2));
        posRectMap.insert(selhandles::Position_LineEnd, rect.translated(x + w - hw2, y + h - hw2));
    }
    else {
        qDebug() << "[ERROR] SelectionHandles::createOrUpdateHandles";
        Q_ASSERT(false);
    }

    if (createNewHandles) {
        std::vector<SelectionHandleGraphicsItem*>& handlesRef = selHandleBase->_selectionHandles;
        handlesRef.clear();

        foreach (auto posEnum, positions) {
            auto selHandleItem = new SelectionHandleGraphicsItem(this, posEnum, selHandleBase, posRectMap[posEnum]);
            // create as "render invisible" to avoid showing the handle
            // if the item is added to the selection before the mouse button is released
            selHandleItem->setRenderVisible(false);
            handlesRef.push_back(selHandleItem);
        }

        //setAllHandlesRenderVisible(true); // set all visible to true because isVisible is a cached value, see doc
    }
    else {
        std::vector<SelectionHandleGraphicsItem*> handles = selHandleBase->_selectionHandles;

        int i = 0; // todo later: make independent from i
        foreach (auto posEnum, positions) {
            handles[i++]->setRect(posRectMap[posEnum]);
        }
    }
}

bool SelectionHandles::isAnyHandleUnderMouse()
{
    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        foreach (auto handleItem, selHandleItem->_selectionHandles) {
            if (handleItem->isUnderMouse()) {
                return true;
            }
        }
    }

    return false;
}

void SelectionHandles::setHandlesVisible(bool visible)
{
    setAllHandlesRenderVisible(visible);
}

void SelectionHandles::setAllSelectedItemsMovable(bool isMoveable)
{
    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        selHandleItem->selHandleBaseInstrumentedItem()->setFlag(QGraphicsItem::ItemIsMovable, isMoveable);
    }
}

void SelectionHandles::setAllHandlesRenderVisible(bool isVisible)
{
    qDebug() << "SelectionHandles::setAllHandlesRenderVisible:" << isVisible;

    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        foreach (auto handleItem, selHandleItem->_selectionHandles) {
            handleItem->setRenderVisible(isVisible);
            handleItem->update(); // otherwise only the current's item handles disappear or the handles do not disappear at all when moving items
        }
    }

    d->allRenderVisible = isVisible;
}

bool SelectionHandles::allHandlesRenderVisible()
{
    return d->allRenderVisible;
}

void SelectionHandles::slotRubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
{
    // show selection handles when rubber band is active
    setAllHandlesRenderVisible(!rubberBandRect.isEmpty());
}

}
}
