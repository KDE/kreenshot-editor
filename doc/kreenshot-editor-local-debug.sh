#!/bin/bash
# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJDIR=$DIR/..

# to find the libkreenshoteditor.so
LD_LIBRARY_PATH=$PROJDIR/build/install/usr/lib64:$LD_LIBRARY_PATH

# to find the icons and other app data
XDG_DATA_DIRS=$PROJDIR/build/install/usr/share/:$XDG_DATA_DIRS

$PROJDIR/build/install/usr/bin/kreenshot-editor $@
