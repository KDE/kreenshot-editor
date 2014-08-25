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
    void reset()
    {
        imageOperationGraphicsItem = nullptr; // todo: memleak?
        imageOperationItem = nullptr;
    }

    bool imageOperationItemActive()
    {
        return imageOperationItem != nullptr;
    }

public: // todo: make private
    /**
     * not nullptr if there is an image operation item (like crop) waiting for the user
     */
    QGraphicsItem* imageOperationGraphicsItem = nullptr; // TODO: rename to imgOpGraphicsItem
    KreenItemPtr imageOperationItem = nullptr; // imgOpItem
};

class MainEditorWidgetImpl
{
public:
    MainEditorWidget* _owner;
    KreenshotEditorPtr kreenshotEditor() {
        return _kreenshotEditor;
    };
    ToolManagerPtr toolManager_;
    KreenGraphicsViewPtr graphicsView;
    QGraphicsPixmapItem* baseImageSceneItem = nullptr;
    ImageOperationHandling imgOpHandling;
    SelectionHandlesPtr selectionHandles;

private:
    KreenshotEditorPtr _kreenshotEditor;

public:
    MainEditorWidgetImpl(MainEditorWidget* owner)
    {
        _owner = owner;
    }

    void init(KreenshotEditorPtr kreenshotEditor_)
    {
        _kreenshotEditor = kreenshotEditor_;

    }

    void initScene() {
        qDebug() << "initScene()";

        // this might still exist and the clear() would delete it
        if (baseImageSceneItem) {
            // delete baseImageSceneItem; // todo: mem leak?
            baseImageSceneItem = nullptr;
        }

        imgOpHandling.reset();

        scene()->clear();

        QRect baseRect = kreenshotEditor()->document()->baseImage().rect();
        scene()->setSceneRect(baseRect);

        // WORKAROUND:
        graphicsView->setSceneRect(0, 0, 10, 10); // this makes sure that the view scrolls to 0, 0
        graphicsView->setScene(scene().get());
        graphicsView->setSceneRect(baseRect); // this makes sure the scroll bars are shown for large images

        // scroll to 0,0 / does all not work:
        //graphicsView->scroll(-100, -100);
        // graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        // graphicsView->ensureVisible(0, 0, 1, 1);

        createSceneFromModel();

        // cancel current img op or anything else
        // also prevents some bugs from happening when opening a new document
        _owner->requestTool("select");
    }

    /**
     * (Re)create the scene to reflect the current kreenshotEditor->document().
     * Saves and restores the selected items.
     */
    void createSceneFromModel()
    {
        QRect baseRect = kreenshotEditor()->document()->baseImage().rect();

        qDebug() << "createSceneFromModel(). baseRect=" << baseRect;

        // save the current item selection if any
        //
        scene()->saveCurrentKreenItemsSelection();

        // Remove all KreenItems and base image item
        // but NOT the one possible image operation (see imgOpHandling.imageOperationItemActive())
        //
        foreach (auto item, scene()->kreenGraphicsItems()) {
            if (!item->item()->isImageOperation()) {
                scene()->removeItem(item->graphicsItem());
            }
        }

        if (baseImageSceneItem) {
            scene()->removeItem(baseImageSceneItem);
            // NOTE: only scene()->clear() also DELETES the items
            //delete baseImageSceneItem; // todo: more cleanup elsewhere?
            baseImageSceneItem = nullptr; // todo: mem leak?
        }

        // set base image
        //
        QPixmap pixmap;
        pixmap.convertFromImage(kreenshotEditor()->document()->baseImage());
        baseImageSceneItem = new QGraphicsPixmapItem(pixmap);
        graphicsView->setHelperBaseImageItem(baseImageSceneItem);
        scene()->addItem(baseImageSceneItem);

        scene()->setSceneRect(baseRect);
        graphicsView->setSceneRect(baseRect); // needed, otherwise no visual update

        //
        // create QGraphicsItems from all document KreenItems and add them to scene
        //
        foreach (KreenItemPtr item, kreenshotEditor()->document()->items()) {

            auto grItem = toolManager()->createGraphicsItemFromKreenItem(item);
            scene()->addItem(grItem);
        }

        slotUpdateItemsGeometryFromModel();
        updateItemsBehaviourFromChosenTool();

        scene()->restoreSavedKreenItemsSelection_1();
    }

//     std::map<ItemPtr, bool> mouseOverMap; // TODO later
//     const int mouseOverMargin = 2; // TODO later

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
     * update positions and sizes from model
     */
    void slotUpdateItemsGeometryFromModel()
    {
        foreach(auto grItemBase, scene()->kreenGraphicsItems()) {

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
        foreach(auto grItemBase, scene()->kreenGraphicsItems()) {
            grItemBase->setSelectableAndMovable(toolManager()->chosenTool() == ToolEnum::Select);
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

    /**
     * DEPRECATED, todo: remove
     */
    void createDemoScene()
    {
        scene()->setSceneRect(kreenshotEditor()->document()->baseImage().rect());

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

    d->selectionHandles = std::make_shared<SelectionHandles>(d->scene().get()); // needs valid kreenshotEditor

    d->kreenshotEditor()->document()->graphicsScene()->setToolManager(d->toolManager());

    connect(d->kreenshotEditor()->document().get(), SIGNAL(contentChangedSignal()),
            this, SLOT(slotDocumentContentChanged()));

    d->initScene();

    // makes sure that every time the mouse is released the whole scene is update from model
    // to check if everything is ok (e. g. with multiselection moves)
    connect(d->scene().get(), SIGNAL(mouseReleasedSignal()), this, SLOT(slotUpdateItemsGeometryFromModel()));

    connect(d->scene().get(), SIGNAL(itemCreated(KreenItemPtr)), this, SLOT(slotHandleNewItem(KreenItemPtr)));

    connect(d->scene().get(), SIGNAL(selectionChanged()), this, SLOT(slotSceneSelectionChanged()));

    d->kreenshotEditor()->document()->addDemoItems(); // TODO: remove later
}

void MainEditorWidget::slotDocumentContentChanged()
{
    qDebug() << "MainEditorWidget::slotDocumentContentChanged()";
    d->createSceneFromModel();
}

void MainEditorWidget::setSceneImageOperationItem(KreenItemPtr imageOperationItem)
{
    qDebug() << "setSceneImageOperationItem" << imageOperationItem.get();

    d->imgOpHandling.imageOperationItem = imageOperationItem; // this makes imgOpHandling.imageOperationItemActive() to be true
    d->toolManager()->setImageOperationActive(d->imgOpHandling.imageOperationItemActive());

    // first remove img op graphics item and reset imgOpHandling
    if (d->imgOpHandling.imageOperationGraphicsItem != nullptr) {
        d->scene()->removeItem(d->imgOpHandling.imageOperationGraphicsItem);
        d->imgOpHandling.reset();
    }

    // then depending on active or not:
    if (d->imgOpHandling.imageOperationItemActive()) {
        auto grItem = d->toolManager()->createGraphicsItemFromKreenItem(imageOperationItem);
        d->scene()->addItem(grItem);
        d->imgOpHandling.imageOperationGraphicsItem = grItem;
        auto grItemBase = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        grItemBase->setIsCreating(false); // todo: needed?
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
    else if (toolId == "obfuscate") {
        toolEnum = ToolEnum::DrawObfuscate;
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
        setSceneImageOperationItem(nullptr);
    }

    d->graphicsView->setCursorFromChosenTool();
    d->updateDragModeFromChosenTool();
    emit toolChosenSignal(toolId);
}

void MainEditorWidget::slotUpdateItemsGeometryFromModel()
{
    qDebug() << "slotUpdateItemsGeometryFromModel";

    // NOTE that slotUpdateItemsGeometryFromModel() is currently
    // always called when the mouse button is released.
    // This is not that bad. And later we will have the same with
    // key presses (only after the key is released after pressing it a while that slot will be called).
    //
    // First we find out if any changes actually happen:
    //

    // find relevant items:
    //
    QList<KreenItemPtr> relevantItems;
    foreach (auto item, d->scene()->selectedKreenItems()) {
        if (!item->isImageOperation()) {
            // only KreenItems which are not the
            // potential image operation item because it does not belong to the document
            relevantItems << item;
        }
    }

    // find if any change has happend:
    //
    bool hasAnyItemChanged = false;
    auto doc = d->kreenshotEditor()->document();

    foreach (auto item, relevantItems) {
        hasAnyItemChanged = hasAnyItemChanged || doc->hasItemPropertiesChanged(item);
    }

    // if yet, apply to document and do some updates
    //
    if (hasAnyItemChanged) {

        doc->contentChangedNotificationGroupBegin(
            true, QString("At least one property change"));

        foreach (auto item, relevantItems) {
            doc->applyItemPropertyChanges(item);
        }

        d->slotUpdateItemsGeometryFromModel();
        d->selectionHandles->redrawSelectionHandles(true);

        // emits contentChangedSignal() which triggers slotDocumentContentChanged()
        doc->contentChangedNotificationGroupEnd();
    }
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

    setSceneImageOperationItem(nullptr); // remove image operation item

    // would causes crash in mouse event if not called in the decoupled method
    // TODO: is above statement still true?
    d->createSceneFromModel();

    requestTool("select"); // go to Select after an image operation
}

void MainEditorWidget::slotImageOperationCanceled()
{
    setSceneImageOperationItem(nullptr);
}

void MainEditorWidget::slotHandleNewItem(KreenItemPtr item)
{
    Q_ASSERT(item != nullptr);

    qDebug() << "add item: " << item->rect();
    if (!item->isImageOperation()) {

        // emits contentChangedSignal() which triggers slotDocumentContentChanged()
        d->kreenshotEditor()->document()->addItem(item);

        qDebug() << "add item id: " << item->id(); // id was assigned by addItem

        // must be called after slotDocumentContentChanged() because there the GraphicsItem is created
        // todo: find a better place for this?
        // (why? still valid comment? -> make item selectable AFTER calling updateItemsBehaviourFromChosenTool() because we might override)
        auto newKGrItem = d->scene()->graphicsItemFromItem(item); // todo: use method from GrItemBase
        if (newKGrItem != nullptr) {
            newKGrItem->setSelectable(true); // not movable because we are still in creating mode but we would like to be able to resize the new item using the handles
            newKGrItem->graphicsItem()->setSelected(true);
        }
        else {
            Q_ASSERT_X(false, "MainEditorWidget::slotHandleNewItem", "should never happen");
        }
    }
    else {
        setSceneImageOperationItem(item);
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
        if (!item->isImageOperation()) { // not the image operation
            d->kreenshotEditor()->document()->deleteItem(item);
        }
    }

    // emits contentChangedSignal() which triggers slotDocumentContentChanged()
    d->kreenshotEditor()->document()->contentChangedNotificationGroupEnd();
}

void MainEditorWidget::selectAllItems()
{
    requestTool("select");

    // todo: potential optimization: disable events before the loop and enable afterwards
    foreach(auto kreenGraphicsItemBase, d->scene()->kreenGraphicsItems()) {
        kreenGraphicsItemBase->graphicsItem()->setSelected(true);
    }
}

int MainEditorWidget::selectedItemsCount()
{
    return d->scene()->selectedItems().size();
}

}
}
