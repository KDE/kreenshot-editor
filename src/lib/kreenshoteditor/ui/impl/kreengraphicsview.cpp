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
#include "kreengraphicsview.h"
#include "../../core/impl/toolmanager.h"

namespace kreen {
namespace ui {

using namespace kreen::core;

KreenGraphicsView::KreenGraphicsView(ToolManagerPtr toolmanager)
{
    _toolManager = toolmanager;

    setRenderHints(QPainter::Antialiasing/* | QPainter::SmoothPixmapTransform*/);

    // "the alignment of the scene in the view when the whole scene is visible."
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // see overriden method drawForeground
    //graphicsView->setBackgroundBrush(QBrush(Qt::BDiagPattern));
    //d->graphicsView->setBackgroundBrush(QBrush(Qt::lightGray, Qt::DiagCrossPattern));
}

void KreenGraphicsView::setHelperBaseImageItem(QGraphicsItem* helperBaseImageItem)
{
    _helperBaseImageItem = helperBaseImageItem;
}


void KreenGraphicsView::setCursorFromChosenTool(QPoint* pos, bool leftButtonDown)
{
    Q_ASSERT(_helperBaseImageItem != nullptr); // TODO: also verify if _helperBaseImageItem exists in scene

    if (pos != nullptr) { // if pos is given
        auto item = itemAt(*pos);
        if (item != _helperBaseImageItem) // if mouse over any item or no item except the background...
            return; // ... do not try to set the cursor
    }

    if (leftButtonDown) { // if left button is down then an item is probably moved and we do not want to change the cursor
        return;
    }

    // workaround (not really) for https://bugreports.qt-project.org/browse/QTBUG-4190
    QWidget* w = viewport();
    QCursor curCursor = w->cursor();
    Qt::CursorShape newCursorShape;
    auto tool = _toolManager->chosenTool();

    if (tool == Select) {
        newCursorShape = Qt::ArrowCursor;
    }
    else if (tool == DrawRect) {
        newCursorShape = Qt::CrossCursor;
    }
    else if (tool == DrawLine) {
        newCursorShape = Qt::CrossCursor;
    }
    else if (tool == DrawEllipse) {
        newCursorShape = Qt::CrossCursor;
    }
    else if (tool == DrawText) {
        newCursorShape = Qt::CrossCursor;
    }
    else if (tool == OperationCrop) {
        newCursorShape = Qt::CrossCursor;
    }
    else {
        qDebug() << "_chosenTool" << tool;
        Q_ASSERT(false);
    }

    if (curCursor.shape() != newCursorShape) {
        auto tool = _toolManager->chosenTool();
        qDebug() << QTime::currentTime() << " setCursor for " << tool;
        w->setCursor(newCursorShape);
    }
}

void KreenGraphicsView::enterEvent(QEvent* event)
{
    qDebug() << "KreenGraphicsView::enterEvent";
    // for not to have to click once before one can start moving items
    this->setFocus();
    QGraphicsView::enterEvent(event);
}

void KreenGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    // makes sure that the cursor is always right
    // NOTE: when an image operation (like crop) is active and
    //  there is a WidgetProxy active the mouse movement on the proxy widget
    //  is caught and the cursor is tried to be change, it does NOT change
    //  which is ok (but there is massive console output from the qDebug()).
    //  But it might be the cause why we need to set the cursor
    //  here to have it reset to Arrow after Crop is finished and Select is
    //  chosen again.
    // So probably cause for illbehaved cursor is the QGraphicsProxyWidget.
    QPoint pos = event->pos();
    setCursorFromChosenTool(&pos, event->button() | Qt::LeftButton);

    QGraphicsView::mouseMoveEvent(event);
}

void KreenGraphicsView::drawForeground(QPainter* painter, const QRectF& rect)
{
    // WORKAROUND: clip painting outside the sceneRect
    // side effect: nice dimming of outside items which makes it possible to get them even if completely "hidden"

//     painter->setClipping(true);
//     painter->setClipRect(sceneRect());
    QRectF sc = sceneRect();
    int maxScreenWidth = 5000;
    QRectF r1(sc.width(), 0, maxScreenWidth, maxScreenWidth);
    QRectF r2(0, sc.height(), sc.width(), maxScreenWidth);
    painter->save();
    //painter->setBrush(Qt::white);
    painter->setBrush(QColor(255, 255, 255, 215)); // dim everything which is outside the sceneRect
    painter->setPen(Qt::PenStyle::NoPen);
    // draw with dimming color:
    painter->drawRect(r1);
    painter->drawRect(r2);
    painter->setBrush(QBrush(Qt::lightGray, Qt::DiagCrossPattern));
    // draw again with cross pattern:
    painter->drawRect(r1);
    painter->drawRect(r2);
    painter->restore();

    QGraphicsView::drawForeground(painter, rect); // draw foreground if it is set
}


}
}

