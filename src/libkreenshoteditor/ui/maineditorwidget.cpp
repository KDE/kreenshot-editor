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
#include <QMouseEvent>
#include <QMargins>
#include <memory>

class MainEditorWidgetImpl
{
public:
    KreenshotEditor* kreenshotEditor;

    // QGraphicsView graphicsView;// causes coordinate offsets and is not needed
    QGraphicsScene scene;

    std::map<std::shared_ptr<Item>, bool> mouseOverMap;
    const int mouseOverMargin = 2;

    std::map<std::shared_ptr<Item>, bool> selectedMap;

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
    }
    /**
     * recreate the scene to reflect the current kreenshotEditor->itemsManager
     */
    void refreshScene()
    {
        scene.clear();

        foreach (std::shared_ptr<Item> item, kreenshotEditor->itemsManager().items()) {

            //QGraphicsItem* grItem = nullptr;

            //qDebug() << "item: " << item->rect();

            // draw
            //
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
                //grItem = rectItem;
            }
            else if (item->typeId == "line") {
                auto grItem = new QGraphicsLineItem();
                grItem->setLine(item->line());
                grItem->setPen(QPen(Qt::red, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                scene.addItem(grItem);
                //grItem = ellipseItem;
            }
            else if (item->typeId == "ellipse") {
                auto ellipseItem = new QGraphicsEllipseItem();
                ellipseItem->setRect(item->rect());
                ellipseItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                scene.addItem(ellipseItem);
                //grItem = ellipseItem;
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
                //grItem = textItem;
            }

            // highlight: // TODO: drawRoundRect => move this away from the scene but directly to the painter!
            auto mouseOverMapItem = mouseOverMap.find(item);
            bool isMouseOver = mouseOverMapItem != mouseOverMap.end() && mouseOverMapItem->second == true;

            // selected indicator: TODO: draw handles
            auto selectedMapItem = selectedMap.find(item);
            bool isSelected = selectedMapItem != selectedMap.end() && selectedMapItem->second == true;


            if (isMouseOver && !isSelected) {
                //qDebug() << "item: " << item->rect() << "mouseOverMapItem";

                // removes any other effect which is not desired:
                //auto effect = new QGraphicsColorizeEffect();
                //effect->setColor(Qt::blue);
                //grItem->setGraphicsEffect(effect);

                auto rectItem = new QGraphicsRectItem();
                rectItem->setRect(item->rect().marginsAdded(QMargins(mouseOverMargin, mouseOverMargin, mouseOverMargin, mouseOverMargin)));
                //rectItem->setPen(QPen(Qt::gray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                rectItem->setPen(QPen(Qt::transparent));
                rectItem->setBrush(QBrush(Qt::Dense7Pattern));
                scene.addItem(rectItem);
            }

            if (isSelected) {
                auto rectItem = new QGraphicsRectItem();
                rectItem->setRect(item->rect().marginsAdded(QMargins(mouseOverMargin, mouseOverMargin, mouseOverMargin, mouseOverMargin)));
                rectItem->setPen(QPen(Qt::darkGray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                //rectItem->setBrush(QBrush(Qt::Dense7Pattern));
                scene.addItem(rectItem);
            }
        }
    }

    void mouseOverMapClear()
    {
        mouseOverMap.clear();
    }

    void mouseOverMapUpdate(QPoint pos)
    {
        mouseOverMap.clear();

        foreach (std::shared_ptr<Item> item, kreenshotEditor->itemsManager().items()) {
            mouseOverMap[item] = item->rect().contains(pos);
        }
    }

    void mouseLeftButtonClick(QPoint pos, bool toggle)
    {
        if (!toggle) {
            selectedMap.clear();
        }

        foreach (std::shared_ptr<Item> item, kreenshotEditor->itemsManager().items()) {
            bool hit = item->rect().contains(pos);
            if (!toggle) {
                selectedMap[item] = hit;
            }
            else {
                if (hit) {
                    selectedMap[item] = !selectedMap[item];
                }
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
    setMinimumSize(d->getBaseRect().size());

    //d->createDemoScene();
    d->kreenshotEditor->itemsManager().addDemoItems();
    d->initScene();

    setMouseTracking(true); // to enable mouseMoveEvent
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

    //painter.drawRect(205, 205, 30, 30);

    //painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin)); // TODO: set antialias
    //painter.drawRoundRect(100, 200, 100, 200, 10, 10);

    d->scene.render(&painter);
}

void MainEditorWidget::mouseMoveEvent(QMouseEvent* event)
{
    //qDebug() << "mouseMoveEvent";
    //qDebug() << event->pos();

    d->mouseOverMapUpdate(event->pos());
    update(); // schedules repaint

    QWidget::mouseMoveEvent(event);
}

void MainEditorWidget::leaveEvent(QEvent* event)
{
    d->mouseOverMapClear();
    update(); // schedules repaint

    QWidget::leaveEvent(event);
}

void MainEditorWidget::mousePressEvent(QMouseEvent* event)
{
    d->mouseLeftButtonClick(event->pos(), event->modifiers() & Qt::ControlModifier);
    update(); // schedules repaint

    QWidget::mousePressEvent(event);
}

