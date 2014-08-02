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

public:
    QImage baseImage;

    KreenGraphicsScenePtr scene;

    QUndoStack undoStack;
    int transientContentId = 0; // TMP, todo
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

    setBaseImage(baseImage);
}

Document::~Document()
{

}

QImage Document::baseImage()
{
    return d->baseImage;
}

void Document::setBaseImage(QImage image)
{
    d->baseImage = image;
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
    d->undoStack.undo();
}

void Document::redo()
{
    d->undoStack.redo();
}

// void Document::undoMacroBegin(QString text)
// {
//     d->undoStack.beginMacro(text);
// }
//
// void Document::undoMacroEnd()
// {
//     d->undoStack.endMacro();
// }

// int Document::contentHashTransient()
// {
//     return d->transientContentId;
// }

void Document::addItem(KreenItemPtr item, bool recordUndo)
{
    if (recordUndo) {
        d->undoStack.push(new AddItemCmd(this, item)); // this will call addItem with recordUndo=false
    }
    else {
        _items.push_back(item);
    }

    emit contentChangedSignal();
}

void Document::removeItems(const QList< kreen::core::KreenItemPtr > items, bool recordUndo)
{
    if (recordUndo) {
        d->undoStack.push(new DeleteItemsCmd(this, items));
    }
    else {
        foreach (auto item, items) {
            Q_ASSERT(_items.removeOne(item));
        }
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
        addItem(item);
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
    setBaseImage(baseImage().copy(rect));

    foreach (auto item, items()) {
        item->translate(-rect.x(), -rect.y());
    }

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
