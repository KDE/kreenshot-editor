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
#include <QMessageBox>
#include <QDebug>
#include "toolmanager.h"
#include "kreengraphicsitems.h"
#include "selectionhandlegraphicsitem.h"

namespace kreen {
namespace core {

KreenGraphicsScene::KreenGraphicsScene()
{
    _creatingItem = nullptr;
}

void KreenGraphicsScene::setToolManager(ToolManagerPtr toolManager)
{
    _toolManager = toolManager;
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
}

void KreenGraphicsScene::restoreSavedKreenItemsSelection_1()
{
    // TODO: optimize this double loop

    foreach (auto kreenItem, _savedSelection) {
        foreach (auto kGrItem, kreenGraphicsItems()) {
            if (kGrItem->item()->id() == kreenItem->id()) {
                kGrItem->graphicsItem()->setSelected(true);
            }
        }
    }
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
    qDebug() << "MyQGraphicsScene::mousePressEvent";
    Q_ASSERT(_toolManager != nullptr);

    _creatingItemStartPoint = event->scenePos().toPoint();

    if (!_toolManager->isImageOperationActive()) {
        ToolEnum tool = _toolManager->chosenTool();

        KreenItemPtr item;

        if (tool == Select) {
            // do nothing
        }
        else if (tool == DrawRect) {
            item = KreenItem::create("rect");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item);
            this->addItem(_creatingItem);
        }
        else if (tool == DrawEllipse) {
            item = KreenItem::create("ellipse");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item);
            this->addItem(_creatingItem);
        }
        else if (tool == DrawLine) {
            item = KreenItem::create("line");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item);
            this->addItem(_creatingItem);
        }
//         else if (tool == DrawText) {
//             item = KreenItem::create("text");
//             _creatingItem = _toolManager->createGraphicsItemFromKreenItem...(item);
//             this->addItem(_creatingItem);
//         }
        else if (tool == DrawObfuscate) {
            item = KreenItem::create("obfuscate");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item);
            this->addItem(_creatingItem);
        }
        else if (tool == OperationCrop) {
            item = KreenItem::create("op-crop");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item);
            this->addItem(_creatingItem);
        }
        else {
            qDebug() << "Unknown tool: " << tool;
            Q_ASSERT(false);
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

/**
    * all corresponding graphics items for all KreenItems (including op- items like crop or rip out!)
    */
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

KreenGraphicsItemBase* KreenGraphicsScene::graphicsItemFromItem(KreenItemPtr item)
{
    foreach(auto kreenGraphicsItemBase, kreenGraphicsItems()) {
        if (kreenGraphicsItemBase->item()->id() == item->id()) { // compare by id because Document holds secret copies.
            return kreenGraphicsItemBase;
        }
    }
    
    return nullptr;
}

void KreenGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "MyQGraphicsScene::mouseReleaseEvent enter";

    if (_creatingItem != nullptr) {
        // remove this temporary item from the scene because we will add it to the document
        this->removeItem(_creatingItem);

        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(_creatingItem);
        grItemBase->updateModelFromVisualGeometry();

        _creatingItem = nullptr;
        if (!grItemBase->item()->rect().isNull()) {
            qDebug() << "emit itemCreated";
            emit itemCreated(grItemBase->item());
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

}
}

