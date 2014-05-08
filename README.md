kreenshot-editor
================

kreenshot-editor is incubating

build
-----
$ mkdir build; cd build; cmake ..
$ make

run
---
in directory build:
$ ./src/kreenshot-editor

NEXT steps
----------
1. Read QT quick tutorial and build project upon it
2. have command line parser to give an image
3. build one component (widget) that will be used in a separated main component

TODO
----
- add handles for rect resizing and line repositioning (as extra class)
    - Ctrl for propertional
- Draw rect tool
- draw arrow
- click on empty space chooses Select tool
- The QGraphicsProxyWidget class provides a proxy layer for embedding a QWidget in a QGraphicsScene
- nullable QPoint for function parameter?
- show custom editor widget in MainWindow
- is QImage reference counted?

DEV
---
Debug with: $ kreenshot-editor ~/Pictures/screenshots/2014-05-04_Screenshot-1.png

main-application
----------------
- command line options to give image and cursor (optional)
- show editor-component

editor-component
----------------
- display image
- show toolbars for editing
    - define toolbars by creating "prototype items" with default settings
    - make it possible for each toolbar entry to save the current configuration
        - dropdown menu (show basic properties as string like line width, color etc)
- set (including an optional cursor) and get image

TODO
----
- Item
- DrawingArea
- BackgroundImage
- On the DrawingArea there are Items on the BackgroundImage.
- Select a Item
  - Handles for resizing appear
  - Controls for common properties appear
     - most recently used or pinned colors
     - slider for line width
     - slider for font size
     - slider for arrow tip typ?
- Hover over an Item => highlighing and ability to move the Item
