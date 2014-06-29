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

KreenGraphicsView::KreenGraphicsView(kreen::core::ToolManagerPtr toolmanager)
{
    _toolManager = toolmanager;
    setRenderHints(QPainter::Antialiasing/* | QPainter::SmoothPixmapTransform*/);
}

void KreenGraphicsView::setCursorFromChosenTool()
{
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
    // for not to have to click once before one can start moving items
    this->setFocus();
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
    setCursorFromChosenTool();

    QGraphicsView::mouseMoveEvent(event);
}

}
}

