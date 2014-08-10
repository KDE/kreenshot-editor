kreenshot-editor
================

kreenshot-editor is incubating.

Build
-----
# EITHER: for development (Debug, local install prefix):
$ mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=`pwd`/../usr ..

# OR: default (DCMAKE_INSTALL_PREFIX defaults to /usr/local):
$ mkdir build; cd build; cmake ..

# THEN:
$ make

Run
---
in directory build:
$ ./src/kreenshot-editor

Component 1: main-application
-----------------------------
- command line options to give image and optionally captured cursor
- show editor-component

Component 2: image-editor
-------------------------
- display image
- edit image
- [TODO] show toolbars for editing
    - define toolbars by creating "prototype items" with default settings
    - make it possible for each toolbar entry to save the current configuration
        - dropdown menu (show basic properties as string like line width, color etc)
- set (including an optional cursor) and get image

DEV
---
Debug with: $ kreenshot-editor ~/Pictures/screenshots/2014-05-04_Screenshot-1.png

Create new header guard UUI:
$ python -c 'import uuid; print("UUID_" + str(uuid.uuid1()).replace("-", "_"))'

TODO
----
+// TODO: do not apply EVERY mouse release (only those with real changes)
+// TODO: creating a new item crashes
+// TODO: the selection is lost after mouse release!
QString("slotUpdateItemsGeometryFromModel___todo"));
TODO: return KreenGraphicsItemBase* instead of QGraphicsItem*
TODO: what happens on active image operation?
scene()->saveCurrentKreenItemsSelection(); // TODO: ok here and below?
+        // TODO: reenable!!!

- QUndostack, Undo, Redo
    - first: MoveItemCmd (see uncommitted code; does not compile yet)
    - OP-crop is not finished yet (notificationGroup etc, setSceneRect seems not to be called after undo)
    - after OP-crop etc: "QGraphicsScene::removeItem: item 0x1183580's scene (0x7fa4ed478f20) is different from this scene (0xe83b18)"
        - find out if this happens again
    - do remaining commands (move item)
- implement SelectionHandles
    - best way to make the item resize?
    - (oder zuerst QUndostack?)
- when document is modified, it file status should become "modified"
    - do this by cleanly use commands (qundostack)
- see /home/gregor/kdesrc/kde/kdegraphics/kolourpaint/cursors/
- mouse over: draw a light version of the selection rects
- use more KreenGraphicsItemBase::graphicsItem()
- enable more item properties (see item.h/cpp)
- enable hover visual
- makes sure line has a bigger catch area when trying to move
- key movement of items
- crop etc. tool
    - should set new baseImage and move item coordinates to adapt
- add context menu to select tools
- add handles for rect resizing and line repositioning (as extra class)
    - Ctrl for propertional
- draw arrow
- The QGraphicsProxyWidget class provides a proxy layer for embedding a QWidget in a QGraphicsScene
- think about it: File->New with same filename as before is not good
    - add postfix?
    - set new datetime (has no effect if preferences are not set accordingly)
        - apply changed prefs for new document (output path) without restart
    - just use empty filename?

LATER
-----
- BUG:
    - Select Tool selected
    - with mouse over an item ("move" cursor is shown)
    - Hit ESC => "move" cursor switches to normal and one has to move mouse to restore it
- new: KreenColorButton with transparency handling etc
- remove default QGraphicsItem selection marquee
- cmake for testdata: file(COPY yourDir DESTINATION yourDestination)
- PageOutput Settings: file format combobox should use setModel(), QAbstractItemModel,
    be an own widget and show also description
- OpenSuseBuild service
    - pak: run unit tests after build
- Ctrl+Shift+C: copy image to clipboard
    - show message flash, when done (or KMessageBox, "show never again")
    - plus: provide drag source for DND
- Open last save location
- lib: provide different kinds of mainwidget
   - with or without toolbar
   - with some predefined stuff etc.
- Select a Item
  - Controls for common properties appear
     - most recently used or pinned colors
     - slider for line width
     - slider for font size
     - slider for arrow tip typ?
- AFTER UndoManager is complete:
    - write filename in title and full path in status bar (<path> [change...])
        let appear in windows title and in filename --> statusbar / ...edit pattern...
        [NEW] ...
        *  ... (see mainwindow.cpp)
- "Open last file storage location" or "Open storage location"
- Drag source for image in toolbar
    - this can currently been achieved by lauching default viewer (Ctrl+Alt+L) and use drag source from there
- DND:
    - Drop image on scene to create an image object not implemented yet.\nDrop the image anywhere else on the window to load it as new base image.
    - better: not not distinguish between window and scene => maybe show a context menu or similar
- DND: support dropping images from HTTP browser
    // TODO: support also http images
- "Copy filename to clipboard": show notification that something was copied; show warning if file is not saved yet
- how to read/use the scalable image (svg)?
     //baseImage = QIcon::fromTheme("kcmkwm").pixmap(QSize(512, 512)).toImage();
- ? click on empty space should choose Select tool
- ? use m4 to generate settings class
- ? use #pragma once? (non-standard) / replace it as soon as someone sees a problem

BUGS
----
- BUG: resize the main window requires the user to click the editor widget to restore the cursor to normal
    nothing goes until it was clicked
- item->lineStyle()->width = 3; // TODO: if this number is uneven, then the item AND the black selection handles become blurred!

QUESTIONS
---------
- search for "WORKAROUND"
- how to provide own icons?
- how to install the lib and the app?
    - how to put the include files for dev in the right place?
- where to store data files like default background image?
- nullable QPoint for function parameter? --> just use a shared_ptr???
- statt QScrollView kann man auch bei QGraphicsScene.setSceneRect verwenden. Was ist besser?
- prepare-include-files.sh: does it makes sense to distinguish between KREEN_CORE_EXPORT and KREEN_UI_EXPORT???
- restrict drawing outside scene rect; clip, clipping (currently solved with workaround)

Related tools
-------------
These Linux tools have similar image editor capabilities:
- greenshot (.NET) - the inspirational source for kreenshot-editor
- shutter (Perl)
- hotshots (QT)

Thanks to contributors
----------------------
- Code from Tikzkit (https://projects.kde.org/projects/playground/graphics/tikzkit)

