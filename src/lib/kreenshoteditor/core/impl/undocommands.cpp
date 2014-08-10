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
#include "undocommands.h"
#include <kreen/core/document.h>

namespace kreen {
namespace core {

/////////////////////////////////////////////////////////////////////

AddItemCmd::AddItemCmd(kreen::core::Document* document, KreenItemPtr item) : KreenUndoCmd(document)
{
    _item = item->deepCopy();
}

void AddItemCmd::redo()
{
    _document->addItem(_item, false);
}

void AddItemCmd::undo()
{
    _document->deleteItem(_item, false);
}

/////////////////////////////////////////////////////////////////////

DeleteItemCmd::DeleteItemCmd(Document* document, KreenItemPtr item): KreenUndoCmd(document)
{
    _item = item->deepCopy();
}

void DeleteItemCmd::redo()
{
    _document->deleteItem(_item, false);
}

void DeleteItemCmd::undo()
{
    _document->addItem(_item, false);
}

/////////////////////////////////////////////////////////////////////

SetBaseImageCmd::SetBaseImageCmd(Document* document, QImage image): KreenUndoCmd(document)
{
    _image = image;
}

void SetBaseImageCmd::redo()
{
    _origImage = _document->baseImage();
    Q_ASSERT(!_origImage.isNull());
    _document->setBaseImage(_image, false);
}

void SetBaseImageCmd::undo()
{
    Q_ASSERT(!_origImage.isNull());
    _document->setBaseImage(_origImage, false);
}

/////////////////////////////////////////////////////////////////////

ApplyItemPropertyChangesCmd::ApplyItemPropertyChangesCmd(Document* document,
                                                         const KreenItemPtr item,
                                                         const KreenItemPtr oldItem
                                                        ): KreenUndoCmd(document)
{
    _item = item->deepCopy();
    _oldItem = oldItem->deepCopy();
}

void ApplyItemPropertyChangesCmd::redo()
{
    _document->applyItemPropertyChanges(_item, false);
}

void ApplyItemPropertyChangesCmd::undo()
{
    _document->applyItemPropertyChanges(_oldItem, false);
}

/////////////////////////////////////////////////////////////////////

ImageOperationCropCmd::ImageOperationCropCmd(Document* document, QRect rect): KreenUndoCmd(document)
{
    _rect = rect;
}

void ImageOperationCropCmd::redo()
{
    //_document->imageOpCrop(_rect, false); // todo

    // TODO: first implement MoveItemCmd!!!
}

void ImageOperationCropCmd::undo()
{
    // TODO
}

/////////////////////////////////////////////////////////////////////

}
}
