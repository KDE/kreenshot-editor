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
#ifndef UUID_689145c41a7e11e4b715002454dd224f
#define UUID_689145c41a7e11e4b715002454dd224f

#include "../../util/sharedptrutil.h"
#include <QUndoCommand>
#include <QImage>

/**
 * This file contains several QUndoCommand derivates
 */
namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(Document)
KREEN_SHAREDPTR_FORWARD_DECL(KreenItem)

/**
*
*/
class KreenUndoCmd : public QUndoCommand
{
public:
    KreenUndoCmd(Document* document = nullptr)
    {
        _document = document;
    }

protected:
    Document* _document;
};

class AddItemCmd : public KreenUndoCmd
{
public:
    AddItemCmd(Document* document, KreenItemPtr item);
    virtual void redo();
    virtual void undo();
private:
    KreenItemPtr _item;
};

class DeleteItemCmd : public KreenUndoCmd
{
public:
    DeleteItemCmd(Document* document, const KreenItemPtr item);
    virtual void redo();
    virtual void undo();
private:
    KreenItemPtr _item;
};

class SetBaseImageCmd : public KreenUndoCmd
{
public:
    SetBaseImageCmd(Document* document, QImage image);
    virtual void redo();
    virtual void undo();
private:
    QImage _image;
    QImage _origImage;
};

class ApplyItemPropertyChangesCmd : public KreenUndoCmd
{
public:
    ApplyItemPropertyChangesCmd(Document* document, const KreenItemPtr item, const KreenItemPtr oldItem);
    virtual void redo();
    virtual void undo();
private:
    KreenItemPtr _item;
    KreenItemPtr _oldItem;
};

class ImageOperationCropCmd : public KreenUndoCmd
{
public:
    ImageOperationCropCmd(Document* document, QRect rect);
    virtual void redo();
    virtual void undo();
private:
    QRect _rect;
};

}
}

#endif

// kate: indent-mode cstyle; replace-tabs on;
