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

steps
-----
1. Read QT quick tutorial and build project upon it
2. have command line parser to give an image
3. build one component (widget) that will be used in a separated main component

editor-component
----------------
- display image
- show toolbars for editing
- set (including an optional cursor) and get image

main-window
-----------
- show editor-component
- command line options to give image and cursor (optional)
