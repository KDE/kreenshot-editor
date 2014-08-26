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
#include "selectionhandles.h"
#include <QRect>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "kreengraphicsitembase.h" // todo: remove this dependency if possible
#include "selectionhandlegraphicsitem.h"

namespace kreen {
namespace core {

class SelectionHandles::Impl
{
public:
    SelectionHandles* owner = nullptr;
    QGraphicsScene* scene = nullptr;
    std::map<QGraphicsItem*, std::vector<SelectionHandleGraphicsItem*>> currentHandles;

public:
    Impl(SelectionHandles* owner_)
    {
        owner = owner_;
    }

    /**
     * creates a new SelectionHandleGraphicsItem including cursor and move event handling
     *
     * TODO: let the SelectionHandleGraphicsItem handle the cursors
     *        according to the PositionType (TopLeft, TopBorder, ... see Handle from tikzkit
     */
    SelectionHandleGraphicsItem* createSelectionHandleItem(QGraphicsItem* instrumentedItem, QRectF rect, const QCursor& cursor)
    {
        auto grItem = new SelectionHandleGraphicsItem(instrumentedItem, rect);
        grItem->setBrush(QBrush(Qt::black));
        grItem->setPen(Qt::NoPen);
        grItem->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
        grItem->setFlag(QGraphicsItem::ItemIsMovable, true);
        // qDebug() << "newRectItemWithCursor, setCursor";
        grItem->setCursor(cursor);

        // connect(grItem, itemPositionHasChangedSignal, owner, // TODO later or go directly to the instrumentedItem

        return grItem;
    }
};


SelectionHandles::SelectionHandles(QGraphicsScene* scene) {
    qDebug() << "SelectionHandles::ctor";
    KREEN_PIMPL_INIT_THIS(SelectionHandles);
    d->scene = scene;
}

SelectionHandles::~SelectionHandles()
{
    qDebug() << "SelectionHandles destructor";
}


void SelectionHandles::redrawSelectionHandles(bool createNewHandles)
{
    if (createNewHandles) {
        foreach (auto grItemPair, d->currentHandles) {
            foreach (auto grItem, grItemPair.second) {
                d->scene->removeItem(grItem);
            }
        }

        d->currentHandles.clear();
    }

    const qreal handleWidth = 10.0;

    //
    // 1   2   3
    // 4   5   6
    // 7   8   9
    //
    std::vector<QCursor> cursors;
    cursors.push_back(Qt::ArrowCursor); // not used
    cursors.push_back(Qt::SizeFDiagCursor); // 1
    cursors.push_back(Qt::SizeVerCursor); // 2
    cursors.push_back(Qt::SizeBDiagCursor); // 3
    cursors.push_back(Qt::SizeHorCursor); // 4
    cursors.push_back(Qt::OpenHandCursor); // 5
    cursors.push_back(Qt::SizeHorCursor); // 6
    cursors.push_back(Qt::SizeBDiagCursor); // 7
    cursors.push_back(Qt::SizeVerCursor); // 8
    cursors.push_back(Qt::SizeFDiagCursor); // 9

    foreach (auto grItem, d->scene->selectedItems()) {

        // TODO ................. DO THIS AS SOON A HANDLE IS CLICKED
        //graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, false);
        // ......... and undo it after a handle operation is complete

        qreal hw = handleWidth;

        // WORKAROUND:
        // handle width, TODO: why? even or uneven numbers: these or those items will have blurred rects
        //                     the underlying item is also blurred
        //                     WTF-->the black selection rects get also blurred
        auto kGrItem = dynamic_cast<KreenGraphicsItemBase*>(grItem);
        if (kGrItem->workaroundIsBlurredOnUnevenHandleWidth()) {
            qDebug() << "INFO: workaround used";
            hw--;
        }

        qreal hw2 = hw / 2.0;

        auto baseRect = grItem->sceneBoundingRect();
        qreal x = baseRect.x();
        qreal y = baseRect.y();
        qreal w = baseRect.width();
        qreal h = baseRect.height();
        qreal w2 = floor(w / 2.0);
        qreal h2 = floor(h / 2.0);
        auto rect = QRectF(0, 0, hw, hw);

        //
        // 1   2   3
        // 4   5   6
        // 7   8   9
        //
        auto r1 = rect.translated(x - hw2, y - hw2); // 1
        auto r2 = rect.translated(x + w2 - hw2, y - hw2); // 2
        auto r3 = rect.translated(x + w - hw2, y - hw2); // 3

        auto r4 = rect.translated(x - hw2, y + h2 - hw2); // 4
        auto r6 = rect.translated(x + w - hw2, y + h2 - hw2); // 6

        auto r7 = rect.translated(x - hw2, y + h - hw2); // 7
        auto r8 = rect.translated(x + w2 - hw2, y + h - hw2); // 8
        auto r9 = rect.translated(x + w - hw2, y + h - hw2); // 9


        if (createNewHandles) {
            std::vector<SelectionHandleGraphicsItem*> handles;
            handles.push_back(d->createSelectionHandleItem(grItem, r1, cursors[1]));
            handles.push_back(d->createSelectionHandleItem(grItem, r2, cursors[2]));
            handles.push_back(d->createSelectionHandleItem(grItem, r3, cursors[3]));

            handles.push_back(d->createSelectionHandleItem(grItem, r4, cursors[4]));
            handles.push_back(d->createSelectionHandleItem(grItem, r6, cursors[6]));

            handles.push_back(d->createSelectionHandleItem(grItem, r7, cursors[7]));
            handles.push_back(d->createSelectionHandleItem(grItem, r8, cursors[8]));
            handles.push_back(d->createSelectionHandleItem(grItem, r9, cursors[9]));

            d->currentHandles.insert(std::make_pair(grItem, handles));
        }
        else {
            std::vector<SelectionHandleGraphicsItem*> handles = d->currentHandles[grItem];

            int i = 0;
            handles[i++]->setRect(r1);
            handles[i++]->setRect(r2);
            handles[i++]->setRect(r3);
            handles[i++]->setRect(r4);
            handles[i++]->setRect(r6);
            handles[i++]->setRect(r7);
            handles[i++]->setRect(r8);
            handles[i++]->setRect(r9);
        }
    }

    if (createNewHandles) {
        foreach (auto grItemPair, d->currentHandles) {
            foreach (auto grItem, grItemPair.second) {
                d->scene->addItem(grItem);
            }
        }
    }
}

}
}
