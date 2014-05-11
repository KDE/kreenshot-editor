/*
 * This file is part of the kreenshot-editor project.
 *
 * Copyright (C) 2014 by Gregor Mi <codeminister@publicstatic.de>
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

#include <memory>

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
    ToolEnum chosenTool;

    bool isImageOperationActive = false;
};

typedef std::shared_ptr<ToolManager> ToolManagerPtr;

#endif // UI_KREENQGRAPHICSITEMS_H

// kate: indent-mode cstyle; replace-tabs on;
