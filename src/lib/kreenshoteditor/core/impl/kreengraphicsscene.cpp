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
#include "toolmanager.h"
#include "kreengraphicsitems.h"

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

void KreenGraphicsScene::renderFinalImageOnly(bool finalOnly)
{
    foreach (QGraphicsItem* grItem, items()) {

        auto kreenGrItem = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
        if (kreenGrItem && kreenGrItem->item()->typeId.startsWith("op-")) { // hide image operations like crop or rip out
            grItem->setVisible(!finalOnly);
        }

        // TODO: also hide the selection rect
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
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
            this->addItem(_creatingItem);
        }
        else if (tool == DrawEllipse) {
            item = KreenItem::create("ellipse");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
            this->addItem(_creatingItem);
        }
        else if (tool == DrawLine) {
            item = KreenItem::create("line");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
            this->addItem(_creatingItem);
        }
        else if (tool == OperationCrop) {
            item = KreenItem::create("op-crop");
            _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
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
        auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(_creatingItem);

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

void KreenGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "MyQGraphicsScene::mouseReleaseEvent";

    if (_creatingItem != nullptr) {
        this->removeItem(_creatingItem);

        auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(_creatingItem);
        grItemBase->updateModelFromVisualGeometry();

        _creatingItem = nullptr;
        if (!grItemBase->item()->rect().isNull()) {
            qDebug() << "emit itemCreated";
            emit itemCreated(grItemBase->item());
            qDebug() << "QGraphicsScene::mouseReleaseEvent(event)";
            QGraphicsScene::mouseReleaseEvent(event);
            return;
        }
        else {
            qDebug() << "rect is null, discard";
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);

    qDebug() << "emit MyQGraphicsScene::mouseReleased";
    emit mouseReleased(); // used to update from model to have instant visual feedback if something is wrong with model/view mappine
}

}
}

