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
#include "selectionhandlesmgr.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "selectionhandlegraphicsitem.h"
#include "selectionhandlebase.h"

namespace kreen {
namespace ui {

class SelectionHandlesMgr::Impl
{
public:
    QGraphicsScene* scene = nullptr;
    QGraphicsView* view = nullptr;
    bool allRenderVisible = true;
    bool mouseIsDown = false;

public:
    Impl(SelectionHandlesMgr* owner)
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

    void assertInit()
    {
        if (scene == nullptr || view == nullptr) {
            qDebug() << "setSceneAndView";
            Q_ASSERT(false);
        }
    }

private:
    SelectionHandlesMgr* _owner = nullptr;
};


SelectionHandlesMgr::SelectionHandlesMgr() {
    KREEN_PIMPL_INIT_THIS(SelectionHandlesMgr);
}

SelectionHandlesMgr::~SelectionHandlesMgr()
{
}

void SelectionHandlesMgr::setSceneAndView(QGraphicsScene* scene, QGraphicsView* view)
{
    d->scene = scene;
    d->view = view;

    disconnect(d->scene, 0, this, 0);
    connect(d->scene, SIGNAL(selectionChanged()), this, SLOT(slotSceneSelectionChanged()));

    disconnect(d->view, 0, this, 0); // qt doc "3. Disconnect a specific receiver"
    connect(view, SIGNAL(rubberBandChanged(QRect, QPointF, QPointF)), this, SLOT(slotRubberBandChanged(QRect, QPointF, QPointF)));
}

void SelectionHandlesMgr::registerItem(SelectionHandleBase* instrItem)
{
    instrItem->setSelectionHandlesMgr(shared_from_this());
}

bool SelectionHandlesMgr::onScene_mousePressEvent_Enter(QGraphicsSceneMouseEvent* event)
{
    d->mouseIsDown = true;

    // Set handle visibility to false but only if no keyboard modifier like Ctrl is pressed.
    // Because it looks strange when Ctrl selection more items and the handles disappear while clicking around
    if (event->modifiers() == Qt::NoModifier) {
        setAllHandlesRenderVisible(false);
    }

    // if mouse is over a handle, no new item should be created on mouse press:
    //
    if (isAnyHandleUnderMouse()) {
        qDebug() << "SelectionHandlesMgr::isAnyHandleUnderMouse()";
        return true;
    }

    return false;
}

void SelectionHandlesMgr::onScene_mouseReleaseEvent_Enter()
{
    d->mouseIsDown = false;
    // qDebug() << "...............SelectionHandlesMgr::onScene_mouseReleaseEvent_Enter()";
    setAllHandlesRenderVisible(true);
}

void SelectionHandlesMgr::onItemSelectedHasChanged(kreen::ui::SelectionHandleBase* instrItem)
{
    d->assertInit();

    if (instrItem->selHandleBaseInstrumentedItem()->isSelected()) {
        createOrUpdateHandles(instrItem, true);

        foreach (auto handleItem, instrItem->_selectionHandles) {
            d->scene->addItem(handleItem);
        }
    }
    else {
        d->clearHandlesFromScene(instrItem);
    }
}

void SelectionHandlesMgr::onItemSceneHasChanged(kreen::ui::SelectionHandleBase* instrItem)
{
    d->assertInit();

    if (!instrItem->selHandleBaseInstrumentedItem()->scene()) { // check whether item is part of a scene or not
        d->clearHandlesFromScene(instrItem); // remove handles if instrumentedItem was removed from scene
    }
}

void SelectionHandlesMgr::onItemPositionHasChanged(kreen::ui::SelectionHandleBase* instrItem)
{
    d->assertInit();

    if (instrItem->_selectionHandles.size() > 0) { // update if handles are present
        createOrUpdateHandles(instrItem, false);
    }
}

void SelectionHandlesMgr::createOrUpdateHandles(SelectionHandleBase* selHandleBase, bool createNewHandles)
{
    // qDebug() << "...........SelectionHandlesMgr::createOrUpdateHandles, createNewHandles = " << createNewHandles;

    d->assertInit();

    qreal hw = 8.0; // handleWidth;

    auto grItem = selHandleBase->selHandleBaseInstrumentedItem();

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
            if (d->mouseIsDown) {
                // create as "render invisible" to avoid showing the handle
                // if the item is added to the selection before the mouse button is released
                selHandleItem->setRenderVisible(false);
            }
            handlesRef.push_back(selHandleItem);
        }
    }
    else {
        std::vector<SelectionHandleGraphicsItem*> handles = selHandleBase->_selectionHandles;

        int i = 0; // todo later: make independent from i
        foreach (auto posEnum, positions) {
            handles[i++]->setRect(posRectMap[posEnum]);
        }
    }
}

bool SelectionHandlesMgr::isAnyHandleUnderMouse()
{
    d->assertInit();

    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        foreach (auto handleItem, selHandleItem->_selectionHandles) {
            if (handleItem->isUnderMouse()) {
                return true;
            }
        }
    }

    return false;
}

void SelectionHandlesMgr::setAllSelectedItemsMovable(bool isMoveable)
{
    d->assertInit();

    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        selHandleItem->selHandleBaseInstrumentedItem()->setFlag(QGraphicsItem::ItemIsMovable, isMoveable);
    }
}

void SelectionHandlesMgr::setAllHandlesRenderVisible(bool isVisible)
{
    d->assertInit();

    // qDebug() << "..SelectionHandlesMgr::setAllHandlesRenderVisible:" << isVisible;

    foreach (auto selHandleItem, d->selectedSelHandleItems()) {
        foreach (auto handleItem, selHandleItem->_selectionHandles) {
            handleItem->setRenderVisible(isVisible);
        }
    }

    d->allRenderVisible = isVisible;
}

void SelectionHandlesMgr::slotSceneSelectionChanged()
{
    //setAllHandlesRenderVisible(true); // not needed
}

void SelectionHandlesMgr::slotRubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
{
    // show selection handles when rubber band is active
    setAllHandlesRenderVisible(!rubberBandRect.isEmpty());
}

}
}
