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
#include "maineditorwidget.h"
#include "core/document.h"
#include "core/documentfile.h"
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
#include "kreengraphicsitems.h"
#include "myqgraphicsscene.h"
#include "myqgraphicsview.h"
#include "toolmanager.h"

namespace kreen {
namespace ui {


//class ItemVisual {
//public:
// bool mouseOver; // TODO later
//};

class MainEditorWidgetImpl
{
public:
    KreenshotEditorPtr kreenshotEditor;
    MyQGraphicsViewPtr graphicsView;
    MyQGraphicsScenePtr scene;
    ToolManagerPtr toolManager;

    /**
     * not nullptr if there is an image operation item (like crop) waiting for the user
     */
    QGraphicsItem* imageOperationItem;

public:
    MainEditorWidgetImpl()
    {
        toolManager = std::make_shared<ToolManager>();
        scene = std::make_shared<MyQGraphicsScene>(toolManager);
        imageOperationItem = nullptr;
    }

//     std::map<ItemPtr, bool> mouseOverMap; // TODO later
//     const int mouseOverMargin = 2; // TODO later

public:
    // todo: optimize this method?
    QRect getBaseRect() {
        QImage baseImage = kreenshotEditor->documentFile()->document()->baseImage();
        QRect rect(0, 0, baseImage.width(), baseImage.height());
        qDebug() << rect;
        return rect;
    }

    // todo: remove later
//             auto mouseOverMapItem = mouseOverMap.find(item);
//             bool isMouseOver = mouseOverMapItem != mouseOverMap.end() && mouseOverMapItem->second == true;
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

    /**
     * update positions and sizes from model
     */
    void updateItemsGeometryFromModel()
    {
        foreach(auto grItem, graphicsView->items()) {
            //qDebug() << "muh";

            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
            if (grItemBase != nullptr) { // there might also be other items

                //if (!grItemBase->item()->typeId.startsWith("op-")) {
                //qDebug() << "updateGeometryFromModel";
                grItemBase->updateVisualGeometryFromModel();
//                 }
//                 else {
//                     qDebug() << "updateItemsGeometryFromModel: ignore because of op- item";
//                 }
            }
        }
    }

    void updateItemsBehaviourFromChosenTool()
    {
        foreach(auto grItem, graphicsView->items()) {
            //qDebug() << "muh";
            auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
            if (grItemBase != nullptr) { // there might also be other items
                grItemBase->setMovable(toolManager->chosenTool() == ToolEnum::Select);
            }
        }
    }

    bool imageOperationItemActive()
    {
        return imageOperationItem != nullptr;
    }

    void createDemoScene()
    {
        QRect rect = getBaseRect();
        scene->setSceneRect(rect);

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(3, 3));
            dropShadow->setBlurRadius(10);

            auto rectItem = new QGraphicsRectItem();
            rectItem->setRect(120, 100, 150, 100);
            rectItem->setGraphicsEffect(dropShadow);
            rectItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene->addItem(rectItem);
        }

        {
            auto rect2Item = new QGraphicsRectItem();
            rect2Item->setRect(10, 5, 30, 40);
            //rect2Item->setGraphicsEffect(dropShadow);
            //rect2Item->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene->addItem(rect2Item);
        }

        scene->addText("Hello, world!");

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(2, 2));
            dropShadow->setBlurRadius(5);

            auto textItem = new QGraphicsTextItem("With drop shadow");
            textItem->setPos(30, 60);
            textItem->setGraphicsEffect(dropShadow);
            scene->addItem(textItem);
        }

        {
            auto textItem = new QGraphicsTextItem("TODO: With white glow background");
            textItem->setPos(30, 80);
            scene->addItem(textItem);
        }
    }
};

MainEditorWidget::MainEditorWidget(KreenshotEditorPtr kreenshotEditor)
{
    d = std::make_shared<MainEditorWidgetImpl>();
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
    d->graphicsView = std::make_shared<MyQGraphicsView>();
    layout->addWidget(d->graphicsView.get(), 0, 0);
    layout->setMargin(0);

    //d->createDemoScene();
    d->kreenshotEditor->documentFile()->document()->addDemoItems();
    initScene();

    // makes sure that every time the mouse is released the whole scene is update from model
    // to check if everything is ok (e. g. with multiselection moves)
    connect(d->scene.get(), SIGNAL(mouseReleased()), this, SLOT(updateItemsGeometryFromModel()));

    connect(d->scene.get(), SIGNAL(itemCreated(ItemPtr)), this, SLOT(handleNewItem(ItemPtr)));
}

MainEditorWidget::~MainEditorWidget()
{

}

void MainEditorWidget::initScene() {
    QRect rect = d->getBaseRect();
    d->scene->setSceneRect(rect);

    d->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //graphicsView->setBackgroundBrush(QBrush(Qt::BDiagPattern));
    d->graphicsView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::DiagCrossPattern)); // todo: make nicer

    d->graphicsView->setSceneRect(0, 0, 10, 10); // this makes sure that the view scrolls to 0, 0
    d->graphicsView->setScene(d->scene.get());
    d->graphicsView->setSceneRect(rect); // this makes sure the scroll bars are shown for large images

    // scroll to 0,0 / does all not work:
    //graphicsView->scroll(-100, -100);
    // graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    // graphicsView->ensureVisible(0, 0, 1, 1);

    createSceneFromModel();
}

/**
    * recreate the scene to reflect the current kreenshotEditor->itemsManager
    */
void MainEditorWidget::createSceneFromModel()
{
    d->scene->clear();

    QPixmap pixmap;
    pixmap.convertFromImage(d->kreenshotEditor->documentFile()->document()->baseImage());
    auto baseImageItem = new QGraphicsPixmapItem(pixmap);
    d->scene->addItem(baseImageItem);

    foreach (ItemPtr item, d->kreenshotEditor->documentFile()->document()->items()) {

        auto kgrItem = d->toolManager->createGraphicsItemFromKreenItem(item, d->scene.get());
        d->scene->addItem(kgrItem);
    }

    d->updateItemsGeometryFromModel();
}

void MainEditorWidget::updateSceneWithImageOperationItem(ItemPtr item)
{
    qDebug() << "updateSceneWithImageOperationItem";

    d->toolManager->setImageOperationActive(item != nullptr);

    if (d->imageOperationItem != nullptr) {
        d->scene->removeItem(d->imageOperationItem);
        d->imageOperationItem = nullptr;
    }

    if (item != nullptr) {

//             auto dimRect = new QGraphicsRectItem();
//             //dimRect->setBrush();
//             scene.addItem(dimRect);

        auto kgrItem = d->toolManager->createGraphicsItemFromKreenItem(item, d->scene.get());
        d->scene->addItem(kgrItem);
        d->imageOperationItem = kgrItem;
        auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(kgrItem);
        grItemBase->setIsCreating(false);
        grItemBase->updateVisualGeometryFromModel();
        connect(grItemBase, SIGNAL(operationAccepted()), this, SLOT(imageOperationAccepted()));
        connect(grItemBase, SIGNAL(operationCanceled()), this, SLOT(imageOperationCanceled()));
    }
}

// todo: remove
void MainEditorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // QPainter painterImage(QImage); // TODO: use this to render to image and then to save to file

//     auto baseImage = d->kreenshotEditor->baseImage();
//     painter.drawImage(QPoint(0, 0), baseImage);

    //painter.drawRect(205, 205, 30, 30);

    //painter.setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin)); // TODO: set antialias
    //painter.drawRoundRect(100, 200, 100, 200, 10, 10);

    //d->scene.render(&painter);
    //d->graphicsView.render(&painter); // does not work like this
}

void MainEditorWidget::requestTool(QString toolId)
{
    ToolEnum toolEnum;

    if (toolId == "select") {
        toolEnum = ToolEnum::Select;
    }
    else if (toolId == "rect") {
        toolEnum = ToolEnum::DrawRect;
    }
    else if (toolId == "ellipse") {
        toolEnum = ToolEnum::DrawEllipse;
    }
    else if (toolId == "line") {
        toolEnum = ToolEnum::DrawLine;
    }
    else if (toolId == "op-crop") {
        toolEnum = ToolEnum::OperationCrop;
    }
    else {
        toolEnum = ToolEnum::Select;
        toolId = "select";

        QString message = QString("TODO / Unknown tool id '%1'").arg(toolId);
        qDebug() << message;
        QMessageBox::information(this, "Not impl", message);
    }

    d->toolManager->setChosenTool(toolEnum, this);
    d->updateItemsBehaviourFromChosenTool();

    // remove current image operation if another tool is selected
    if (!toolId.startsWith("op-")) {
        updateSceneWithImageOperationItem(nullptr);
    }

    emit toolChosen(toolId);
}

void MainEditorWidget::updateItemsGeometryFromModel()
{
    qDebug() << "updateItemsGeometryFromModel";
    d->updateItemsGeometryFromModel();
}

void MainEditorWidget::imageOperationAccepted()
{
    qDebug() << "............TODO: MainEditorWidget::imageOperationAccepted()";
//     auto baseImage = d->kreenshotEditor->baseImage();
//     d->kreenshotEditor->setBaseImage(baseImage.copy(QRect(10, 10, 100, 100)));
//     initScene(d->graphicsView); // causes crash
}

void MainEditorWidget::imageOperationCanceled()
{
    updateSceneWithImageOperationItem(nullptr);
}

void MainEditorWidget::handleNewItem(ItemPtr item)
{
    qDebug() << "add item: " << item->rect();
    if (!item->typeId.startsWith("op-")) {
        d->kreenshotEditor->documentFile()->document()->addItem(item);
        createSceneFromModel();
    }
    else {
        updateSceneWithImageOperationItem(item);
    }
}

ErrorStatus MainEditorWidget::saveToFile(QString filepath)
{
    qDebug() << filepath;
    qDebug() << QImageReader::supportedImageFormats();
    qDebug() << "MainEditorWidget::saveToFile(QString filepath): " << filepath;
    QImage image = d->kreenshotEditor->documentFile()->document()->baseImage().copy();
    //qDebug() << image.isNull();
    QPainter painterImage(&image);
    painterImage.setRenderHint(QPainter::Antialiasing);
    d->scene->render(&painterImage);
    //qDebug() << image.save(filepath, "png"); // returns false;
    QImageWriter writer(filepath);
    writer.write(image);
    qDebug() << "error code: " << writer.error();
    if (writer.error() != 0) {
        qDebug() << "error: " << writer.errorString();
        // throw std::runtime_error(
        return tr("Error saving image ('%1'): '%2' (code %3)")
            .arg(filepath)
            .arg(writer.errorString())
            .arg(writer.error());
    }

    return ErrorStatus(); // TODO: if we forget that the compiler does not warn but we get garbage out
}

}
}
