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
#include "document.h"
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QClipboard>
#include <QUndoStack>
#include "impl/kreengraphicsscene.h"
#include "impl/toolmanager.h"
#include "impl/undocommands.h"

namespace kreen {
namespace core {

class DocumentImpl
{
public:
    DocumentImpl()
    {
        scene = KreenGraphicsScene::make_shared();
    }

    /**
     * this image serves as a placeholder image if no base image is loaded
     */
    static QImage createDefaultImage()
    {
        QPixmap pixmap(400, 300);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBackground(QBrush(Qt::white));
        painter.setBrush(QBrush(Qt::white));
        painter.setPen(QPen(Qt::white));
        painter.drawRect(0, 0, 400, 300);
        //painter.setBrush(QBrush(Qt::lightGray));
        painter.setPen(QPen(Qt::gray));
        painter.setFont(QFont("Sans", 16));
        painter.drawText(QPointF(30.0, 50.0), "No image loaded.");
        return pixmap.toImage();
    }

    bool contentChangedNotificationGroupActive()
    {
        return contentChangedNotificationGroupDepth > 0;
    }

public:
    QImage baseImage;

    KreenGraphicsScenePtr scene = nullptr;

    QUndoStack undoStack;
    int transientContentId = 0; // TMP, todo
    /**
     * greater than 0 if during macro recording mode
     */
    int contentChangedNotificationGroupDepth = 0;
    bool contentChangedNotificationGroupRecordUndo = false;
};

DocumentPtr Document::make_shared(QImage baseImage)
{
    return std::make_shared<Document>(baseImage);
}

Document::Document(QImage baseImage)
{
    KREEN_PIMPL_INIT(Document)

    if (baseImage.isNull()) {
        baseImage = DocumentImpl::createDefaultImage();
    }

    setBaseImage(baseImage, false);
}

Document::~Document()
{

}

QImage Document::baseImage()
{
    return d->baseImage;
}

void Document::setBaseImage(QImage image, bool recordUndo)
{
    if (recordUndo) {
        d->undoStack.push(new SetBaseImageCmd(this, image)); // this will call setBaseImage with recordUndo=false
    }
    else {
        d->baseImage = image;
    }
}

bool Document::isClean()
{
    // TMP, TODO: use QUndoStack
    return d->transientContentId == 0;
}

void Document::setClean()
{
    // TMP, TODO: use QUndoStack
    d->transientContentId = 0;
}

void Document::undo()
{
    contentChangedNotificationGroupBegin(false);
    d->undoStack.undo();
    contentChangedNotificationGroupEnd();
}

void Document::redo()
{
    contentChangedNotificationGroupBegin(false);
    d->undoStack.redo();
    contentChangedNotificationGroupEnd();
}

void Document::contentChangedNotificationGroupBegin(bool recordUndo, QString undoMacroText)
{
    d->contentChangedNotificationGroupDepth++;
    d->contentChangedNotificationGroupRecordUndo = recordUndo;

    if (recordUndo) {
        d->undoStack.beginMacro(undoMacroText);
    }
}

void Document::contentChangedNotificationGroupEnd()
{
    Q_ASSERT(d->contentChangedNotificationGroupDepth > 0);
    d->contentChangedNotificationGroupDepth--;

    if (d->contentChangedNotificationGroupRecordUndo) {
        d->undoStack.endMacro();
        d->contentChangedNotificationGroupRecordUndo = false;
    }

    emit contentChangedSignal();
}

// int Document::contentHashTransient()
// {
//     return d->transientContentId;
// }

void Document::addItem(KreenItemPtr item, bool recordUndo)
{
    if (d->contentChangedNotificationGroupActive()) {
        Q_ASSERT(d->contentChangedNotificationGroupRecordUndo == recordUndo);
    }

    if (recordUndo) {
        d->undoStack.push(new AddItemCmd(this, item)); // this will call addItem with recordUndo=false
    }
    else {
        _items.push_back(item);
    }

    if (!d->contentChangedNotificationGroupActive()) {
        emit contentChangedSignal();
    }
}

void Document::deleteItem(KreenItemPtr item, bool recordUndo)
{
    if (recordUndo) {
        d->undoStack.push(new DeleteItemCmd(this, item));
    }
    else {
        Q_ASSERT(_items.removeOne(item));
    }

    emit contentChangedSignal();
}

void Document::addDemoItems()
{
    d->undoStack.beginMacro("add demo items");

    {
        auto item = KreenItem::create("line");
        item->setLine(QLine(QPoint(10, 20), QPoint(30, 40)));
        addItem(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(30, 30, 40, 30));
        item->lineColor()->color = Qt::darkGreen;
        item->lineStyle()->width = 3; // TODO: if this number is uneven, then the item AND the black selection handles become blurred!
        item->lineStyle()->penStyle = Qt::DotLine;
        addItem(item);
    }

    {
        auto item = KreenItem::create("ellipse");
        item->setRect(QRect(10, 40, 40, 40));
        addItem(item);
    }

    {
        auto item = KreenItem::create("ellipse");
        item->setRect(QRect(10, 80, 70, 40));
        item->lineColor()->color = Qt::blue;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::DashLine;
        addItem(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(0, 0, 10, 10));
        item->lineStyle()->width = 1;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = false;
        addItem(item);
    }


    {
        auto item = KreenItem::create("text");
        item->setRect(QRect(10, 120, 200, 40));
        item->lineColor()->color = Qt::gray;
        item->text = TextProperty::create();
        item->text->text = "Hello from the document";
        addItem(item);
    }

    {
        auto item = KreenItem::create("text");
        item->setRect(QRect(10, 420, 150, 40));
        item->lineColor()->color = Qt::magenta;
        item->text = TextProperty::create();
        item->text->text = "TODO: apply attributes; fill shape; multiline; edit text";
        addItem(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(200, 200, 50, 50));
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(962-10, 556-10, 10, 10));
        addItem(item);
    }

    d->undoStack.endMacro();

    emit contentChangedSignal();
}

void Document::operationCrop(QRect rect)
{
    d->undoStack.beginMacro("Image operation crop");

    setBaseImage(baseImage().copy(rect), true);

    // TODO: undo these translations also!!!
    foreach (auto item, items()) {
        item->translate(-rect.x(), -rect.y());
    }

    d->undoStack.endMacro();

    emit contentChangedSignal();
}

const QList<KreenItemPtr> Document::items()
{
    return _items;
}

KreenGraphicsScenePtr Document::graphicsScene()
{
    return d->scene;
}

QImage Document::renderToImage()
{
    QImage image = baseImage().copy();
    Q_ASSERT_X(!image.isNull(), "renderToImage", "image must not be empty otherwise creation of the painter will fail");
    QPainter painterImage(&image);
    painterImage.setRenderHint(QPainter::Antialiasing);
    d->scene->renderFinalImageOnly(true);
    d->scene->render(&painterImage);
    d->scene->renderFinalImageOnly(false);
    return image;
}

void Document::copyImageToClipboard()
{
    QImage image = renderToImage();
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setImage(image);
}

}
}
