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
#include "impl/kreengraphicsscene.h"
#include "impl/toolmanager.h"

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

    int transientContentId = 0;
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

// int Document::contentHashTransient()
// {
//     return d->transientContentId;
// }

void Document::addItem(KreenItemPtr item)
{
    d->transientContentId++;
    _items.push_back(item);
}

void Document::removeItems(const QList<KreenItemPtr> items)
{
    foreach (auto item, items) {
        d->transientContentId++;
        _items.removeAll(item);
    }
}

void Document::addDemoItems()
{
    d->transientContentId++;

    //TODO

    {
        auto item = KreenItem::create("line");
        item->setLine(QLine(QPoint(10, 20), QPoint(30, 40)));
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(30, 30, 40, 30));
        item->lineColor()->color = Qt::darkGreen;
        item->lineStyle()->width = 3; // TODO: if this number is uneven, then the item AND the black selection handles become blurred!
        item->lineStyle()->penStyle = Qt::DotLine;
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("ellipse");
        item->setRect(QRect(10, 40, 40, 40));
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("ellipse");
        item->setRect(QRect(10, 80, 70, 40));
        item->lineColor()->color = Qt::blue;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::DashLine;
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(0, 0, 10, 10));
        item->lineStyle()->width = 1;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = false;
        _items.push_back(item);
    }


    {
        auto item = KreenItem::create("text");
        item->setRect(QRect(10, 120, 200, 40));
        item->lineColor()->color = Qt::gray;
        item->text = TextProperty::create();
        item->text->text = "Hello from the document";
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("text");
        item->setRect(QRect(10, 420, 150, 40));
        item->lineColor()->color = Qt::magenta;
        item->text = TextProperty::create();
        item->text->text = "TODO: apply attributes; fill shape; multiline; edit text";
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(200, 200, 50, 50));
        _items.push_back(item);
    }

    {
        auto item = KreenItem::create("rect");
        item->setRect(QRect(962-10, 556-10, 10, 10));
        _items.push_back(item);
    }
}

void Document::operationCrop(QRect rect)
{
    setBaseImage(baseImage().copy(rect));

    foreach (auto item, items()) {
        item->translate(-rect.x(), -rect.y());
    }
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
