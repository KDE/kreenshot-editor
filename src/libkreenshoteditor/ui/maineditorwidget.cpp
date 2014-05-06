/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mitsch <codeminister@publicstatic.de>
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
#include "maineditorwidget.h"
#include "core/itemsmanager.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

class MainEditorWidgetImpl
{
public:
    KreenshotEditor* kreenshotEditor;

    QGraphicsView graphicsView;
    QGraphicsScene scene;

    //std::map<

public:
    // todo: optimize?
    QRect getBaseRect() {
        const QImage& baseImage = kreenshotEditor->getBaseImage();
        QRect rect(0, 0, baseImage.width(), baseImage.height());
        qDebug() << rect;
        return rect;
    }

    void initScene() {
        QRect rect = getBaseRect();
        scene.setSceneRect(rect);
        graphicsView.setScene(&scene);
    }
    /**
     * recreate the scene to reflect the current kreenshotEditor->itemsManager
     *
     * TODO: 1. done
     *       2. with this as base implement basic mouse hover highlighting!
     */
    void refreshScene()
    {
        scene.clear();

        foreach (std::shared_ptr<Item> item, kreenshotEditor->itemsManager().items()) {

            if (item->typeId == "rect") {
                auto dropShadow = new QGraphicsDropShadowEffect();
                dropShadow->setColor(Qt::black);
                dropShadow->setOffset(QPoint(3, 3));
                dropShadow->setBlurRadius(10);

                auto rectItem = new QGraphicsRectItem();
                rectItem->setRect(item->rect());
                rectItem->setGraphicsEffect(dropShadow);
                rectItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                scene.addItem(rectItem);
            }
            else if (item->typeId == "ellipse") {
                auto ellipseItem = new QGraphicsEllipseItem();
                ellipseItem->setRect(item->rect());
                ellipseItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                scene.addItem(ellipseItem);
            }
            else if (item->typeId == "text") {
                auto dropShadow = new QGraphicsDropShadowEffect();
                dropShadow->setColor(Qt::black);
                dropShadow->setOffset(QPoint(2, 2));
                dropShadow->setBlurRadius(5);

                auto textItem = new QGraphicsTextItem("With drop shadow");
                textItem->setPos(item->rect().x(), item->rect().y());
                textItem->setGraphicsEffect(dropShadow);
                scene.addItem(textItem);
            }
        }
    }

    void createDemoScene()
    {
        QRect rect = getBaseRect();
        scene.setSceneRect(rect);

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(3, 3));
            dropShadow->setBlurRadius(10);

            auto rectItem = new QGraphicsRectItem();
            rectItem->setRect(120, 100, 150, 100);
            rectItem->setGraphicsEffect(dropShadow);
            rectItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene.addItem(rectItem);
        }

        {
            auto rect2Item = new QGraphicsRectItem();
            rect2Item->setRect(10, 5, 30, 40);
            //rect2Item->setGraphicsEffect(dropShadow);
            //rect2Item->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene.addItem(rect2Item);
        }

        scene.addText("Hello, world!");

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(2, 2));
            dropShadow->setBlurRadius(5);

            auto textItem = new QGraphicsTextItem("With drop shadow");
            textItem->setPos(30, 60);
            textItem->setGraphicsEffect(dropShadow);
            scene.addItem(textItem);
        }

        {
            auto textItem = new QGraphicsTextItem("TODO: With white glow background");
            textItem->setPos(30, 80);
            scene.addItem(textItem);
        }

        graphicsView.setScene(&scene);
    }
};

MainEditorWidget::MainEditorWidget(KreenshotEditor* kreenshotEditor)
{
    d = new MainEditorWidgetImpl();
    d->kreenshotEditor = kreenshotEditor;

    // use this if not using QScrollArea:
    // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // setMinimumSize(50, 50);

    // for QScrollArea:
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(d->kreenshotEditor->getBaseImage().size());

    //d->createDemoScene();
    d->kreenshotEditor->itemsManager().addDemoItems();
    d->initScene();
}

MainEditorWidget::~MainEditorWidget()
{
    delete d;
}

void MainEditorWidget::paintEvent(QPaintEvent*)
{
    d->refreshScene();

    QPainter painter(this);

    const QImage& baseImage = d->kreenshotEditor->getBaseImage();
    painter.drawImage(QPoint(0, 0), baseImage);

//     painter.drawRect(10, 20, 30, 40);
//
//     painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin)); // TODO: set antialias
//     painter.drawRoundRect(100, 200, 100, 200, 10, 10);

    d->graphicsView.render(&painter);
}
