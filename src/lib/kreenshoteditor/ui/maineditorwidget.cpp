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
#include "../core/impl/selectionhandles.h" // TODO: move to ui/impl?

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

class MainEditorWidgetImpl
{
public:
    MainEditorWidget* _owner;
    KreenshotEditorPtr kreenshotEditor() { return _kreenshotEditor; };
    ToolManagerPtr toolManager_;
    KreenGraphicsViewPtr graphicsView;
    QGraphicsPixmapItem* baseImageSceneItem;
    ImageOperationHandling imgOpHandling;
    SelectionHandlesPtr selectionHandles;

private:
    KreenshotEditorPtr _kreenshotEditor;

public:
    MainEditorWidgetImpl(MainEditorWidget* owner)
    {
        _owner = owner;
        imgOpHandling.imageOperationGraphicsItem = nullptr;
    }

    void init(KreenshotEditorPtr kreenshotEditor_)
    {
        _kreenshotEditor = kreenshotEditor_;

    }

    void onNewDocument()
    {
        selectionHandles = std::make_shared<SelectionHandles>(scene().get()); // needs valid kreenshotEditor

        kreenshotEditor()->document()->graphicsScene()->setToolManager(toolManager());

        _owner->connect(kreenshotEditor()->document().get(), SIGNAL(contentChangedSignal()),
                _owner, SLOT(slotDocumentContentChanged()));

        kreenshotEditor()->document()->addDemoItems(); // todo: remove later
    }

    void initScene() {
        QRect rect = getBaseRect();
        scene()->setSceneRect(rect);

        // WORKAROUND:
        graphicsView->setSceneRect(0, 0, 10, 10); // this makes sure that the view scrolls to 0, 0
        graphicsView->setScene(scene().get());
        graphicsView->setSceneRect(rect); // this makes sure the scroll bars are shown for large images

        // scroll to 0,0 / does all not work:
        //graphicsView->scroll(-100, -100);
        // graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        // graphicsView->ensureVisible(0, 0, 1, 1);

        createSceneFromModel();
    }

    /**
     * recreate the scene to reflect the current kreenshotEditor->document()
     */
    void createSceneFromModel()
    {
        scene()->clear();

        QPixmap pixmap;
        pixmap.convertFromImage(kreenshotEditor()->document()->baseImage());
        baseImageSceneItem = new QGraphicsPixmapItem(pixmap);
        graphicsView->setHelperBaseImageItem(baseImageSceneItem);
        scene()->addItem(baseImageSceneItem);

        foreach (KreenItemPtr item, kreenshotEditor()->document()->items()) {

            auto grItem = toolManager()->createGraphicsItemFromKreenItem(item, scene().get());
            auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(grItem);

            _owner->connect(grItemBase, SIGNAL(itemPositionHasChangedSignal()), _owner, SLOT(slotRedrawSelectionHandles()));

            scene()->addItem(grItem);
        }

        slotUpdateItemsGeometryFromModel();
        updateItemsBehaviourFromChosenTool();
    }

//     std::map<ItemPtr, bool> mouseOverMap; // TODO later
//     const int mouseOverMargin = 2; // TODO later

    // todo: optimize this method?
    QRect getBaseRect() {
        QImage baseImage = _kreenshotEditor->document()->baseImage();
        QRect rect(0, 0, baseImage.width(), baseImage.height());
        qDebug() << rect;
        return rect;
    }

    KreenGraphicsScenePtr scene()
    {
        return _kreenshotEditor->document()->graphicsScene();
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
     * all corresponding graphics items for all KreenItems (including op- items like crop or rip out!)
     */
    QList<KreenGraphicsItemBase*> kreenGraphicsItems()
    {
        QList<KreenGraphicsItemBase*> list;

        foreach(auto grItem, scene()->items()) {

            auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(grItem);
            if (grItemBase != nullptr) { // there might also be other items
                list << grItemBase;
            }
        }

        return list;
    }

    /**
     * update positions and sizes from model
     */
    void slotUpdateItemsGeometryFromModel()
    {
        foreach(auto grItemBase, kreenGraphicsItems()) {

            //if (!grItemBase->item()->typeId.startsWith("op-")) {
            //qDebug() << "updateGeometryFromModel";
            grItemBase->updateVisualGeometryFromModel();
//                 }
//                 else {
//                     qDebug() << "updateItemsGeometryFromModel: ignore because of op- item";
//                 }
        }
    }

    /**
     * E. g. if "Select" tool is selected then items are movable. Otherwise not.
     */
    void updateItemsBehaviourFromChosenTool()
    {
        foreach(auto grItemBase, kreenGraphicsItems()) {
            grItemBase->setMovable(toolManager()->chosenTool() == ToolEnum::Select);
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

    QGraphicsItem* graphicsItemFromItem(KreenItemPtr item)
    {
        foreach(auto kreenGraphicsItemBase, kreenGraphicsItems()) {
            if (kreenGraphicsItemBase->item()->id() == item->id()) { // compare by id because Document holds secret copies.
                return kreenGraphicsItemBase->graphicsItem();
            }
        }
        return nullptr;
    }

    /**
     * DEPRECATED, todo: remove
     */
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

MainEditorWidget::MainEditorWidget(KreenshotEditorPtr kreenshotEditor)
{
    KREEN_PIMPL_INIT_THIS(MainEditorWidget);
    d->init(kreenshotEditor);
    d->toolManager_ = std::make_shared<ToolManager>();

//     bool oldScrollAreaCode = false;
//
//     if (oldScrollAreaCode) {
//         // for QScrollArea:
//         setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//         setMinimumSize(d->getBaseRect().size());
//     }
//     else {
//         // use this if not using QScrollArea:
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumSize(50, 50);
//     }

    setMouseTracking(true); // to enable mouseMoveEvent

    auto layout = new QGridLayout();
    this->setLayout(layout);
    d->graphicsView = std::make_shared<KreenGraphicsView>(d->toolManager());
    //d->graphicsView->setRubberBandSelectionMode(Qt::IntersectsItemShape); // default
    layout->addWidget(d->graphicsView.get(), 0, 0);
    layout->setMargin(0);

    connect(kreenshotEditor.get(), SIGNAL(newDocumentCreatedSignal()), this, SLOT(slotDocumentCreated()));
    if (kreenshotEditor->document() != nullptr) {
        slotDocumentCreated();
    }
}

MainEditorWidget::~MainEditorWidget()
{

}

void MainEditorWidget::slotDocumentCreated()
{
    qDebug() << "MainEditorWidget::slotDocumentCreated()";
    d->onNewDocument();
    d->initScene();

    // makes sure that every time the mouse is released the whole scene is update from model
    // to check if everything is ok (e. g. with multiselection moves)
    connect(d->scene().get(), SIGNAL(mouseReleasedSignal()), this, SLOT(slotUpdateItemsGeometryFromModel()));

    connect(d->scene().get(), SIGNAL(itemCreated(KreenItemPtr)), this, SLOT(slotHandleNewItem(KreenItemPtr)));

    connect(d->scene().get(), SIGNAL(selectionChanged()), this, SLOT(slotSceneSelectionChanged()));
}

void MainEditorWidget::slotDocumentContentChanged()
{
    d->createSceneFromModel();
}

void MainEditorWidget::slotUpdateSceneWithImageOperationItem(KreenItemPtr item)
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
        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        grItemBase->setIsCreating(false);
        grItemBase->updateVisualGeometryFromModel();
        connect(grItemBase, SIGNAL(operationAccepted()), this, SLOT(slotImageOperationAccepted()));
        connect(grItemBase, SIGNAL(operationCanceled()), this, SLOT(slotImageOperationCanceled()));
    }
}

void MainEditorWidget::paintEvent(QPaintEvent* event)
{
    // QPainter painter(this);
    QWidget::paintEvent(event);
}

// void setCursorFromChosenTool(ToolManagerPtr toolManager, QGraphicsItem* imageItem)
// {
//     // workaround (not really) for https://bugreports.qt-project.org/browse/QTBUG-4190
//     //QWidget* w = imageItem;
//     QCursor curCursor = imageItem->cursor();
//     Qt::CursorShape newCursorShape;
//     auto tool = toolManager->chosenTool();
//
//     if (tool == Select) {
//         newCursorShape = Qt::ArrowCursor;
//     }
//     else if (tool == DrawRect) {
//         newCursorShape = Qt::CrossCursor;
//     }
//     else if (tool == DrawLine) {
//         newCursorShape = Qt::CrossCursor;
//     }
//     else if (tool == DrawEllipse) {
//         newCursorShape = Qt::CrossCursor;
//     }
//     else if (tool == DrawText) {
//         newCursorShape = Qt::CrossCursor;
//     }
//     else if (tool == OperationCrop) {
//         newCursorShape = Qt::CrossCursor;
//     }
//     else {
//         qDebug() << "_chosenTool" << tool;
//         Q_ASSERT(false);
//     }
//
//     if (curCursor.shape() != newCursorShape) {
//         auto tool = toolManager->chosenTool();
//         qDebug() << QTime::currentTime() << " baseImage setCursor for " << tool << "cursor=" << newCursorShape;
//         imageItem->setCursor(newCursorShape);
//     }
// }


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
        slotUpdateSceneWithImageOperationItem(nullptr);
    }

    d->graphicsView->setCursorFromChosenTool();
    d->updateDragModeFromChosenTool();
    emit toolChosenSignal(toolId);
}

// TODO: do not apply EVERY mouse release (only those with real changes)
// TODO: creating a new item crashes
// TODO: the selection is lost after mouse release!
void MainEditorWidget::slotUpdateItemsGeometryFromModel()
{
    qDebug() << "updateItemsGeometryFromModel";

    d->kreenshotEditor()->document()->contentChangedNotificationGroupBegin(
        true, QString("slotUpdateItemsGeometryFromModel___todo"));
    foreach (auto item, d->scene()->selectedKreenItems()) {
        d->kreenshotEditor()->document()->applyItemPropertyChanges(item);
    }

    d->slotUpdateItemsGeometryFromModel();
    d->selectionHandles->redrawSelectionHandles(true);

    // emits contentChangedSignal() which triggers slotDocumentContentChanged()
    d->kreenshotEditor()->document()->contentChangedNotificationGroupEnd();
}

void MainEditorWidget::slotImageOperationAccepted()
{
    qDebug() << "MainEditorWidget::imageOperationAccepted(). Forward to imageOperationAcceptedDecoupled() because otherwise some mouse release event will crash because image operation object will be remove";
    QTimer::singleShot(0, this, SLOT(slotImageOperationAcceptedDecoupled()));
}

void MainEditorWidget::slotImageOperationAcceptedDecoupled()
{
    qDebug() << "MainEditorWidget::imageOperationAcceptedDecoupled()";
    d->kreenshotEditor()->document()->imageOpCrop(d->imgOpHandling.imageOperationItem->rect());

    slotUpdateSceneWithImageOperationItem(nullptr); // remove image operation item
    d->initScene(); // would causes crash in mouse event if not called in the decoupled method
    requestTool("select"); // go to Select after an image operation
}

void MainEditorWidget::slotImageOperationCanceled()
{
    slotUpdateSceneWithImageOperationItem(nullptr);
}

void MainEditorWidget::slotHandleNewItem(KreenItemPtr item)
{
    Q_ASSERT(item != nullptr);

    qDebug() << "add item: " << item->rect();
    if (!item->typeId.startsWith("op-")) {
        // emits contentChangedSignal() which triggers slotDocumentContentChanged()
        d->kreenshotEditor()->document()->addItem(item);

        // will (must be) called after slotDocumentContentChanged() because there the GraphicsItem is created
        // (why? still valid comment? -> make item selectable AFTER calling updateItemsBehaviourFromChosenTool() because we might override)
        auto newGrItem = d->graphicsItemFromItem(item);
        if (newGrItem != nullptr) {
            newGrItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
            newGrItem->setSelected(true);
            // qDebug() << "isSelected: " << grItem->isSelected();
        }
        else {
            Q_ASSERT_X(false, "MainEditorWidget::slotHandleNewItem", "should never happen");
        }

    }
    else {
        slotUpdateSceneWithImageOperationItem(item);
    }
}

void MainEditorWidget::slotSceneSelectionChanged()
{
    qDebug() << "sceneSelectionChanged() " << d->scene()->selectedItems();

    qDebug() << "[DEBUG] d->graphicsView->unsetCursor() (does not work yet). See comment in code.";
    d->graphicsView->unsetCursor(); // DETAIL: to have the cursor correct on the selection handle without having to move the mouse

    d->selectionHandles->redrawSelectionHandles(true);

    emit itemSelectionChanged();
}

void MainEditorWidget::slotRedrawSelectionHandles()
{
    //qDebug() << "SLOT redrawSelectionHandles";
    d->selectionHandles->redrawSelectionHandles(false);
}

void MainEditorWidget::deleteSelectedItems()
{
    QList<KreenItemPtr> toBeDeleted = d->scene()->selectedKreenItems();

    d->kreenshotEditor()->document()->contentChangedNotificationGroupBegin(
        true, QString("Delete %1 item(s)").arg(toBeDeleted.length()));
    foreach (auto item, toBeDeleted) {
        d->kreenshotEditor()->document()->deleteItem(item);
    }

    // emits contentChangedSignal() which triggers slotDocumentContentChanged()
    d->kreenshotEditor()->document()->contentChangedNotificationGroupEnd();
}

void MainEditorWidget::selectAllItems()
{
    requestTool("select");
    
    // todo: potential optimization: disable events before the loop and enable afterwards
    foreach(auto kreenGraphicsItemBase, d->kreenGraphicsItems()) {
        kreenGraphicsItemBase->graphicsItem()->setSelected(true);
    }
}

int MainEditorWidget::selectedItemsCount()
{
    return d->scene()->selectedItems().size();
}

}
}
