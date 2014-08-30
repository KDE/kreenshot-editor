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

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include <kreen/core/kreenitem.h>

#include <vector>
#include <memory>
#include <QImage>

namespace kreen {
namespace core {

KREEN_SHAREDPTR_FORWARD_DECL(Document)

/**
 * The "document"
 * Contains all items like Rects, Ellipses, Text, Blurring etc.
 * Manages Z-Order.
 */
class KREEN_DECL_EXPORT Document : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new document.
     * If baseImage isNull then a default image is used as baseImage.
     * The document will be "clean".
     */
    static DocumentPtr make_shared(QImage baseImage = QImage());

public:
    /**
     * see make_shared
     */
    Document(QImage baseImage = QImage());

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

    void undo();

    void redo();

    bool canUndo();

    bool canRedo();

    /**
     * Use it to suppress emitting of contentChangedSignal during
     * a composite operation.
     * The contentChangedSignal will not be called until contentChangedNotificationGroupEnd() is called.
     */
    void contentChangedNotificationGroupBegin(bool recordUndo, QString undoMacroText = QString());

    /**
     * See contentChangedNotificationGroupBegin(). Emits contentChangedSignal()
     */
    void contentChangedNotificationGroupEnd();

    /**
     * Returns the base image (without any items).
     * Note that after a crop operation or any other "image operation" the baseImage is changed
     */
    QImage baseImage();

    /**
     * Sets the background image (base image).
     * The contentChangedSignal will NOT be called. Use macroBegin/End if you need it to be called.
     */
    void setBaseImage(QImage image, bool recordUndo);

    /**
     * Adds the given item to the Document.
     * A unique id will be generated and be available through the id() method.
     * If the id of the given item is not the default (-1) an assertion will fail.
     * But if recordUndo is false the id will be used to replace the given item.
     */
    void addItem(KreenItemPtr item, bool recordUndo = true);

    /**
     * Deletes the item from the Document.
     * The item is identified by id().
     * Differences in the content of the document's and the given item will be ignored (for now).
     */
    void deleteItem(KreenItemPtr item, bool recordUndo = true);

    /**
     * returns true if the given item's properties have changed compared
     * to the stored one (identified by id()) in the document
     */
    bool hasItemPropertiesChanged(KreenItemPtr item);

    /**
     * Call this if one or several properties of a KreenItem has changed
     * to apply them to the document content and to register it for undo/redo.
     *
     * This is needed because the document keeps secret copies of all added items
     * so that changes to the items from the outside have no effect on the
     * document's content.
     *
     * Return false if new and old item were different.
     * Returns false if new and old item were equal and the operation was canceled.
     * TODO: return value not used; remove it?
     */
    bool applyItemPropertyChanges(KreenItemPtr item, bool recordUndo = true);

    /**
     * Operates on the base image:
     * keeps only the given @param rect and moves all items accordingly.
     * Records undo by default.
     */
    void imageOpCrop(QRect rect);

    /**
     * (this is treated as one undo operation)
     */
    void addDemoItems();

    const QList<KreenItemPtr> items();

    /**
     * Renders the document to a QImage omitting all graphical elements
     * like selection rects etc.
     * Used to save to the document to an image file.
     * NOTE: this method makes use of decoupled QEventLoop calling and thus does not block the UI
     * WARN: in order to work correctly certain signals/slots have to be connected in kreenshoteditor.cpp (requestRenderToImageSignal)
     */
    QImage renderToImage();

    /**
     * see renderToImage()
     * see requestRenderToImageSignal()
     */
    void onRenderToImageComplete(QImage image);

    void copyImageToClipboard();

Q_SIGNALS:
    /**
     * document's content was changed (add item, remove item, set base image after crop, ...)
     */
    void contentChangedSignal();

    /**
     * used by "renderToImage()" because the UI renders the final image and we do not want this dependency here
     */
    void requestRenderToImageSignal(kreen::core::Document* document);

    // void requestRenderToImageCompleteSignal(); // not needed

private Q_SLOTS:
    /**
     * will emit contentChangedSignal
     */
    void slotContentChangedSignalEmitter();

private:
    KREEN_PIMPL_DEFINE_D_PTR
};

}
}

#endif
