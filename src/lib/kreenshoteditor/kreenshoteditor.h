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

#include <kreen/util/pimplutil.h>
#include <QObject>
#include <memory>
#include <kreen/core/errorstatus.h>

class QWidget;
class QImage;

namespace kreen {

KREEN_PIMPL_FORWARD_DECL(KreenshotEditor)

namespace core {
KREEN_SHAREDPTR_FORWARD_DECL(Document)
KREEN_SHAREDPTR_FORWARD_DECL(OutputFilenameManager)
}

using namespace core;

namespace ui {
class MainEditorWidget;
}

using namespace ui;

/**
 *
 */
class KreenshotEditor : public QObject, public std::enable_shared_from_this<KreenshotEditor>
// http://stackoverflow.com/questions/11711034/stdshared-ptr-of-this
{
    Q_OBJECT
public:
    KreenshotEditor();

    virtual ~KreenshotEditor();

    /**
     * Set the image data of the image to be edited.
     * Since there is no filename given the file is treated as new.
     */
    void setBaseImageData(QImage image);

    /**
     * Loads the image data from the given path and set output filepath to path.
     */
    void setBaseImagePath(QString path);

    /**
     * Returns the base image (without any items).
     * TODO: currently after a crop operation the cropped image will be returned. Is this ok?
     */
    QImage baseImage();

    /**
     * If filepath is not set then the settings from outputFilenameManager are used
     */
    ErrorStatus saveToFile(QString filepath = QString());

    /**
     * singleton
     *
     * TODO: does shared_ptr makes sense here?
     */
    MainEditorWidget* mainEditorWidget();

    /**
     * Access to used edited items
     */
    DocumentPtr itemsManager();

    /**
     * Use for initCaptureTime() and setDescription(...)
     */
    OutputFilenameManagerPtr outputFilenameManager();

    /**
     * returns true if the file is not saved yet (not stored on disk yet)
     */
    bool isFileNew();

    /**
     * TODO
     *
     * Is saved file modified? Always false if file is new.
     */
    bool isFileModified();

    /**
     * opens the Preferences dialog to let the user view and edit settings
     */
    void showPreferencesDialog();

Q_SIGNALS:
    /**
     * todo: fire when appropriate
     */
    void outputFileStatusChanged();

private:
    KreenshotEditorImplPtr d;
};

typedef std::shared_ptr<KreenshotEditor> KreenshotEditorPtr;

}

#endif // KREENSHOTEDITOR_H
