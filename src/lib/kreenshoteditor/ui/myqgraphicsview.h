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
#ifndef UI_MYQGRAPHICSVIEW_H
#define UI_MYQGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <memory>
#include "../kreenshoteditor.h"
#include "kreenqgraphicsitemsimpl.h"
#include "toolmanager.h"
#include "../core/itemsmanager.h"

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyQGraphicsView(QGraphicsScene* scene, ToolManagerPtr toolManager /*, ItemsManagerPtr itemsManager*/)
    {
        _scene = scene;
        _toolManager = toolManager;
        _creatingItem = nullptr;
        //_itemsManager = itemsManager;

        setRenderHint(QPainter::Antialiasing);
    }

Q_SIGNALS:
    void mouseReleased();

    /**
     * this can be drawn item like rect or an operation op-crop
     */
    void itemCreated(ItemPtr item);

protected:
    virtual void enterEvent(QEvent* event)
    {
        // for not to have to click once before one can start moving items
        this->setFocus();
    }

    virtual void mousePressEvent(QMouseEvent* event)
    {
        qDebug() << "mousePressEvent";

        _creatingItemStartPoint = event->pos(); // todo: map coo?

        if (!_toolManager->isImageOperationActive) {
            if (_toolManager->chosenTool == DrawRect) {
                ItemPtr item = Item::create("rect");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, _scene);
                _scene->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == DrawEllipse) {
                ItemPtr item = Item::create("ellipse");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, _scene);
                _scene->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == DrawLine) {
                ItemPtr item = Item::create("line");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, _scene);
                _scene->addItem(_creatingItem);
            }
            else if (_toolManager->chosenTool == OperationCrop) {
                ItemPtr item = Item::create("op-crop");
                _creatingItem = _toolManager->createGraphicsItemFromKreenItem(item, _scene);
                _scene->addItem(_creatingItem);
            }
        }

        QGraphicsView::mousePressEvent(event);
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        if (_creatingItem != nullptr) {

            //qDebug() << "mouseMoveEvent";
            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(_creatingItem);
            grItemBase->updateVisualGeometryFromPoints(_creatingItemStartPoint, event->pos());
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent* event)
    {
        if (_creatingItem != nullptr) {
            _scene->removeItem(_creatingItem);

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

        QGraphicsView::mouseReleaseEvent(event);

        qDebug() << "emit mouseReleased";
        emit mouseReleased(); // used to update from model to have instant visual feedback iif something is wrong with model/view mappine
    }

private:
    QGraphicsScene* _scene;
    ToolManagerPtr _toolManager;

    QGraphicsItem* _creatingItem;
    QPoint _creatingItemStartPoint;
};

#endif // MYQGRAPHICSVIEW

// kate: indent-mode cstyle; replace-tabs on;
