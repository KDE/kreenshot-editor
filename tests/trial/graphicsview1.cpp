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
#include "graphicsview1.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>

class EllipseItemWithContextMenu : public QGraphicsEllipseItem
{
protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override
    {
        QMenu menu;
        auto a1 = new QAction("Action AAA 1", nullptr);
        auto a2 = new QAction("Action AAA 2", nullptr);
        menu.addAction(a1);
        menu.addAction(a2);
        menu.exec(event->screenPos());
    }
};

class MyScene : public QGraphicsScene
{
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton) {
            // WORKAROUND,
            // see http://www.qtcentre.org/threads/36953-QGraphicsItem-deselected-on-contextMenuEvent
            // and https://bugreports.qt-project.org/browse/QTBUG-10138 "Right click context menu in Graphics View clears rubberband selection."
            // and my comment https://bugreports.qt-project.org/browse/QTBUG-21943
            event->accept();
            return;
        }
        QGraphicsScene::mousePressEvent(event);
    }
};

/**
 *  QApplication app(argc, argv);
 *  auto graphicsView = createTrialGraphicsView();
 *  graphicsView->show();
 *  app.exec();
 */
QGraphicsView* createTrialGraphicsView()
{
    auto graphicsView = new QGraphicsView();
    auto scene = new MyScene();
    graphicsView->setScene(scene);
    {
        auto item1 = new QGraphicsEllipseItem();
        item1->setRect(10, 10, 300, 200);
        item1->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item1->setFlag(QGraphicsItem::ItemIsMovable, true);
        scene->addItem(item1);
    }
    {
        auto item2 = new EllipseItemWithContextMenu();
        item2->setRect(310, 10, 150, 100);
        item2->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item2->setFlag(QGraphicsItem::ItemIsMovable, true);
        scene->addItem(item2);
    }
    return graphicsView;
}
