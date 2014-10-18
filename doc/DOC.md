kreenshot-editor Documentation
==============================

This document describes the content of the kreenshot-documentation folder.


pak
---
This folder contains an example package for the openSUSE build service (osc).
Two goals are achieved:

1.  Show by example which packages are needed to build and run kreenshot-editor.
    See pak/osc/kreenshot-editor/kreenshot-editor.spec

2.  Provide a package for openSUSE which can be downloaded here: http://software.opensuse.org/package/kreenshot-editor

More on how to use the openSUSE build service see pak/osc/README.md


kreenshot-editor-local-debug.sh
-------------------------------
When installing kreenshot-editor locally using

    # TODO...
    cmake install

then this script shows which variables must be set to make it run properly.


About this document
-------------------
Written in CommonMark http://commonmark.org/