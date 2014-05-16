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
#ifndef KREENSHOTEDITOR_H
#define KREENSHOTEDITOR_H

#include <QObject>
#include <memory>

class MainEditorWidget;
class QImage;
class QWidget;

class KreenshotEditorImpl;
typedef std::shared_ptr<KreenshotEditorImpl> KreenshotEditorImplPtr;

class ItemsManager;
typedef std::shared_ptr<ItemsManager> ItemsManagerPtr;

class OutputFilenameManager;
typedef std::shared_ptr<OutputFilenameManager> OutputFilenameManagerPtr;

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
    void setBaseImageData(const QImage& image); // TODO: is QImage reference counted?

    /**
     * Loads the image data from the given path and set default output path to path.
     */
    void setBaseImagePath(QString path);

    /**
     * Returns the base image (without any items).
     * TODO: currently after a crop operation the cropped image will be returned. Is this ok?
     */
    const QImage& getBaseImage();

    /**
     * Ff filepath is not set then the settings from outputFilenameManager are used
     */
    void saveToFile(QString filepath = QString());

    /**
     * singleton
     */
    MainEditorWidget* getMainEditorWidget();

    /**
     * Access to used edited items
     */
    ItemsManagerPtr itemsManager();

    /**
     * Use for initCaptureTime() and setDescription(...)
     */
    OutputFilenameManagerPtr outputFilenameManager();

    /**
     * Is file not saved yet?
     */
    bool isFileNew();

    /**
     * TODO
     *
     * Is saved file modified? Always false if file is new.
     */
    bool isFileModified();

Q_SIGNALS:
    /**
     * todo: fire when appropriate
     */
    void outputFileStatusChanged();

private:
    KreenshotEditorImplPtr d;
};

typedef std::shared_ptr<KreenshotEditor> KreenshotEditorPtr;

#endif // KREENSHOTEDITOR_H
