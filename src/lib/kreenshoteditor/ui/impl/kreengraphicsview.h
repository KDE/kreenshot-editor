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
namespace core {
    KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)
}

namespace ui {

KREEN_SHAREDPTR_FORWARD_DECL(KreenGraphicsView)

class KreenGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    KreenGraphicsView(kreen::core::ToolManagerPtr toolmanager);

    /**
     * impl detail for cursor handling
     */
    void setHelperBaseImageItem(QGraphicsItem* helperBaseImageItem);

    void setCursorFromChosenTool(QPoint* pos = nullptr);

protected:
    virtual void enterEvent(QEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    kreen::core::ToolManagerPtr _toolManager;
    QGraphicsItem* _helperBaseImageItem;
};

}
}

#endif // MYQGRAPHICSVIEW

// kate: indent-mode cstyle; replace-tabs on;
