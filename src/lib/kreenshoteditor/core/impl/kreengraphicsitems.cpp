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
#include "kreengraphicsitems.h"
#include "kreengraphicsscene.h"
#include "rendervisibilitycontrol.h"
#include <QGraphicsView>
#include <QLabel>

namespace kreen {
namespace core {

void KreenGraphicsRectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // todo remove later
//     if (!getScene()->renderVisibilityControl()->onPaintEnter(this)) {
//         return;
//     }

    // QGraphicsRectItem::paint(painter, option, widget);
    // see src/qt5/qtbase/src/widgets/graphicsview/qgraphicsitem.cpp

    Q_UNUSED(widget);
    //painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(rect());
    //painter->restore();

    // omit the selected rect because we draw it ourselves
}

void KreenGraphicsEllipseItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // todo remove later
//     if (!getScene()->renderVisibilityControl()->onPaintEnter(this)) {
//         return;
//     }

    // TODO see Rect: reimpl this method to suppress selection marquee
    QGraphicsEllipseItem::paint(painter, option, widget);
}

//int NOT_RENDER = 0; // almost works

//
// see commit d1021d6329680c6c67c2a78d30723538f18ff876 for some experiments
//
void KreenGraphicsObfuscateItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget);

//     if (!getScene()->renderVisibilityControl()->onPaintEnter(this)) {
//         return;
//     }

    auto kScene = getScene();

//     if (NOT_RENDER > 0) return;
//     qDebug() << NOT_RENDER;
//     NOT_RENDER++;
//     kScene->renderVisibilityControl()->pushPaintUpToItem(this);
//     auto TESTIMAGE = kScene->document()->renderToImage(this);
//     kScene->renderVisibilityControl()->popPaintUpToItem();
//     NOT_RENDER--;

    auto r = rect();
    qreal x = pos().x();
    qreal y = pos().y();
    //qDebug() << "x, y" << x << y;
    qreal w = r.width();
    qreal h = r.height();
    int pixSize = 8;
    qreal pixW = ceil(w / pixSize);
    qreal pixH = ceil(h / pixSize);

    // http://developer.nokia.com/community/wiki/QPainter::begin:Paint_device_returned_engine_%3D%3D_0_%28Known_Issue%29
    if (w == 0 || h == 0) {
        return;
    }

    QPixmap baseImageForItem(w, h); // will hold the base image cutout suitable for the current obfuscate item
    baseImageForItem.fill(Qt::white);

    {
        QPainter imagePainter(&baseImageForItem);
        imagePainter.setRenderHint(QPainter::Antialiasing);
        imagePainter.drawImage(QRectF(0, 0, w, h), kScene->document()->baseImage(), QRectF(x, y, w, h));
        //imagePainter.drawImage(QRectF(0, 0, w, h), TESTIMAGE, QRectF(x, y, w, h)); // todo: try later
    }

    {
        QPixmap itemImage = baseImageForItem.copy();

        if (pixelizeEffect) {
            QPainter imagePainter(&itemImage);
            imagePainter.drawPixmap(QRect(0, 0, pixW, pixH), baseImageForItem, QRect(0, 0, w, h)); // pixelize: make small
            QPixmap itemImageSmall = itemImage.copy();
            imagePainter.drawPixmap(QRect(0.0, 0.0, w, h), itemImageSmall, QRect(0, 0, pixW, pixH)); // pixelize: make big from small
        }
        else {
            // is done implicitly by the set Blur effect
        }

        painter->drawPixmap(0, 0, itemImage);
    }

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(rect());
}

}
}
