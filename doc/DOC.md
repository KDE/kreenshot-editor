kreenshot-editor Documentation
==============================
Screenshot image editing.

This document is the entry point for the kreenshot-editor documentation.

About kreenshot-editor
----------------------
See ../README.md for how to build and run the application. Also known bugs and open questions are listed there.

TODO...

Component 1: main-application
`````````````````````````````
- command line options to give image and optionally captured cursor
- show editor-component

Component 2: image-editor
`````````````````````````
- display image
- edit image
- [TODO] show toolbars for editing
    - define toolbars by creating "prototype items" with default settings
    - make it possible for each toolbar entry to save the current configuration
        - dropdown menu (show basic properties as string like line width, color etc)
- set (including an optional cursor) and get image


The following sections describe the content of the kreenshot documentation folder.

./pak/
------
This folder contains an example package for the openSUSE build service (osc).
Two goals are achieved:

1.  Show by example which packages are needed to build and run kreenshot-editor.
    See pak/osc/kreenshot-editor/kreenshot-editor.spec

2.  Provide a package for openSUSE which can be downloaded here: http://software.opensuse.org/package/kreenshot-editor

3.  Try out building on different target platforms (e.g. i586 or x86_64)

More on how to use the openSUSE build service see pak/osc/README.md

Use the pak/osc/master-to-source.sh to quickly create the source tarball for openSUSE build service.


./kreenshot-editor-local-debug.sh
---------------------------------
When installing kreenshot-editor locally using

    # TODO...
    cmake install

then this script shows which variables must be set to make it run properly.


About this document
-------------------
written in CommonMark http://commonmark.org/
