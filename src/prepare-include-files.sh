#!/bin/sh

echo "[prepare-include-files.sh] start (first argument is INC_TARGET_DIR)"
echo "[prepare-include-files.sh] pwd=`pwd`"
INC_TARGET_DIR=$1
echo "[prepare-include-files.sh] INC_TARGET_DIR=$1"

# for each given dir (separated by space)
# find header files and copy them to target dir
HEADER_SRC_ARR=()
HEADER_TRT_ARR=()
HEADER_SRC_ARR[0]=lib/kreenshoteditor/kreenshoteditor.h
HEADER_TRT_ARR[0]=kreen
HEADER_SRC_ARR[1]=lib/kreenshoteditor/core
HEADER_TRT_ARR[1]=kreen/core
HEADER_SRC_ARR[2]=lib/kreenshoteditor/ui
HEADER_TRT_ARR[2]=kreen/ui
for i in 0 1 2 ; do
    HEADER_SRC=${HEADER_SRC_ARR[$i]}
    HEADER_TRT=${HEADER_TRT_ARR[$i]}
    echo "[prepare-include-files.sh] HEADER_SRC=$HEADER_SRC"
    echo "[prepare-include-files.sh] HEADER_TRT=$HEADER_TRT"
    TARGET_DIR=$INC_TARGET_DIR/$HEADER_TRT
    echo "[prepare-include-files.sh] TARGET_DIR=$TARGET_DIR"
    mkdir -p $TARGET_DIR

    # causes rebuilds (but not with ln)
    # TODO: move outside to clean everything at once recursively?
    echo "[prepare-include-files.sh] clean"
    rm --verbose $TARGET_DIR/*

    echo "[prepare-include-files.sh] fill (flatten subdirs)" # TODO: is flatten that what we want???
    FILES=$(grep -R -l --include=*.h class $HEADER_SRC) # TODO: replace search keyword "class" with KREEN_EXPORT
    for f in $FILES ; do
        # we need absolute path for ln
        FROM=`pwd`/$f
        echo "[prepare-include-files.sh] create symlink to $FROM"
        # cp --update "$FROM" "$TO" # cp: dANGER to edit a non-tracked file when navigating via IDE
        # so we use ln:
        ln -s --force "$FROM" "$TARGET_DIR"
    done
done

# CORE_FILES=$(grep -R -l TIKZCORE_EXPORT core)
# ...
# UI_FILES=$(grep -R -l TIKZUI_EXPORT ui)
# for i in $UI_FILES ; do
#   echo $i
#   cp --update "$i" ./include/tikz/ui
# done
