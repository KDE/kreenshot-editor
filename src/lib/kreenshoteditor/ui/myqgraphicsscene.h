/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codeminister@publicstatic.de>
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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <memory>
#include "../kreenshoteditor.h"
#include "kreengraphicsitems.h"
#include "toolmanager.h"
#include "../core/itemsmanager.h"

/**
 * TODO: move all logic from MyQGraphicsView to here to potentially solve the wrong-drawing-when-scrolling-bug
 * and make other things easier
 */
class MyQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyQGraphicsScene(ToolManagerPtr toolManager)
    {
        _toolManager = toolManager;
        _creatingItem = nullptr;
    }

Q_SIGNALS:
    void mouseReleased();

    /**
     * this can be drawn item like rect or an operation op-crop
     */
    void itemCreated(ItemPtr item);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        qDebug() << "MyQGraphicsScene::mousePressEvent";

        _creatingItemStartPoint = event->scenePos().toPoint();

        if (!_toolManager->isImageOperationActive) {
            if (_toolManager->chosenTool == DrawRect) {
                ItemPtr item = Item::create("rect");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
                this->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == DrawEllipse) {
                ItemPtr item = Item::create("ellipse");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
                this->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == DrawLine) {
                ItemPtr item = Item::create("line");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
                this->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == OperationCrop) {
                ItemPtr item = Item::create("op-crop");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, this);
                this->addItem(_creatingItem);
            }
        }

        QGraphicsScene::mousePressEvent(event);
    }

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event)
    {
        if (_creatingItem != nullptr) {

            //qDebug() << "mouseMoveEvent";
            QPoint scenePos = event->scenePos().toPoint();
            //qDebug() << scenePos;
            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(_creatingItem);
            grItemBase->updateVisualGeometryFromPoints(_creatingItemStartPoint, scenePos );
        }

        QGraphicsScene::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
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
                return;
            }
            else {
                qDebug() << "rect is null, discard";
            }
        }

        QGraphicsScene::mouseReleaseEvent(event);

        qDebug() << "emit MyQGraphicsScene::mouseReleased";
        emit mouseReleased(); // used to update from model to have instant visual feedback iif something is wrong with model/view mappine
    }

private:
    ToolManagerPtr _toolManager;

    QGraphicsItem* _creatingItem;
    QPoint _creatingItemStartPoint;
};

typedef std::shared_ptr<MyQGraphicsScene> MyQGraphicsScenePtr;

#endif // UI_MYQGRAPHICSSCENE_H

// kate: indent-mode cstyle; replace-tabs on;
