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
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "kreengraphicsscene.h" // todo: remove this dependency if possible
#include "kreengraphicsitembase.h" // todo: remove this dependency if possible
#include "selectionhandlegraphicsitem.h"

namespace kreen {
namespace ui {

class SelectionHandles::Impl
{
public:
    KreenGraphicsScenePtr scene;
    std::vector<QCursor> cursors;
    bool allRenderVisible = true;

public:
    Impl(SelectionHandles* owner)
    {
        _owner = owner;

        //
        // 1   2   3
        // 4   5   6
        // 7   8   9
        //
        cursors.push_back(Qt::ArrowCursor); // not used
        cursors.push_back(Qt::SizeFDiagCursor); // 1
        cursors.push_back(Qt::SizeVerCursor); // 2
        cursors.push_back(Qt::SizeBDiagCursor); // 3
        cursors.push_back(Qt::SizeHorCursor); // 4
        cursors.push_back(Qt::OpenHandCursor); // 5
        cursors.push_back(Qt::SizeHorCursor); // 6
        cursors.push_back(Qt::SizeBDiagCursor); // 7
        cursors.push_back(Qt::SizeVerCursor); // 8
        cursors.push_back(Qt::SizeFDiagCursor); // 9
    }

    /**
     * creates a new SelectionHandleGraphicsItem including cursor and move event handling
     *
     * TODO: let the SelectionHandleGraphicsItem handle the cursors
     *        according to the PositionType (TopLeft, TopBorder, ... see Handle from tikzkit
     */
    SelectionHandleGraphicsItem* createSelectionHandleItem(KreenGraphicsItemBase* instrumentedItem, QRectF rect, const QCursor& cursor)
    {
        auto handleItem = new SelectionHandleGraphicsItem(_owner, instrumentedItem, rect);
        handleItem->setCursor(cursor);
        return handleItem;
    }

    void clearHandlesFromScene(KreenGraphicsItemBase* kGrItem)
    {
        foreach (auto handleItem, kGrItem->_selectionHandles) {
            scene->removeItem(handleItem);
        }
        kGrItem->_selectionHandles.clear();
    }

private:
    SelectionHandles* _owner = nullptr;
};


SelectionHandles::SelectionHandles(KreenGraphicsScenePtr scene) {
    qDebug() << "SelectionHandles::ctor";
    KREEN_PIMPL_INIT_THIS(SelectionHandles);
    d->scene = scene;
}

SelectionHandles::~SelectionHandles()
{
    qDebug() << "SelectionHandles destructor";
}

void SelectionHandles::onItemSelectedHasChanged(KreenGraphicsItemBase* kGrItem)
{
    if (kGrItem->graphicsItem()->isSelected()) {
        createOrUpdateHandles(kGrItem, true);

        foreach (auto handleItem, kGrItem->_selectionHandles) {
            d->scene->addItem(handleItem);
        }
    }
    else {
        d->clearHandlesFromScene(kGrItem);
    }
}

void SelectionHandles::onItemSceneHasChanged(KreenGraphicsItemBase* kGrItem)
{
    if (!kGrItem->graphicsItem()->scene()) {
        d->clearHandlesFromScene(kGrItem); // remove handles if instrumentedItem was removed from scene
    }
}

void SelectionHandles::onItemPositionHasChanged(KreenGraphicsItemBase* kGrItem)
{
    if (kGrItem->_selectionHandles.size() > 0) { // update if handles are present
        createOrUpdateHandles(kGrItem, false);
    }
}

void SelectionHandles::createOrUpdateHandles(KreenGraphicsItemBase* kGrItem, bool createNewHandles)
{
    qreal hw = 8.0; // handleWidth;

    // WORKAROUND:
    // handle width, TODO: why? even or uneven numbers: these or those items will have blurred rects
    //                     the underlying item is also blurred
    //                     WTF-->the black selection rects get also blurred

    if (kGrItem->workaroundIsBlurredOnUnevenHandleWidth()) {
        qDebug() << "INFO: workaround used";
        hw--;
    }

    auto grItem = kGrItem->graphicsItem();

    qreal hw2 = hw / 2.0;

    auto baseRect = grItem->sceneBoundingRect();
    qreal x = baseRect.x();
    qreal y = baseRect.y();
    qreal w = baseRect.width();
    qreal h = baseRect.height();
    qreal w2 = floor(w / 2.0);
    qreal h2 = floor(h / 2.0);
    auto rect = QRectF(0, 0, hw, hw);

    //
    // 1   2   3
    // 4   5   6
    // 7   8   9
    //
    auto r1 = rect.translated(x - hw2, y - hw2); // 1
    auto r2 = rect.translated(x + w2 - hw2, y - hw2); // 2
    auto r3 = rect.translated(x + w - hw2, y - hw2); // 3

    auto r4 = rect.translated(x - hw2, y + h2 - hw2); // 4
    auto r6 = rect.translated(x + w - hw2, y + h2 - hw2); // 6

    auto r7 = rect.translated(x - hw2, y + h - hw2); // 7
    auto r8 = rect.translated(x + w2 - hw2, y + h - hw2); // 8
    auto r9 = rect.translated(x + w - hw2, y + h - hw2); // 9


    if (createNewHandles) {
        std::vector<SelectionHandleGraphicsItem*>& handlesRef = kGrItem->_selectionHandles;
        handlesRef.clear();

        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r1, d->cursors[1]));
        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r2, d->cursors[2]));
        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r3, d->cursors[3]));

        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r4, d->cursors[4]));
        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r6, d->cursors[6]));

        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r7, d->cursors[7]));
        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r8, d->cursors[8]));
        handlesRef.push_back(d->createSelectionHandleItem(kGrItem, r9, d->cursors[9]));

        setAllHandlesRenderVisible(true); // set all visible to true because isVisible is a cached value, see doc
    }
    else {
        std::vector<SelectionHandleGraphicsItem*> handles = kGrItem->_selectionHandles;

        int i = 0; // todo later: handle also less handles (e.g. for lines)
        SelectionHandleGraphicsItem* handle = nullptr;
        // exclude kGrItem->_activeHandle to avoid the effect that that handle is additionally moved
        // TODO: this concept has to redone to also support resizing objects over the flipping point
        //
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r1); }
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r2); }
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r3); }
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r4); } // sic!
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r6); } // sic!
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r7); }
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r8); }
        handle = handles[i++]; if (handle != kGrItem->_activeHandle) { handle->setRect(r9); }
    }
}

bool SelectionHandles::isAnyHandleUnderMouse()
{
    foreach (auto grItem, d->scene->selectedKreenGraphicsItems()) {
        foreach (auto handleItem, grItem->_selectionHandles) {
            if (handleItem->isUnderMouse()) {
                return true;
            }
        }
    }

    return false;
}

void SelectionHandles::setAllSelectedItemsMovable(bool isMoveable)
{
    foreach (auto kGrItem, d->scene->selectedKreenGraphicsItems()) {
        kGrItem->graphicsItem()->setFlag(QGraphicsItem::ItemIsMovable, isMoveable);
    }
}

void SelectionHandles::setAllHandlesRenderVisible(bool isVisible)
{
    qDebug() << "SelectionHandles::setAllHandlesRenderVisible:" << isVisible;

    foreach (auto grItem, d->scene->selectedKreenGraphicsItems()) {
        foreach (auto handleItem, grItem->_selectionHandles) {
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

}
}
