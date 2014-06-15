#!/bin/sh

echo "[prepare-include-files.sh] start"
echo "[prepare-include-files.sh] pwd=`pwd`"

mkdir -p generated-include/kreen/core
mkdir -p generated-include/kreen/ui

for d in lib/kreenshoteditor/core lib/kreenshoteditor/ui ; do
    echo "[prepare-include-files.sh] d=$d"
    TO=./generated-include/kreen/`basename $d`
    echo "[prepare-include-files.sh] TO=$TO / clean it"
    rm --verbose $TO/*
    echo "[prepare-include-files.sh] fill it"
    FILES=$(grep -R -l --include=*.h class $d) # TODO: replace class with KREEN_EXPORT
    for i in $FILES ; do
        FROM=$i
        echo "[prepare-include-files.sh] $FROM --> TO"
        cp --update "$FROM" "$TO"
    done
done

# CORE_FILES=$(grep -R -l TIKZCORE_EXPORT core)
# ...
# UI_FILES=$(grep -R -l TIKZUI_EXPORT ui)
# for i in $UI_FILES ; do
#   echo $i
#   cp --update "$i" ./include/tikz/ui
# done
