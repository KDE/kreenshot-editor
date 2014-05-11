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
#include "maineditorwidget.h"
#include "core/itemsmanager.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMargins>
#include <QImageReader>
#include <QImageWriter>
#include <memory>
#include "kreenqgraphicsitemsimpl.h"
#include "myqgraphicsview.h"
#include "toolmanager.h"

//class ItemVisual {
//public:
// bool mouseOver; // TODO later
//};

class MainEditorWidgetImpl
{
public:
    KreenshotEditorPtr kreenshotEditor;
    QGraphicsScene scene;
    MyQGraphicsView* graphicsView;
    ToolManagerPtr toolManager;

public:
    MainEditorWidgetImpl()
    {
        toolManager = ToolManagerPtr(new ToolManager());
    }

//     std::map<ItemPtr, bool> mouseOverMap; // TODO later
//     const int mouseOverMargin = 2; // TODO later

public:
    // todo: optimize this method?
    QRect getBaseRect() {
        const QImage& baseImage = kreenshotEditor->getBaseImage();
        QRect rect(0, 0, baseImage.width(), baseImage.height());
        qDebug() << rect;
        return rect;
    }

    void initScene(QGraphicsView* graphicsView) {
        QRect rect = getBaseRect();
        scene.setSceneRect(rect);

        graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        //graphicsView->setBackgroundBrush(QBrush(Qt::BDiagPattern));
        graphicsView->setBackgroundBrush(QBrush(Qt::DiagCrossPattern)); // todo: make nicer

        graphicsView->setSceneRect(0, 0, 10, 10); // this makes sure that the view scrolls to 0, 0
        graphicsView->setScene(&scene);
        graphicsView->setSceneRect(rect); // this makes sure the scroll bars are shown for large images

        // scroll to 0,0 / does all not work:
        //graphicsView->scroll(-100, -100);
        // graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        // graphicsView->ensureVisible(0, 0, 1, 1);

        createSceneFromModel();
    }

    /**
     * recreate the scene to reflect the current kreenshotEditor->itemsManager
     */
    void createSceneFromModel()
    {
        scene.clear();

        QPixmap pixmap;
        pixmap.convertFromImage(kreenshotEditor->getBaseImage());
        auto baseImageItem = new QGraphicsPixmapItem(pixmap);
        scene.addItem(baseImageItem);

        foreach (ItemPtr item, kreenshotEditor->itemsManager()->items()) {

            auto kgrItem = toolManager->createGraphicsItemFromKreenItem(item, &scene);
            scene.addItem(kgrItem);

            // todo: remove later
//             auto mouseOverMapItem = mouseOverMap.find(item);
//             bool isMouseOver = mouseOverMapItem != mouseOverMap.end() && mouseOverMapItem->second == true;
//
//             auto selectedMapItem = selectedMap.find(item);
//             bool isSelected = selectedMapItem != selectedMap.end() && selectedMapItem->second == true;
//
//             // highlight: // TODO: for "drawRoundRect" move this away from the scene but directly to the painter!
//             if (isMouseOver && !isSelected) {
//                 //qDebug() << "item: " << item->rect() << "mouseOverMapItem";
//
//                 // removes any other effect which is not desired:
//                 //auto effect = new QGraphicsColorizeEffect();
//                 //effect->setColor(Qt::blue);
//                 //grItem->setGraphicsEffect(effect);
//
//                 auto rectItem = new QGraphicsRectItem();
//                 rectItem->setRect(item->rect().marginsAdded(QMargins(mouseOverMargin, mouseOverMargin, mouseOverMargin, mouseOverMargin)));
//                 //rectItem->setPen(QPen(Qt::gray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
//                 rectItem->setPen(QPen(Qt::transparent));
//                 rectItem->setBrush(QBrush(Qt::Dense7Pattern));
//                 scene.addItem(rectItem);
//             }
//
//             // selected indicator: TODO: draw handles (as extra class?)
//             if (isSelected) {
//                 auto rectItem = new QGraphicsRectItem();
//                 rectItem->setRect(item->rect().marginsAdded(QMargins(mouseOverMargin, mouseOverMargin, mouseOverMargin, mouseOverMargin)));
//                 rectItem->setPen(QPen(Qt::darkGray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
//                 //rectItem->setBrush(QBrush(Qt::Dense7Pattern));
//                 scene.addItem(rectItem);
//             }
        }

        updateItemsGeometryFromModel();
    }

    /**
     * update positions and sizes from model
     */
    void updateItemsGeometryFromModel()
    {
        foreach(auto grItem, graphicsView->items()) {
            //qDebug() << "muh";
            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
            if (grItemBase != nullptr) { // there might also be other items
                //qDebug() << "updateGeometryFromModel";
                grItemBase->updateVisualGeometryFromModel();
            }
        }
    }

    void updateItemsBehaviourFromChosenTool()
    {
        foreach(auto grItem, graphicsView->items()) {
            //qDebug() << "muh";
            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
            if (grItemBase != nullptr) { // there might also be other items
                grItemBase->setMovable(toolManager->chosenTool == ToolEnum::Select);
            }
        }
    }

    void setChosenTool(ToolEnum tool)
    {
        toolManager->chosenTool = tool;
        updateItemsBehaviourFromChosenTool();
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

MainEditorWidget::MainEditorWidget(KreenshotEditorPtr kreenshotEditor)
{
    d = MainEditorWidgetImplPtr(new MainEditorWidgetImpl());
    d->kreenshotEditor = kreenshotEditor;

    bool oldScrollAreaCode = false;

    if (oldScrollAreaCode) {
        // for QScrollArea:
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setMinimumSize(d->getBaseRect().size());
    }
    else {
        // use this if not using QScrollArea:
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumSize(50, 50);
    }

    setMouseTracking(true); // to enable mouseMoveEvent

    auto layout = new QGridLayout();
    this->setLayout(layout);
    _graphicsView = new MyQGraphicsView(&d->scene, d->toolManager /*, kreenshotEditor->itemsManager()*/);
    layout->addWidget(_graphicsView, 0, 0);
    layout->setMargin(0);

    //d->createDemoScene();
    d->graphicsView = _graphicsView;
    d->kreenshotEditor->itemsManager()->addDemoItems();
    d->initScene(_graphicsView);

    // makes sure that every time the mouse is released the whole scene is update from model
    // to check if everything is ok (e. g. with multiselection moves)
    connect(_graphicsView, SIGNAL(mouseReleased()), this, SLOT(updateItemsGeometryFromModel()));
    connect(_graphicsView, SIGNAL(itemCreated(ItemPtr)), this, SLOT(addItemToModel(ItemPtr)));
}

MainEditorWidget::~MainEditorWidget()
{

}

// todo: remove
void MainEditorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // QPainter painterImage(QImage); // TODO: use this to render to image and then to save to file

//     auto baseImage = d->kreenshotEditor->getBaseImage();
//     painter.drawImage(QPoint(0, 0), baseImage);

    //painter.drawRect(205, 205, 30, 30);

    //painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin)); // TODO: set antialias
    //painter.drawRoundRect(100, 200, 100, 200, 10, 10);

    //d->scene.render(&painter);
    //d->graphicsView.render(&painter); // does not work like this
}

void MainEditorWidget::requestTool(QString toolId)
{
    if (toolId == "select") {
        d->setChosenTool(ToolEnum::Select);
    }
    else if (toolId == "rect") {
        d->setChosenTool(ToolEnum::DrawRect);
    }
    else if (toolId == "ellipse") {
        d->setChosenTool(ToolEnum::DrawEllipse);
    }
    else if (toolId == "line") {
        d->setChosenTool(ToolEnum::DrawLine);
    }
    else if (toolId == "op-crop") {
        d->setChosenTool(ToolEnum::OperationCrop);
    }
    else {
        d->setChosenTool(ToolEnum::Select); // needed

        QString message = QString("TODO / Unknown tool id '%1'").arg(toolId);
        qDebug() << message;
        QMessageBox::information(this, "Not impl", message);
        emit toolChosen("select");
        return;
    }

    emit toolChosen(toolId);
}

void MainEditorWidget::updateItemsGeometryFromModel()
{
    qDebug() << "updateItemsGeometryFromModel";
    d->updateItemsGeometryFromModel();
}

void MainEditorWidget::addItemToModel(ItemPtr item)
{
    qDebug() << "add item: " << item->rect();
    d->kreenshotEditor->itemsManager()->addItem(item);
    d->createSceneFromModel();
}

void MainEditorWidget::saveToFile(QString filepath)
{
    qDebug() << filepath;
    qDebug() << QImageReader::supportedImageFormats();
    qDebug() << "MainEditorWidget::saveToFile(QString filepath): " << filepath;
    QImage image = d->kreenshotEditor->getBaseImage().copy();
    //qDebug() << image.isNull();
    QPainter painterImage(&image);
    painterImage.setRenderHint(QPainter::Antialiasing);
    d->scene.render(&painterImage);
    //qDebug() << image.save(filepath, "png"); // returns false;
    QImageWriter writer(filepath);
    writer.write(image);
    qDebug() << writer.error() << writer.errorString();
}
