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
    DrawObfuscate,
    OperationCrop
};

KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)

class ToolManager
{
public:
    /**
     * todo: return value could also be QGraphicsItem*
     */
    static KreenGraphicsItemBase* createGraphicsItemFromKreenItem(KreenItemPtr item)
    {
        KreenGraphicsItemBase* kGrItem;

        // create items
        //
        if (item->typeId == "rect") {
            kGrItem = new KreenGraphicsRectItem(item);
            return kGrItem;
        }
        else if (item->typeId == "line") {
            kGrItem = new KreenGraphicsLineItem(item);
            return kGrItem;
        }
        else if (item->typeId == "ellipse") {
            kGrItem = new KreenGraphicsEllipseItem(item);
            return kGrItem;
        }
        else if (item->typeId == "text") {
            kGrItem = new KreenGraphicsTextRectItem(item);
            return kGrItem;
        }
        else if (item->typeId == "obfuscate") {
            kGrItem = new KreenGraphicsObfuscateItem(item);
            return kGrItem;
        }
        else if (item->typeId == "op-crop") {
            kGrItem = new KreenGraphicsOperationCropItem(item);
            return kGrItem;
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

#endif

// kate: indent-mode cstyle; replace-tabs on;
