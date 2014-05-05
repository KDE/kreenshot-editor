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
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

class MainEditorWidgetImpl
{
public:
    KreenshotEditor* kreenshotEditor;

    QGraphicsView graphicsView;
    QGraphicsScene scene;

public:
    void createDemoScene()
    {
        auto dropShadow = new QGraphicsDropShadowEffect();
        dropShadow->setColor(Qt::black);
        dropShadow->setOffset(QPoint(3, 3));
        dropShadow->setBlurRadius(10);

        auto rectItem = new QGraphicsRectItem();
        rectItem->setRect(110, 100, 150, 100);
        rectItem->setGraphicsEffect(dropShadow);
        rectItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        scene.addItem(rectItem);
        scene.addText("Hello, world!");

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
    setMinimumSize(kreenshotEditor->getBaseImage().size());

    d->createDemoScene();
}

MainEditorWidget::~MainEditorWidget()
{
    delete d;
}

void MainEditorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    const QImage& baseImage = d->kreenshotEditor->getBaseImage();
    painter.drawImage(QPoint(0, 0), baseImage);

    painter.drawRect(10, 20, 30, 40);

    painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin)); // TODO: set antialias
    painter.drawRoundRect(100, 200, 100, 200, 10, 10);

    d->graphicsView.render(&painter);
}
