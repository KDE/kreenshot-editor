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
#ifndef KREENSHOTEDITOR_H
#define KREENSHOTEDITOR_H

#include <kreen/util/exportutil.h>
#include <kreen/util/pimplutil.h>
#include <QObject>
#include <QList>
#include <QDateTime>
#include <QAction>
#include <kreen/core/errorstatus.h>

class QWidget;
class QImage;

namespace kreen {

KREEN_PIMPL_FORWARD_DECL(KreenshotEditor)
KREEN_SHAREDPTR_FORWARD_DECL(KreenshotEditor)

namespace core {
KREEN_SHAREDPTR_FORWARD_DECL(Document)
KREEN_SHAREDPTR_FORWARD_DECL(DocumentFile)
}

using namespace core;

namespace ui {
class MainEditorWidget;
}

using namespace ui;

/**
 *
 */
class KREEN_DECL_EXPORT KreenshotEditor : public QObject, public std::enable_shared_from_this<KreenshotEditor>
// http://stackoverflow.com/questions/11711034/stdshared-ptr-of-this
{
    Q_OBJECT
public:
    KREEN_DEFINE_MAKE_SHARED(KreenshotEditor);

    KreenshotEditor();

    virtual ~KreenshotEditor();

    /**
     * Creates a new document.
     * Set the base image data (image to be edited) from the given parameter.
     * Since there is no filename given the file is treated as new.
     * If no image data is given a default image will be used as base image.
     */
    void createNewDocument(QImage image = QImage());

    /**
     * Creates a new document.
     * Loads the image data from the given file path and set output filename
     * to given filename (with or without path).
     */
    void createNewDocumentFromFile(QString filename);

    void createNewDocumentFromClipbard();

    /**
     * the active document, see also documentFile()
     */
    DocumentPtr document();

    /**
     * active document which might have a file on disk or not yet
     * documentFile()->document() == document()
     */
    DocumentFilePtr documentFile();

    /**
     * sets ${YYYY}, ${MM}, ${DD}, ${hh}, ${mm}, ${ss}
     * if datetime is not set the current time is used
     */
    void setCaptureTime(QDateTime datetime = QDateTime());

    /**
     * sets ${description}
     */
    void setDescription(QString text);

    /**
     * Returns the singleton instance of the main editor widget that
     * contains the QGraphicsView with the base image and created items.
     * (Toolbars and menus are created elsewhere)
     */
    MainEditorWidget* mainEditorWidget();

    /**
     * get all actions for choosing "Select", "Rect", "Ellipse" etc. in a group
     * because only one of them is active at a time
     */
    QList<QAction*> toolActions();

    /**
     * all Action except for the toolActions (todo: merge this later)
     */
    QStringList allActionIds();

    /**
     * Returns action from one of the allActionIds() actions. Returns null if not found.
     */
    QAction* actionFromId(QString actionId);

    /**
     * Returns true if the document has not been saved to a file. (This does not mean that the document is clean.)
     * Rationale: If a document is created it will automatically get an associated filename.
     * Either the user saves to this filename, to another or he/she discards the document.
     * As long as the file is not created yet, changes to the output filename preferences will have an
     * effect to the filename.
     */
    bool isDocumentFileNotCreated();

    /**
     * Returns true if the document is clean (see "Clean State" of QUndoStack)
     *
     * If the document file is not created yet
     * it returns false if there were any changes made to the document after the base image was set.
     *
     * If the document file is already created
     * it returns the true if the documents state does not differ from the save state.
     */
    bool isDocumentClean();

    /**
     * Convenience method that shows a warning to the user if the document is not clean
     * and asks to continue or not.
     * Returns true if the action should be continued and false if not (= abort).
     * If the document is clean it returns true (= continue)
     */
    bool warnIfDocumentIsNotClean_shouldContinue();

    /**
     * Opens the Preferences dialog to let the user view and edit settings.
     * todo: center on parent
     */
    void showPreferencesDialog(QWidget* parent);

    /**
     * todo: center on parent
     */
    void showAboutDialog(QWidget* parent);

    /**
     * To be called in the dragEnterEvent method of the mainwindow.
     */
    void dragEnterEventMainWindow(QDragEnterEvent* event);

    /**
     * see dragEnterEventMainWindow
     */
    void dropEventMainWindow(QDropEvent* event);

    /**
     * util method
     */
    QString actionToToolId(QAction* action);

    QString actionToActionId(QAction* action);


Q_SIGNALS:
    void newDocumentCreatedSignal();
    void documentFileStatusChangedSignal();

    //// void outputFileStatusChanged();

protected Q_SLOTS:
    void slotDocumentFileStatusChanged();

    // document and file actions slots:
    void slotDocumentNew();
    void slotDocumentNewFromClipboard();
    void slotDocumentOpen();
    void slotDocumentSave();
    void slotDocumentSaveAs();
    void slotDocumentCopyImageToClipboard();
    void slotDocumentCopyFilenameToClipboard();
    void slotDocumentSelectInFilemanager();
    void slotDocumentLaunchDefaultViewer();

    // edit actions slots:
    void slotEditUndo();
    void slotEditRedo();
    void slotEditUndoRedoActionEnabledUpdate();

    void slotEditDeleteSelectedItems();
    void slotEditDeleteSelectedItemsActionEnabledUpdate();

    void slotEditSelectAll();

    // tool action slots:
    void slotRequestToolBySenderAction();

private:
    KreenshotEditorImplPtr d;
};

}

#endif
