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
#include "myqgraphicsscene.h"
#include "toolmanager.h"

namespace kreen {
namespace core {

class DocumentImpl
{
public:
    DocumentImpl()
    {
        scene = std::make_shared<MyQGraphicsScene>();
    }

public:
    QImage baseImage;

    MyQGraphicsScenePtr scene;

    int transientContentId = 0;
};

DocumentPtr Document::create(QImage baseImage)
{
    Q_ASSERT_X(!baseImage.isNull(), "create", "image must not be empty");
    auto doc = std::make_shared<Document>();
    doc->setBaseImage(baseImage);
    return doc;
}

Document::Document()
{
    KREEN_PIMPL_INIT(Document)
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

void Document::addItem(ItemPtr item)
{
    d->transientContentId++;
    _items.push_back(item);
}

void Document::addDemoItems()
{
    //TODO

    {
        auto item = Item::create("line");
        item->setLine(QLine(QPoint(10, 20), QPoint(30, 40)));
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(30, 30, 40, 30));
        item->lineColor()->color = Qt::darkGreen;
        item->lineStyle()->width = 3;
        item->lineStyle()->penStyle = Qt::DotLine;
        _items.push_back(item);
    }

    {
        auto item = Item::create("ellipse");
        item->setRect(QRect(10, 40, 40, 40));
        _items.push_back(item);
    }

    {
        auto item = Item::create("ellipse");
        item->setRect(QRect(10, 80, 70, 40));
        item->lineColor()->color = Qt::blue;
        item->lineStyle()->width = 2;
        item->lineStyle()->penStyle = Qt::DashLine;
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(0, 0, 10, 10));
        item->lineStyle()->width = 1;
        item->lineStyle()->penStyle = Qt::SolidLine;
        item->dropShadow()->enabled = false;
        _items.push_back(item);
    }


    {
        auto item = Item::create("text");
        item->setRect(QRect(10, 120, 200, 40));
        item->text = TextProperty::create();
        item->text->text = "Hello from the document";
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(200, 200, 50, 50));
        _items.push_back(item);
    }

    {
        auto item = Item::create("rect");
        item->setRect(QRect(962-10, 556-10, 10, 10));
        _items.push_back(item);
    }
}

void Document::operationCrop(QRect rect)
{
    setBaseImage(baseImage().copy(rect));

    foreach (auto item, items()) {
        item->setRect(item->rect().translated(-rect.x(), -rect.y())); // TODO: this should also work for lines => add a "translate" method
    }
}

const std::vector<ItemPtr>& Document::items()
{
    return _items;
}

MyQGraphicsScenePtr Document::graphicsScene()
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

}
}
