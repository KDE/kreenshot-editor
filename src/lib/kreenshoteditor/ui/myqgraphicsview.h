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
#include <QDebug>
#include <QGraphicsScene>
#include <memory>
#include "../kreenshoteditor.h"
#include "kreenqgraphicsitemsimpl.h"

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyQGraphicsView(QGraphicsScene* scene)
    {
        _scene = scene;
        _creatingItem = nullptr;
    }

Q_SIGNALS:
    void mouseReleased();

protected:
    virtual void enterEvent(QEvent* event)
    {
        // for not to have to click once before one can start moving items
        this->setFocus();
    }

    virtual void mousePressEvent(QMouseEvent* event)
    {
        qDebug() << "mousePressEvent";

        ItemPtr item = Item::create("rect");
        _creatingItem = new KreenQGraphicsRectItem(item, _scene);
        _creatingItem->setRect(50, 50, 100, 100);
        _scene->addItem(_creatingItem);

        QGraphicsView::mousePressEvent(event);
    }

    virtual void mouseMoveEvent(QMouseEvent* event)
    {
        qDebug() << "mouseMoveEvent";
        if (_creatingItem != nullptr) {
            QRectF rect = _creatingItem->rect();
            rect.setRight(rect.right() + 2);
            _creatingItem->setRect(rect);
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    virtual void mouseReleaseEvent(QMouseEvent* event)
    {
        if (_creatingItem != nullptr) {
            _scene->removeItem(_creatingItem);
        }

        QGraphicsView::mouseReleaseEvent(event);

        qDebug() << "mouseReleaseEvent: update from model";
        emit mouseReleased();
    }

private:
    QGraphicsScene* _scene;
    KreenQGraphicsRectItem* _creatingItem; // TMP
};

#endif // MYQGRAPHICSVIEW

// kate: indent-mode cstyle; replace-tabs on;
