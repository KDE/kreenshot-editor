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
#include "kreengraphicsscene.h"
#include <kreen/core/document.h>
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include "toolmanager.h"
#include "kreengraphicsitems.h"
#include "../selectionhandles/selectionhandlegraphicsitem.h"
#include "../selectionhandles/selectionhandlesmgr.h"
#include "rendervisibilitycontrol.h"

namespace kreen {
namespace ui {

KreenGraphicsScene::KreenGraphicsScene()
{
    _creatingItem = nullptr;
    _renderVisibilityControl = RenderVisibilityControl::make_shared();
}

void KreenGraphicsScene::setDocument(DocumentPtr document)
{
    _document = document;
}

DocumentPtr KreenGraphicsScene::document()
{
    return _document;
}

void KreenGraphicsScene::setToolManager(ToolManagerPtr toolManager)
{
    _toolManager = toolManager;
}

void KreenGraphicsScene::setSelectionHandlesMgr(kreen::ui::SelectionHandlesMgrPtr selectionHandlesMgr)
{
    _selectionHandlesMgr = selectionHandlesMgr;
}

QList<KreenGraphicsItemBase*> KreenGraphicsScene::selectedKreenGraphicsItems()
{
    QList<KreenGraphicsItemBase*> result;

    foreach (auto grItem, selectedItems()) {

        auto kGrItem = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        if (kGrItem != nullptr) {
            result.append(kGrItem);
        }
        else {
            Q_ASSERT(false); // todo?
        }
    }

    return result;
}

QList<KreenItemPtr> KreenGraphicsScene::selectedKreenItems()
{
    QList<KreenItemPtr> result;

    foreach (auto kGrItem, selectedKreenGraphicsItems()) {
        //qDebug() << (long)kGrItem << kGrItem->item()->typeId;
        result.append(kGrItem->item());
    }

    return result;
}

void KreenGraphicsScene::saveCurrentKreenItemsSelection()
{
    _savedSelection = selectedKreenItems();
    qDebug() << "KreenGraphicsScene::saveCurrentKreenItemsSelection(), count=" << _savedSelection.count();
}

void KreenGraphicsScene::restoreSavedKreenItemsSelection_1()
{
    qDebug() << "KreenGraphicsScene::restoreSavedKreenItemsSelection_1, count=" << _savedSelection.count();

    int count = 0;
    foreach (auto kreenItem, _savedSelection) {
        auto kGrItem = graphicsItemBaseFromItem(kreenItem);
        if (kGrItem != nullptr) { // item might be deleted already (e. g. after Undo)
            //qDebug() << "   item id=" << kGrItem->item()->id();

            // for case: "An items was just created and then resized using a handle"
            // ... createSceneFromModel() will be called
            // ... updateItemsSelectableAndMovableFromChosenTool() will be called
            // then: because we have still Draw item tool active the updateItemsSelectableAndMovableFromChosenTool() makes
            //       the items NOT selectable
            // Thus, we have to make them selectable in order to make the setSelected(true) work.
            kGrItem->setSelectable(true);

            kGrItem->graphicsItem()->setSelected(true);
            count++;
        }
    }

    //qDebug() << "   restored: " << count;
}

void KreenGraphicsScene::renderFinalImageOnly(bool finalOnly)
{
    foreach (QGraphicsItem* grItem, items()) {

        auto kreenGrItem = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        if (kreenGrItem && kreenGrItem->item()->isImageOperation()) { // hide image operations like crop or rip out
            grItem->setVisible(!finalOnly);
        }

        auto selectionHandleGrItem = dynamic_cast<SelectionHandleGraphicsItem*>(grItem);
        if (selectionHandleGrItem) { // hide SelectionHandleGraphicsItem
            grItem->setVisible(!finalOnly);
        }
    }
}

void KreenGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_ASSERT(_toolManager != nullptr);

    if (event->button() != Qt::LeftButton) {
        // WORKAROUND,
        // see http://www.qtcentre.org/threads/36953-QGraphicsItem-deselected-on-contextMenuEvent
        // and https://bugreports.qt-project.org/browse/QTBUG-10138 "Right click context menu in Graphics View clears rubberband selection."
        // and my comment https://bugreports.qt-project.org/browse/QTBUG-21943
        event->accept();
        //QGraphicsScene::mousePressEvent(event); // this MUST not be called in this case
        return;
        // see also where WORKAROUND_sendFakeMouseEvent is used to fix cursor bugs
    }

    qDebug() << "MyQGraphicsScene::mousePressEvent with left button";

    if (_selectionHandlesMgr->onScene_mousePressEvent_Enter(event)) {
        QGraphicsScene::mousePressEvent(event); // the handle items should receive the mousePressEvent
        return;
    }

    _creatingItemStartPoint = event->scenePos().toPoint();

    if (!_toolManager->isImageOperationActive()) {
        ToolEnum tool = _toolManager->chosenTool();

        KreenItemPtr item;

        if (tool == Select) {
            _creatingItem = nullptr; // not item to be created
        }
        else if (tool == DrawRect) {
            item = KreenItem::create("rect");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item)->graphicsItem();
        }
        else if (tool == DrawEllipse) {
            item = KreenItem::create("ellipse");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item)->graphicsItem();
        }
        else if (tool == DrawLine) {
            item = KreenItem::create("line");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item)->graphicsItem();
        }
//         else if (tool == DrawText) {
//             item = KreenItem::create("text");
//             _creatingItem = _toolManager->createGraphicsItemFromKreenItem...(item);
//         }
        else if (tool == DrawObfuscate) {
            item = KreenItem::create("obfuscate");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item)->graphicsItem();
        }
        else if (tool == OperationCrop) {
            item = KreenItem::create("op-crop");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item)->graphicsItem();
        }
        else {
            qDebug() << "Unknown tool: " << tool;
            Q_ASSERT(false);
        }

        if (_creatingItem) {

            // corner case:
            // 1. create one new itemCreated
            // 2. create another new item and begin dragging within the first one
            // The following loop will prevent the first one from staying selected
            foreach(auto kGrItem, kreenGraphicsItems()) {
                kGrItem->setSelectableAndMovable(false);
            }

            // (interesting: if the internals of SelectionHandlesMgr would not use
            //  std::enable_shared_from_this than this will cause a crash,
            //  this comment can be remove later)
            this->addItem(_creatingItem); // add the new item to scene
        }

        if (item && isItemForPointToSceneRestriction(item)) {
            restrictPointToScene(&_creatingItemStartPoint);
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void KreenGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (_creatingItem != nullptr) {

        //qDebug() << "mouseMoveEvent";
        QPoint scenePos = event->scenePos().toPoint();

        //qDebug() << scenePos;
        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(_creatingItem);

        if (isItemForPointToSceneRestriction(grItemBase->item())) {
            restrictPointToScene(&scenePos);
        }

        grItemBase->updateVisualGeometryFromPoints(_creatingItemStartPoint, scenePos);
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void KreenGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsScene::mouseReleaseEvent(event);
        return;
    }

    qDebug() << "MyQGraphicsScene::mouseReleaseEvent with left button";

    _selectionHandlesMgr->onScene_mouseReleaseEvent_Enter();

    if (_creatingItem != nullptr) {
        // remove this temporary item from the scene because we will add it to the document
        this->removeItem(_creatingItem);

        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(_creatingItem);
        grItemBase->updateModelFromVisualGeometry();

        _creatingItem = nullptr;
        if (!grItemBase->item()->rect().isNull()) {
            qDebug() << "emit itemCreated";
            emit itemCreatedSignal(grItemBase->item());
            qDebug() << "QGraphicsScene::mouseReleaseEvent(event) call";
            QGraphicsScene::mouseReleaseEvent(event);
            return;
        }
        else {
            qDebug() << "rect is null, discard";
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);

    qDebug() << "emit MyQGraphicsScene::mouseReleasedSignal";
    emit mouseReleasedSignal(); // used to update from model to have instant visual feedback if something is wrong with model/view mappine
    update(); // redraw complete scene (still needed for selection handles?)
}

void KreenGraphicsScene::slotRequestRenderToImage(kreen::core::Document* document)
{
    qDebug() << "KreenGraphicsScene::slotRequestRenderToImage";
    QImage image = document->baseImage().copy();
    Q_ASSERT_X(!image.isNull(), "renderToImage", "image must not be empty otherwise creation of the painter will fail");
    QPainter painterImage(&image);
    painterImage.setRenderHint(QPainter::Antialiasing);
    renderFinalImageOnly(true);
    render(&painterImage);
    renderFinalImageOnly(false);
    document->onRenderToImageComplete(image);
}

bool KreenGraphicsScene::isItemForPointToSceneRestriction(KreenItemPtr item)
{
    return item->typeId == "op-crop"; // TODO: op-ripout
}

void KreenGraphicsScene::restrictPointToScene(QPoint* pt)
{
    if (pt->x() < 0) {
        pt->setX(0);
    }

    if (pt->y() < 0) {
        pt->setY(0);
    }

    if (pt->x() > this->width()) {
        pt->setX(this->width());
    }

    if (pt->y() > this->height()) {
        pt->setY(this->height());
    }
}

QList<KreenGraphicsItemBase*> KreenGraphicsScene::kreenGraphicsItems()
{
    QList<KreenGraphicsItemBase*> list;

    foreach(auto grItem, this->items()) {
        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        if (grItemBase != nullptr) { // there might also be other items
            list << grItemBase;
        }
    }

    return list;
}

KreenGraphicsItemBase* KreenGraphicsScene::graphicsItemBaseFromItem(KreenItemPtr item)
{
    foreach(auto kreenGraphicsItemBase, kreenGraphicsItems()) {
        if (kreenGraphicsItemBase->item()->id() == item->id()) { // compare by id because Document holds secret copies.
            return kreenGraphicsItemBase;
        }
    }

    return nullptr;
}

void KreenGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    qDebug() << "KreenGraphicsScene::dragEnterEvent";
    event->accept();
}

void KreenGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    // this is needed (see http://stackoverflow.com/questions/4177720/accepting-drops-on-a-qgraphicsscene)
    event->accept();
}

void KreenGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    qDebug() << "KreenGraphicsScene::dropEvent";
    // TODO
    QMessageBox::warning(nullptr,
                         tr("Not impl"),
                         tr("Drop image on scene to create an image object not implemented yet.\nDrop the image anywhere else on the window to load it as new base image."));
}

void KreenGraphicsScene::drawBackground(QPainter* painter, const QRectF& rect)
{
//     //qDebug() << "KreenGraphicsScene::drawBackground";
//     _renderVisibilityControl->initPainting(); // todo remove later
    QGraphicsScene::drawBackground(painter, rect);
}

void KreenGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent)
{
    qDebug() << "KreenGraphicsScene::contextMenuEvent";

    QGraphicsScene::contextMenuEvent(contextMenuEvent);

    if (!contextMenuEvent->isAccepted()) {
        contextMenuEvent->accept();
        QMenu menu;
        menu.addAction(new QAction("Action 1", this));
        menu.addAction(new QAction("Action 2", this));
        menu.addAction(new QAction("Action 3", this));
        menu.exec(contextMenuEvent->screenPos());
    }
}

}
}

