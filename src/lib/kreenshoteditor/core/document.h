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
#ifndef CORE_DOCUMENT_H
#define CORE_DOCUMENT_H

#include <kreen/util/pimplutil.h>
#include "item.h"

#include <vector>
#include <memory>
#include <QImage>

namespace kreen {
namespace core {

KREEN_PIMPL_FORWARD_DECL(Document)

KREEN_SHAREDPTR_FORWARD_DECL(Document)
KREEN_SHAREDPTR_FORWARD_DECL(MyQGraphicsScene)
KREEN_SHAREDPTR_FORWARD_DECL(ToolManager)

/**
 * The "document"
 * Contains all items like Rects, Ellipses, Text, Blurring etc.
 * Manages Z-Order.
 */
class Document
{
public:
    /**
     * TODO
     */
    static DocumentPtr create(QImage baseImage);

public:
    Document();

    virtual ~Document();

    /**
     * see QUndoStack
     *
     * Can be used in the Undo/redo scenarios to
     * determine if the stored document (DocumentFile) has to be saved.
     */
    bool isClean();

    /**
     * see QUndoStack
     */
    void setClean();

    /**
     * Returns the base image (without any items).
     * TODO: currently after a crop operation the cropped image will be returned. Is this ok? Yes, remove this comment after refac
     */
    QImage baseImage();

    /**
     * TODO: put to undostack
     */
    void setBaseImage(QImage image);

    /**
     * TODO: put to undostack
     */
    void addItem(ItemPtr item);

    /**
     * todo: remove later
     */
    void addDemoItems();

    void operationCrop(QRect rect);

    const std::vector<ItemPtr>& items();

    /**
     * TODO: possible to move this away since it looks so like "UI" (and actually came from there but we would like to able to paint the scene here)?
     */
    MyQGraphicsScenePtr graphicsScene();

    /**
     * TODO: possible to move this away since it looks so like "UI" (and actually came from there but we would like to able to paint the scene here)?
     */
    ToolManagerPtr toolManager();

    /**
     * renders the document to a QImage
     */
    QImage renderToImage();

private:
    std::vector<ItemPtr> _items;

private:
    DocumentImplPtr d;
};

}
}

#endif // CORE_DOCUMENT_H
