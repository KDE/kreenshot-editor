kreenshot-editor
================

kreenshot-editor is incubating.

Build
-----
$ mkdir build; cd build; cmake ..
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

TODO
----
- NEXT: 1. genincludes --> cmake   2. adapt include paths
- / in description should be replaced by _
- // TODO: tell the document that it's filepath has changed!!!
    1. rename ItemsManager to Document
    2. move baseImage from kreenshoteditor to Document
    3. add filepath to Document
    4. make "save as" work correctly
- // TODO: file format handling!
- // TODO: exception handling or similar!!!
    http://stackoverflow.com/questions/1438535/how-to-run-a-command-at-compile-with-in-makefile-generated-by-cmake
- fix cursor when cancelling crop tool
- save image to file: do not save selection indication rect
- enable more item properties (see item.h/cpp)
- enable hover visual
- makes sure line has a bigger catch area when trying to move
- ToolManager: Draw item tool: text, highlight, obfuscate
- key movement of items

- crop etc. tool
- rename Item to KreenItem
- add context menu to select tools
- add handles for rect resizing and line repositioning (as extra class)
    - Ctrl for propertional
- Draw rect tool
- draw arrow
- click on empty space chooses Select tool
- The QGraphicsProxyWidget class provides a proxy layer for embedding a QWidget in a QGraphicsScene

LATER
-----
- OpenSuseBuild service
- Ctrl+Shift+C: copy image to clipboard
- // TODO: use QProcess to lookup if dolphin exists and use it, else use the existing method
    - dolphin --select
- Settings: After saving: copy image path to clipboard
- Select a Item
  - Handles for resizing appear
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
- Create Actions in lib (instead of UI file) and use in GUI (überlegen: nice ui file vs. good coding)
- "Open last file storage location" or "Open storage location"
- use m4 to generate settings class

QUESTIONS
---------
- how to install the lib and the app?
    - how to put the include files for dev in the right place?
- where to store data files like default background image?
- nullable QPoint for function parameter?
- is QImage reference counted?
- statt QScrollView kann man auch bei QGraphicsScene.setSceneRect verwenden. Was ist besser?

Related tools
-------------
These Linux tools have similar image editor capabilities:
- greenshot (.NET) - the inspirational source for kreenshot-editor
- shutter (Perl)
- hotshots (QT)
