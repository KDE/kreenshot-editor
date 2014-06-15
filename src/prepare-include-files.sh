#!/bin/sh

echo "[prepare-include-files.sh] start (first argument is INC_TARGET_DIR)"
echo "[prepare-include-files.sh] pwd=`pwd`"
INC_TARGET_DIR=$1
echo "[prepare-include-files.sh] INC_TARGET_DIR=$1"

# for each given dir (separated by space)
# find header files and copy them to target dir
for d in lib/kreenshoteditor/core lib/kreenshoteditor/ui ; do
    echo "[prepare-include-files.sh] d=$d"
    TO=$INC_TARGET_DIR/kreen/`basename $d`
    echo "[prepare-include-files.sh] TO=$TO"
    mkdir -p $TO

    # causes rebuilds
    # echo "[prepare-include-files.sh] clean it"
    # rm --verbose $TO/*
    # echo "[prepare-include-files.sh] fill it"

    FILES=$(grep -R -l --include=*.h class $d) # TODO: replace class with KREEN_EXPORT
    for i in $FILES ; do
        FROM=`pwd`/$i
        echo "[prepare-include-files.sh] $FROM --> TO"
        # cp --update "$FROM" "$TO" # cp: dANGER to edit a non-tracked file when navigating via IDE
        # so we use ln:
        ln -s --force "$FROM" "$TO"
    done
done

# CORE_FILES=$(grep -R -l TIKZCORE_EXPORT core)
# ...
# UI_FILES=$(grep -R -l TIKZUI_EXPORT ui)
# for i in $UI_FILES ; do
#   echo $i
#   cp --update "$i" ./include/tikz/ui
# done
