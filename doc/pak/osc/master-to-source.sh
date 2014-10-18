#/usr/bin/bash

cd ../../..
TARGETDIR=`pwd`/doc/pak/osc/kreenshot-editor
git archive master --output=$TARGETDIR/kreenshot-editor-AAA.tar.gz

echo '[INFO] File created: osc/kreenshot-editor/kreenshot-editor-AAA.tar.gz'
echo '[TODO] Now change AAA in the filename to the version and adapt the Version tag of the specfile'
echo '[TODO] Then do local build and commit to build service if everything works fine.'

echo "Going to $TARGETDIR (works if the script is sourced)"
cd $TARGETDIR
