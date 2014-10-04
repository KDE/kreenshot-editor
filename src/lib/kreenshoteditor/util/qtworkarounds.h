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

/*
 * This file collects all kinds of workaround code of the QT framework
 * which hopefully we be fixed sooner or later.
 * Naturally this can only happen if the underlying bugs are reported back to the QT guys.
 */
#ifndef QTWORKAROUNDS_H
#define QTWORKAROUNDS_H

#include <QApplication>
#include <QGraphicsSceneContextMenuEvent>

class WORKAROUNDS {
public:
    /**
     * code taken from main.cpp of this bug: https://bugreports.qt-project.org/browse/QTBUG-21943
     * bug also reported at the same spot, see also my comment there about jumping items
     */
    static void sendFakeMouseEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent)
    {
        // Sending a mouse move seems to workarount the Qt bug
        QPoint p = contextMenuEvent->widget()->mapFromGlobal(contextMenuEvent->screenPos());
        QMouseEvent mouseEvent(QEvent::MouseMove, p, Qt::NoButton, Qt::NoButton, contextMenuEvent->modifiers());
        QApplication::sendEvent(contextMenuEvent->widget(), &mouseEvent);
    }
};

#endif // QTWORKAROUNDS_H
