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

// TODO
class ItemVisual {
public:
    bool mouseOver;
    bool selected;
};

class MainEditorWidgetImpl
{
public:
    KreenshotEditor* kreenshotEditor;

    // QGraphicsView graphicsView;// causes coordinate offsets and is not needed
    QGraphicsScene scene;

    std::map<ItemPtr, bool> mouseOverMap;
    const int mouseOverMargin = 2;

    std::map<ItemPtr, bool> selectedMap;

    // currently only one item can be moved at a time
    Item* itemOnTheMove;
    QPoint itemOnTheMoveInitialMousePosTopLeft;

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
        itemOnTheMove = nullptr;
    }
    /**
     * recreate the scene to reflect the current kreenshotEditor->itemsManager
     */
    void refreshScene()
    {
        scene.clear();

        foreach (ItemPtr item, kreenshotEditor->itemsManager().items()) {

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

            auto mouseOverMapItem = mouseOverMap.find(item);
            bool isMouseOver = mouseOverMapItem != mouseOverMap.end() && mouseOverMapItem->second == true;

            auto selectedMapItem = selectedMap.find(item);
            bool isSelected = selectedMapItem != selectedMap.end() && selectedMapItem->second == true;

            // highlight: // TODO: for "drawRoundRect" move this away from the scene but directly to the painter!
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

            // selected indicator: TODO: draw handles (as extra class?)
            if (isSelected) {
                auto rectItem = new QGraphicsRectItem();
                rectItem->setRect(item->rect().marginsAdded(QMargins(mouseOverMargin, mouseOverMargin, mouseOverMargin, mouseOverMargin)));
                rectItem->setPen(QPen(Qt::darkGray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
                //rectItem->setBrush(QBrush(Qt::Dense7Pattern));
                scene.addItem(rectItem);
            }
        }
    }

    void onMouseLeave()
    {
        mouseOverMap.clear();
    }

    void onMouseMove(QPoint pos)
    {
        mouseOverMap.clear();

        foreach (ItemPtr item, kreenshotEditor->itemsManager().items()) {
            bool isOver = item->rect().contains(pos);
            mouseOverMap[item] = isOver;
            if (isOver) {
                break; // only match the top most item if there are more than one
            }
        }
    }

    void onMouseLeftButtonPress(QPoint pos, bool toggle)
    {
        if (!toggle) {
            selectedMap.clear();
        }

        foreach (ItemPtr item, kreenshotEditor->itemsManager().items()) {
            bool hit = item->rect().contains(pos);
            if (!toggle) {
                selectedMap[item] = hit;

                if (hit) {
                    itemOnTheMove = item.get();
                    itemOnTheMoveInitialMousePosTopLeft = pos - itemOnTheMove->rect().topLeft();
                    break; // select the top-most one
                }
            }
            else {
                if (hit) {
                    selectedMap[item] = !selectedMap[item];
                }
            }
        }
    }

    void onMouseMoveNoCtrlKey(QPoint pos) {
        if (itemOnTheMove != nullptr) {
            if (itemOnTheMove->line().isNull()) { // move a rectangle item
                QRect rect = itemOnTheMove->rect();
                rect.moveTopLeft(pos - itemOnTheMoveInitialMousePosTopLeft);
                itemOnTheMove->setRect(rect);
            }
            else { // move a line item
                QLine line = itemOnTheMove->line();
                QPoint p1 = line.p1();
                QPoint p2 = line.p2();
                QPoint diff = p2 - p1;
                p1 = pos - itemOnTheMoveInitialMousePosTopLeft;
                p2 = p1 + diff;
                line.setPoints(p1, p2);
                itemOnTheMove->setLine(line);
            }
        }
    }

    void onMouseRelease(QPoint pos) {
        onMouseMoveNoCtrlKey(pos);
        itemOnTheMove = nullptr;
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
    d = MainEditorWidgetImplPtr(new MainEditorWidgetImpl());
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

    d->onMouseMove(event->pos());
    if (!(event->modifiers() & Qt::ControlModifier)) {
        d->onMouseMoveNoCtrlKey(event->pos());
    }
    update(); // schedules repaint

    QWidget::mouseMoveEvent(event);
}

void MainEditorWidget::leaveEvent(QEvent* event)
{
    d->onMouseLeave();
    update(); // schedules repaint

    QWidget::leaveEvent(event);
}

void MainEditorWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        d->onMouseLeftButtonPress(event->pos(), event->modifiers() & Qt::ControlModifier);
        update(); // schedules repaint
    }

    QWidget::mousePressEvent(event);
}

void MainEditorWidget::mouseReleaseEvent(QMouseEvent* event)
{
    d->onMouseRelease(event->pos());
    update(); // schedules repaint

    QWidget::mouseReleaseEvent(event);
}
