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

namespace kreen {
namespace core {

void KreenGraphicsRectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (!getScene(this)->renderVisibilityControl()->onPaintEnter(this)) {
        return;
    }

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
    qDebug() << "KreenGraphicsEllipseItem::paint";
    if (!getScene(this)->renderVisibilityControl()->onPaintEnter(this)) {
        return;
    }

    // TODO see Rect: reimpl this method to suppress selection marquee
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void KreenGraphicsObfuscateItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (!getScene(this)->renderVisibilityControl()->onPaintEnter(this)) {
        return;
    }

//     if (painterImage.isActive()) {
//         //qDebug() << "return";
//         return;
//     }

//     if (!painter->isActive()) {
//         return;
//     }

    // see also KreenGraphicsRectItem

    Q_UNUSED(widget);

    auto kScene = getScene(this);
    auto r = rect();
    qreal x = pos().x();
    qreal y = pos().y();
    qreal w = r.width();
    qreal h = r.height();
    int pixSize = 4;
    qreal pixW = ceil(w / pixSize);
    qreal pixH = ceil(h / pixSize);
    //qDebug() << "w, h" << w << h;

    // http://developer.nokia.com/community/wiki/QPainter::begin:Paint_device_returned_engine_%3D%3D_0_%28Known_Issue%29
    if (w == 0 || h == 0) {
        return;
    }

    QPixmap image(w, h);
    image.fill(Qt::white);

    //qDebug() << &image;
    //qDebug() << painter->device();

    //qDebug() << "go";

    {
        QPainter painterImage(&image);
        painterImage.setRenderHint(QPainter::Antialiasing);
        //sc->renderFinalImageOnly(true);

        kScene->renderVisibilityControl()->pushPaintUpToItem(this);

        //QGraphicsView view1(sc);
        const QRectF targetRect(0, 0, w, h);
        const QRectF sourceRect(x, y, w, h);
        //view1.render(&painterImage, targetRect.toRect(), sourceRect.toRect(), Qt::KeepAspectRatio);
        kScene->render(&painterImage, targetRect, sourceRect);

        kScene->renderVisibilityControl()->popPaintUpToItem();

        painterImage.drawEllipse(0, 0, w, h);
        //sc->renderFinalImageOnly(false);
    }

    // TODO: disable selection handles
    // TODO: if z-order of Item is greater (on top) of the current item do not draw it because result would be undefined

    QPixmap subPixmap = image.copy();
    QPainter painter2(&subPixmap);
    painter2.drawPixmap(QRect(0, 0, pixW, pixH), image, QRect(0, 0, w, h)); // pixelize: make small
    QPixmap subPixmap2 = subPixmap.copy();
    painter2.drawPixmap(QRect(0.0, 0.0, w, h), subPixmap2, QRect(0, 0, pixW, pixH)); // pixelize: make big from small
    painter->drawPixmap(0, 0, subPixmap);

    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(rect());

    //painter->drawPixmap(0, 0, image);
}

}
}
