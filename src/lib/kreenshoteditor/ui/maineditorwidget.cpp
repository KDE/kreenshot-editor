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
#include <QTimer>
#include <memory>
#include "../core/impl/kreengraphicsitems.h"
#include "../core/impl/kreengraphicsscene.h"
#include "../core/impl/toolmanager.h"
#include "impl/kreengraphicsview.h"

namespace kreen {
namespace ui {


//class ItemVisual {
//public:
// bool mouseOver; // TODO later
//};

class ImageOperationHandling
{
public:
    /**
     * not nullptr if there is an image operation item (like crop) waiting for the user
     */
    QGraphicsItem* imageOperationGraphicsItem; // TODO: rename to imgOpGraphicsItem
    KreenItemPtr imageOperationItem; // imgOpItem

    bool imageOperationItemActive()
    {
        return imageOperationGraphicsItem != nullptr;
    }
};

KREEN_SHAREDPTR_FORWARD_DECL(SelectionHandles)
class SelectionHandles
{
public:
    SelectionHandles(MainEditorWidgetImpl* d_) {
        d = d_;
    }

private:
    MainEditorWidgetImpl* d;
    std::map<QGraphicsItem*, std::vector<QGraphicsRectItem*>> currentHandles;

public:
void redrawSelectionHandles(bool createNewHandles);
};

class MainEditorWidgetImpl
{
public:
    KreenshotEditorPtr kreenshotEditor;
    ToolManagerPtr toolManager_;
    KreenGraphicsViewPtr graphicsView;
    ImageOperationHandling imgOpHandling;
    SelectionHandlesPtr selectionHandles;

public:
    MainEditorWidgetImpl()
    {
        imgOpHandling.imageOperationGraphicsItem = nullptr;
        selectionHandles = std::make_shared<SelectionHandles>(this);
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

    KreenGraphicsScenePtr scene()
    {
        return kreenshotEditor->documentFile()->document()->graphicsScene();
    }

    ToolManagerPtr toolManager()
    {
        return toolManager_;
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
                grItemBase->setMovable(toolManager()->chosenTool() == ToolEnum::Select);
            }
        }
    }

    void updateDragModeFromChosenTool()
    {
        if (toolManager()->chosenTool() == ToolEnum::Select) {
            graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
            //graphicsView->setDragMode(QGraphicsView::ScrollHandDrag); // todo later: use this when press control?
        }
        else {
            graphicsView->setDragMode(QGraphicsView::NoDrag);
        }
    }

    void createDemoScene()
    {
        QRect rect = getBaseRect();
        scene()->setSceneRect(rect);

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(3, 3));
            dropShadow->setBlurRadius(10);

            auto rectItem = new QGraphicsRectItem();
            rectItem->setRect(120, 100, 150, 100);
            rectItem->setGraphicsEffect(dropShadow);
            rectItem->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene()->addItem(rectItem);
        }

        {
            auto rect2Item = new QGraphicsRectItem();
            rect2Item->setRect(10, 5, 30, 40);
            //rect2Item->setGraphicsEffect(dropShadow);
            //rect2Item->setPen(QPen(Qt::darkGreen, 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            scene()->addItem(rect2Item);
        }

        scene()->addText("Hello, world!");

        {
            auto dropShadow = new QGraphicsDropShadowEffect();
            dropShadow->setColor(Qt::black);
            dropShadow->setOffset(QPoint(2, 2));
            dropShadow->setBlurRadius(5);

            auto textItem = new QGraphicsTextItem("With drop shadow");
            textItem->setPos(30, 60);
            textItem->setGraphicsEffect(dropShadow);
            scene()->addItem(textItem);
        }

        {
            auto textItem = new QGraphicsTextItem("TODO: With white glow background");
            textItem->setPos(30, 80);
            scene()->addItem(textItem);
        }
    }
};

QGraphicsRectItem* newRectItemWithCursor(QRectF rect, const QCursor& cursor)
{
    auto grItem = new QGraphicsRectItem(rect);
    grItem->setBrush(QBrush(Qt::black));
    grItem->setPen(Qt::NoPen);
    //grItem->setFlag(QGraphicsItem::ItemIsMovable, true); // TODO
    grItem->setFlag(QGraphicsItem::ItemIsMovable, false);
    grItem->setCursor(cursor);
    return grItem;
}

void SelectionHandles::redrawSelectionHandles(bool createNewHandles)
{
    if (createNewHandles) {
        foreach (auto grItemPair, currentHandles) {
            foreach (auto grItem, grItemPair.second) {
                d->scene()->removeItem(grItem);
            }
        }

        currentHandles.clear();
    }

    const qreal hw = 10.0; // handle width, TODO: even or uneven numbers: these or those items will have blurred rects
    qreal hw2 = hw / 2.0;

    //
    // 1   2   3
    // 4   5   6
    // 7   8   9
    //
    std::vector<QCursor> cursors;
    cursors.push_back(Qt::ArrowCursor); // not used
    cursors.push_back(Qt::SizeFDiagCursor); // 1
    cursors.push_back(Qt::SizeVerCursor); // 2
    cursors.push_back(Qt::SizeBDiagCursor); // 3
    cursors.push_back(Qt::SizeHorCursor); // 4
    cursors.push_back(Qt::OpenHandCursor); // 5
    cursors.push_back(Qt::SizeHorCursor); // 6
    cursors.push_back(Qt::SizeBDiagCursor); // 7
    cursors.push_back(Qt::SizeVerCursor); // 8
    cursors.push_back(Qt::SizeFDiagCursor); // 9

    foreach (auto graphicsItem, d->scene()->selectedItems()) {

        // TODO ................. DO THIS AS SOON A HANDLE IS CLICKED
        //graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, false);
        // ......... and undo it after a handle operation is complete

        auto baseRect = graphicsItem->sceneBoundingRect();
        qreal x = baseRect.x();
        qreal y = baseRect.y();
        qreal w = baseRect.width();
        qreal h = baseRect.height();
        qreal w2 = floor(w / 2.0);
        qreal h2 = floor(h / 2.0);
        auto rect = QRectF(0, 0, hw, hw);

        //
        // 1   2   3
        // 4   5   6
        // 7   8   9
        //
        auto r1 = rect.translated(x - hw2, y - hw2); // 1
        auto r2 = rect.translated(x + w2 - hw2, y - hw2); // 2
        auto r3 = rect.translated(x + w - hw2, y - hw2); // 3

        auto r4 = rect.translated(x - hw2, y + h2 - hw2); // 4
        auto r6 = rect.translated(x + w - hw2, y + h2 - hw2); // 6

        auto r7 = rect.translated(x - hw2, y + h - hw2); // 7
        auto r8 = rect.translated(x + w2 - hw2, y + h - hw2); // 8
        auto r9 = rect.translated(x + w - hw2, y + h - hw2); // 9


        if (createNewHandles) {
            std::vector<QGraphicsRectItem*> handles;
            handles.push_back(newRectItemWithCursor(r1, cursors[1]));
            handles.push_back(newRectItemWithCursor(r2, cursors[2]));
            handles.push_back(newRectItemWithCursor(r3, cursors[3]));

            handles.push_back(newRectItemWithCursor(r4, cursors[4]));
            handles.push_back(newRectItemWithCursor(r6, cursors[6]));

            handles.push_back(newRectItemWithCursor(r7, cursors[7]));
            handles.push_back(newRectItemWithCursor(r8, cursors[8]));
            handles.push_back(newRectItemWithCursor(r9, cursors[9]));

            currentHandles.insert(std::make_pair(graphicsItem, handles));
        }
        else {
            std::vector<QGraphicsRectItem*> handles = currentHandles[graphicsItem];

            int i = 0;
            handles[i++]->setRect(r1);
            handles[i++]->setRect(r2);
            handles[i++]->setRect(r3);
            handles[i++]->setRect(r4);
            handles[i++]->setRect(r6);
            handles[i++]->setRect(r7);
            handles[i++]->setRect(r8);
            handles[i++]->setRect(r9);
        }
    }

    if (createNewHandles) {
        foreach (auto grItemPair, currentHandles) {
            foreach (auto grItem, grItemPair.second) {
                d->scene()->addItem(grItem);
            }
        }
    }
}

MainEditorWidget::MainEditorWidget(KreenshotEditorPtr kreenshotEditor)
{
    d = std::make_shared<MainEditorWidgetImpl>();
    d->kreenshotEditor = kreenshotEditor;
    d->toolManager_ = std::make_shared<ToolManager>();
    d->kreenshotEditor->documentFile()->document()->graphicsScene()->setToolManager(d->toolManager());

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
    d->graphicsView = std::make_shared<KreenGraphicsView>(d->toolManager());
    //d->graphicsView->setRubberBandSelectionMode(Qt::IntersectsItemShape); // default
    layout->addWidget(d->graphicsView.get(), 0, 0);
    layout->setMargin(0);

    //d->createDemoScene();
    d->kreenshotEditor->documentFile()->document()->addDemoItems();
    initScene();

    // makes sure that every time the mouse is released the whole scene is update from model
    // to check if everything is ok (e. g. with multiselection moves)
    connect(d->scene().get(), SIGNAL(mouseReleased()), this, SLOT(updateItemsGeometryFromModel()));

    connect(d->scene().get(), SIGNAL(itemCreated(ItemPtr)), this, SLOT(handleNewItem(ItemPtr)));

    connect(d->scene().get(), SIGNAL(selectionChanged()), this, SLOT(sceneSelectionChanged()));
}

MainEditorWidget::~MainEditorWidget()
{

}

void MainEditorWidget::initScene() {
    QRect rect = d->getBaseRect();
    d->scene()->setSceneRect(rect);

    d->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //graphicsView->setBackgroundBrush(QBrush(Qt::BDiagPattern));
    d->graphicsView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::DiagCrossPattern)); // todo: make nicer

    d->graphicsView->setSceneRect(0, 0, 10, 10); // this makes sure that the view scrolls to 0, 0
    d->graphicsView->setScene(d->scene().get());
    d->graphicsView->setSceneRect(rect); // this makes sure the scroll bars are shown for large images

    // scroll to 0,0 / does all not work:
    //graphicsView->scroll(-100, -100);
    // graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    // graphicsView->ensureVisible(0, 0, 1, 1);

    createSceneFromModel();
}

/**
 * recreate the scene to reflect the current kreenshotEditor->documentFile()->document()
 */
void MainEditorWidget::createSceneFromModel(KreenItemPtr selectNewItem /*= nullptr*/)
{
    d->scene()->clear();

    QPixmap pixmap;
    pixmap.convertFromImage(d->kreenshotEditor->documentFile()->document()->baseImage());
    auto baseImageItem = new QGraphicsPixmapItem(pixmap);
    d->scene()->addItem(baseImageItem);

    foreach (KreenItemPtr item, d->kreenshotEditor->documentFile()->document()->items()) {

        auto grItem = d->toolManager()->createGraphicsItemFromKreenItem(item, d->scene().get());
        auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);

        connect(grItemBase, SIGNAL(itemPositionHasChangedSignal()), this, SLOT(redrawSelectionHandles()));

        d->scene()->addItem(grItem);

        if (selectNewItem == item) {
            grItem->setSelected(true);
            qDebug() << "isSelected: " << grItem->isSelected();
        }
    }

    d->updateItemsGeometryFromModel();
}

void MainEditorWidget::updateSceneWithImageOperationItem(KreenItemPtr item)
{
    qDebug() << "updateSceneWithImageOperationItem";

    d->imgOpHandling.imageOperationItem = item;
    d->toolManager()->setImageOperationActive(item != nullptr);

    if (d->imgOpHandling.imageOperationGraphicsItem != nullptr) {
        d->scene()->removeItem(d->imgOpHandling.imageOperationGraphicsItem);
        d->imgOpHandling.imageOperationGraphicsItem = nullptr;
        d->imgOpHandling.imageOperationItem = nullptr;
    }

    if (item != nullptr) {

//             auto dimRect = new QGraphicsRectItem();
//             //dimRect->setBrush();
//             scene.addItem(dimRect);

        auto grItem = d->toolManager()->createGraphicsItemFromKreenItem(item, d->scene().get());
        d->scene()->addItem(grItem);
        d->imgOpHandling.imageOperationGraphicsItem = grItem;
        auto grItemBase = dynamic_cast<KreenQGraphicsItemBase*>(grItem);
        grItemBase->setIsCreating(false);
        grItemBase->updateVisualGeometryFromModel();
        connect(grItemBase, SIGNAL(operationAccepted()), this, SLOT(imageOperationAccepted()));
        connect(grItemBase, SIGNAL(operationCanceled()), this, SLOT(imageOperationCanceled()));
    }
}

void MainEditorWidget::paintEvent(QPaintEvent*)
{
    // QPainter painter(this);
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

    d->toolManager()->setChosenTool(toolEnum, this);
    d->updateItemsBehaviourFromChosenTool();

    // remove current image operation if another tool is selected
    if (!toolId.startsWith("op-")) {
        updateSceneWithImageOperationItem(nullptr);
    }

    d->graphicsView->setCursorFromChosenTool();
    d->updateDragModeFromChosenTool();
    emit toolChosen(toolId);
}

void MainEditorWidget::updateItemsGeometryFromModel()
{
    qDebug() << "updateItemsGeometryFromModel";
    d->updateItemsGeometryFromModel();
    d->selectionHandles->redrawSelectionHandles(true);
}

void MainEditorWidget::imageOperationAccepted()
{
    qDebug() << "MainEditorWidget::imageOperationAccepted(). Forward to imageOperationAcceptedDecoupled() because otherwise some mouse release event will crash because image operation object will be remove";
    QTimer::singleShot(0, this, SLOT(imageOperationAcceptedDecoupled()));
}

void MainEditorWidget::imageOperationAcceptedDecoupled()
{
    qDebug() << "MainEditorWidget::imageOperationAcceptedDecoupled()";
    d->kreenshotEditor->documentFile()->document()->operationCrop(d->imgOpHandling.imageOperationItem->rect());

    updateSceneWithImageOperationItem(nullptr); // remove image operation item
    initScene(); // would causes crash in mouse event if not called in the decoupled method
    requestTool("select"); // go to Select after an image operation
}


void MainEditorWidget::imageOperationCanceled()
{
    updateSceneWithImageOperationItem(nullptr);
}

void MainEditorWidget::handleNewItem(KreenItemPtr item)
{
    qDebug() << "add item: " << item->rect();
    if (!item->typeId.startsWith("op-")) {
        d->kreenshotEditor->documentFile()->document()->addItem(item);
        createSceneFromModel(item);
    }
    else {
        updateSceneWithImageOperationItem(item);
    }
}

void MainEditorWidget::sceneSelectionChanged()
{
    qDebug() << "sceneSelectionChanged() " << d->scene()->selectedItems();
    d->selectionHandles->redrawSelectionHandles(true);
}

void MainEditorWidget::redrawSelectionHandles()
{
    //qDebug() << "SLOT redrawSelectionHandles";
    d->selectionHandles->redrawSelectionHandles(false);
}

}
}
