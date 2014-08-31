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
#ifndef UI_MYQGRAPHICSVIEW_H
#define UI_MYQGRAPHICSVIEW_H

#include <kreen/util/sharedptrutil.h>
#include <QWidget>
#include <QTime>
#include <QGraphicsView>
#include <QPoint>
#include <memory>

namespace kreen {
namespace ui {
KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)
KREEN_SHAREDPTR_FORWARD_DECL(KreenGraphicsView)

class KreenGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    KreenGraphicsView(ToolManagerPtr toolmanager);

    /**
     * impl detail for cursor handling
     */
    void setHelperBaseImageItem(QGraphicsItem* helperBaseImageItem);

    /**
     * WORKAROUND method to correctly set mouse cursor
     */
    void setCursorFromChosenTool(QPoint* pos = nullptr, bool leftButtonDown = false);

protected:
    /**
     * used for WORKAROUND, see impl
     */
    virtual void enterEvent(QEvent* event) override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual void drawForeground(QPainter * painter, const QRectF & rect) override;

private:
    ToolManagerPtr _toolManager;
    QGraphicsItem* _helperBaseImageItem;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
