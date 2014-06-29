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
#ifndef UI_TOOLMANAGER_H
#define UI_TOOLMANAGER_H

#include <kreen/util/exportutil.h>
#include <util/sharedptrutil.h>
#include <memory>
#include "kreengraphicsitems.h"

namespace kreen {
namespace core {

enum ToolEnum
{
    Select = 0,
    DrawRect,
    DrawEllipse,
    DrawLine,
    DrawText,
    // ...
    OperationCrop
};

KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)

class ToolManager
{
public:
    static QGraphicsItem* createGraphicsItemFromKreenItem(ItemPtr item, QGraphicsScene* scene)
    {
        // create items
        //
        if (item->typeId == "rect") {
            auto kgrItem = new KreenQGraphicsRectItem(item, scene);
            return kgrItem;
        }
        else if (item->typeId == "line") {
            auto kgrItem = new KreenQGraphicsLineItem(item, scene);
            return kgrItem;
        }
        else if (item->typeId == "ellipse") {
            auto kgrItem = new KreenQGraphicsEllipseItem(item, scene);
            return kgrItem;
        }
        else if (item->typeId == "text") {
            auto kgrItem = new KreenGraphicsTextRectItem(item, scene);
            return kgrItem;
        }
        else if (item->typeId == "op-crop") {
            auto kgrItem = new KreenGraphicsOperationCropItem(item, scene);
            return kgrItem;
        }
        else {
            qDebug() << "unknown item->typeId: " << item->typeId;
            Q_ASSERT(false);
        }
    }

public:
    ToolEnum chosenTool()
    {
        return _chosenTool;
    }

    void setChosenTool(ToolEnum tool, QWidget* parent)
    {
        _chosenTool = tool;
    }

    bool isImageOperationActive()
    {
        return _isImageOperationActive;
    }

    void setImageOperationActive(bool active)
    {
        _isImageOperationActive = active;
    }

private:
    ToolEnum _chosenTool;

    bool _isImageOperationActive = false;
};

}
}

#endif // UI_KREENQGRAPHICSITEMS_H

// kate: indent-mode cstyle; replace-tabs on;
