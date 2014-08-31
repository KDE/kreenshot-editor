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
    bool allRenderVisible = true;

public:
    Impl(SelectionHandles* owner)
    {
        _owner = owner;
    }

    /**
    * 1   7   2
    * 5   0   6
    * 3   8   4
    */
    QCursor cursorFromPositionEnum(PositionEnum posEnum)
    {
        switch (posEnum)
        {
            case Position0_Center: return Qt::ArrowCursor; // Qt::OpenHandCursor
            case Position1_TopLeft: return Qt::SizeFDiagCursor;
            case Position2_TopRight: return Qt::SizeBDiagCursor;
            case Position3_BottomLeft: return Qt::SizeBDiagCursor;
            case Position4_BottomRight: return Qt::SizeFDiagCursor;
            case Position5_Left: return Qt::SizeHorCursor;
            case Position6_Right: return Qt::SizeHorCursor;
            case Position7_Top: return Qt::SizeVerCursor;
            case Position8_Bottom: return Qt::SizeVerCursor;
        }

        qDebug() << "[ERROR] cursorFromPositionEnum. All cases must be handled.";
        Q_ASSERT(false);
        return Qt::ArrowCursor;
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

    /**
    * 1   7   2
    * 5   0   6
    * 3   8   4
    */
    std::vector<PositionEnum> positions;
    positions.push_back(Position1_TopLeft);
    positions.push_back(Position2_TopRight);
    positions.push_back(Position3_BottomLeft);
    positions.push_back(Position4_BottomRight);
    positions.push_back(Position5_Left);
    positions.push_back(Position6_Right);
    positions.push_back(Position7_Top);
    positions.push_back(Position8_Bottom);

    QMap<PositionEnum, QRectF> posRectMap;
    // posRectMap.insert(Position0Center)
    posRectMap.insert(Position1_TopLeft, rect.translated(x - hw2, y - hw2));
    posRectMap.insert(Position2_TopRight, rect.translated(x + w - hw2, y - hw2));
    posRectMap.insert(Position3_BottomLeft, rect.translated(x - hw2, y + h - hw2));
    posRectMap.insert(Position4_BottomRight, rect.translated(x + w - hw2, y + h - hw2));
    posRectMap.insert(Position5_Left, rect.translated(x - hw2, y + h2 - hw2));
    posRectMap.insert(Position6_Right, rect.translated(x + w - hw2, y + h2 - hw2));
    posRectMap.insert(Position7_Top, rect.translated(x + w2 - hw2, y - hw2));
    posRectMap.insert(Position8_Bottom, rect.translated(x + w2 - hw2, y + h - hw2));

    if (createNewHandles) {
        std::vector<SelectionHandleGraphicsItem*>& handlesRef = kGrItem->_selectionHandles;
        handlesRef.clear();

        foreach (auto posEnum, positions) {
            // TODO: give the handle also its posEnum
            handlesRef.push_back(d->createSelectionHandleItem(kGrItem, posRectMap[posEnum], d->cursorFromPositionEnum(posEnum)));
        }

        setAllHandlesRenderVisible(true); // set all visible to true because isVisible is a cached value, see doc
    }
    else {
        std::vector<SelectionHandleGraphicsItem*> handles = kGrItem->_selectionHandles;

        int i = 0; // TODO later: handle also less handles (e.g. for lines)
        foreach (auto posEnum, positions) {
            handles[i++]->setRect(posRectMap[posEnum]); // TODO: make independent from i
        }
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
