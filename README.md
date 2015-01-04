kreenshot-editor
================

Screenshot image editing.

More documentation can be found in doc/DOC.md.

Website: http://feinstaub.github.io/kreenshot-editor
Central source code repository: https://projects.kde.org/projects/playground/graphics/kreenshot-editor

Quick Build
-----------
# EITHER: for development (Debug, local install prefix):
$ mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=`pwd`/../usr ..

# OR: default (DCMAKE_INSTALL_PREFIX defaults to /usr/local):
$ mkdir build; cd build; cmake ..

# THEN:
$ make

Run
---
in build directory:
$ ./src/kreenshot-editor

via helper script to test local installation:
$ ./doc/kreenshot-editor-local-debug.sh


KNOWN BUGS
----------
- Resize the main window requires the user to click the editor widget to restore the cursor to normal.
  => No mouse events are handled until mouse click.
  Happens since the beginning of the project
- see TODO.txt


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


DEVELOPMENT
-----------
Debug with: $ kreenshot-editor ~/Pictures/screenshots/2014-05-04_Screenshot-1.png

Create new header guard UUI:
$ python -c 'import uuid; print("UUID_" + str(uuid.uuid1()).replace("-", ""))'

Example to run a install prefixed build:
    #!/bin/bash
    LD_LIBRARY_PATH=~/dev/src/kreenshot-editor/build-release/install/usr/lib64:$LD_LIBRARY_PATH
    ~/dev/src/kreenshot-editor/build-release/install/usr/bin/kreenshot-editor $@


Related tools
-------------
- greenshot (.NET) - the inspirational source for kreenshot-editor, Windows only

Linux tools that have similar image editor capabilities:
- shutter   (Perl)  http://shutter-project.org/
- hotshots  (QT)    http://sourceforge.net/projects/hotshots/ http://thehive.xbee.net/index.php?module=pages&func=display&pageid=31

Linux screenshot tools without own image editor:
- ksnapshot (QT)    https://www.kde.org/applications/graphics/ksnapshot/

More:
- Lookit for Ubunto http://pmeyhoefer.de/blog/lookit-einfaches-und-leichtgewichtiges-screenshot-tool-fur-ubuntu/
- http://lifehacker.com/5889994/the-best-screen-capture-tool-for-linux


Implementation Notes: pimpl idiom
---------------------------------
#include <kreen/util/pimplutil.h>
use `KREEN_Q_DECLARE_PRIVATE` to declare the QT style pimpl idiom
use `Q_D();` to define the `d` variable when needed.
NOTE that Q_D() and the other QT macros can also be called with parameter for a prefix name
 but we omit it here because our private class is a nested class which is then name just `Private`.
See also http://stackoverflow.com/questions/25250171/how-to-use-the-qts-pimpl-idiom
TODO: is the `new Private(this)` delete automatically?


Behaviour details: undo and item selection
------------------------------------------
- Undo modified (move, resize etc.) item(s) => (multi)selection is preserved
- Undo deleted item(s) => selection is not restored (todo: is this ok or should it be restored?)


THANKS
------
See the list contributors in the AUTHORS file.
If you did some contribution feel free to add yourself to this list.

Apart from that, the source code of several applications were consulted
for coding and packaging details:

- tikzkit     - source code and discussions with the author gave valuable input
                https://projects.kde.org/projects/playground/graphics/tikzkit
- kolourpaint - took the light cross cursor
                http://www.kde.org/applications/graphics/kolourpaint/
- kdevelop    - how to use icons and primary dev env
- GammaRay    - how to use icons
                http://www.kdab.com/kdab-products/gammaray/
- xplanet     - spec file writing
(last updated 2015-01-04)

Throughout the source code of kreenshot-editor you will find hyperlinks to
internet resources like blogs, mailing list entries or forum posts that
help to solve the specific problem.

At last, the following applications and tools helped to develop kreenshot-editor
- kdevelop - the IDE
- kate/ktexteditor - great code highlighting features
- ksnapshot - making screenshots
- shutter - making screenshots
- dolphin - shifting files
- krusader - icon browsing
- osc - openSUSE build service tool
- git-cola
- GitExtensions - https://github.com/gitextensions/gitextensions
- Inkscape - graphics
- ...
