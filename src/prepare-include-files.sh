#!/bin/sh

echo "[prepare-include-files.sh] start (first argument is INC_TARGET_DIR)"
echo "[prepare-include-files.sh] pwd=`pwd`"
# e. g. generated-include
INC_TARGET_DIR=$1
INC_TARGET_DIR_TMP=$1-tmp
echo "[prepare-include-files.sh] INC_TARGET_DIR=$1"

if [ $INC_TARGET_DIR -eq "" or $INC_TARGET_DIR -eq "/" ]; then
    echo "[prepare-include-files.sh] Danger. Abort"
    exit 1
fi

# for each given dir (separated by space)
# find header files and copy them to target dir
HEADER_SRC_ARR=() # define array
HEADER_TRT_ARR=() # define array
HEADER_SRC_ARR[0]=lib/kreenshoteditor/kreenshoteditor.h
HEADER_TRT_ARR[0]=kreen
HEADER_SRC_ARR[1]=lib/kreenshoteditor/core
HEADER_TRT_ARR[1]=kreen/core
HEADER_SRC_ARR[2]=lib/kreenshoteditor/ui
HEADER_TRT_ARR[2]=kreen/ui
HEADER_SRC_ARR[3]=lib/kreenshoteditor/util
HEADER_TRT_ARR[3]=kreen/util
for i in 0 1 2 3 ; do
    HEADER_SRC=${HEADER_SRC_ARR[$i]}
    HEADER_TRT=${HEADER_TRT_ARR[$i]}
    echo "[prepare-include-files.sh] HEADER_SRC=$HEADER_SRC"
    echo "[prepare-include-files.sh] HEADER_TRT=$HEADER_TRT"
    TARGET_DIR=$INC_TARGET_DIR/$HEADER_TRT
    TARGET_DIR_TMP=${INC_TARGET_DIR_TMP}/$HEADER_TRT
    echo "[prepare-include-files.sh] TARGET_DIR=$TARGET_DIR"
    mkdir -p $TARGET_DIR_TMP

    echo "[prepare-include-files.sh] fill (flatten subdirs)" # TODO: is flatten that what we want???
    FILES=$(grep -R -l --include=*.h KREEN_DECL_EXPORT $HEADER_SRC) # find files that contain KREEN_DECL_EXPORT
    for f in $FILES ; do
        # we need absolute path for ln
        FROM=`pwd`/$f
        echo "[prepare-include-files.sh] create symlink to $FROM"
        # cp --update "$FROM" "$TO" # cp: DANGER to edit a non-tracked file when navigating via IDE
        # so we use ln:
        ln -s --force "$FROM" "$TARGET_DIR_TMP"
    done
done

echo "[prepare-include-files.sh] check if cleaning is required..."

mkdir -p ${INC_TARGET_DIR}

cd $INC_TARGET_DIR_TMP
DIR_A=`ls -R1`
cd ..
cd $INC_TARGET_DIR
DIR_B=`ls -R1`
cd ..

if [ "$DIR_A" != "$DIR_B" ]; then
    echo "[prepare-include-files.sh] clean"
    rm --verbose -R ${INC_TARGET_DIR}/*
    echo "[prepare-include-files.sh] move tmp to target"
    mv $INC_TARGET_DIR_TMP/* -t "${INC_TARGET_DIR}"
else
    echo "[prepare-include-files.sh] no."
fi

# CORE_FILES=$(grep -R -l TIKZCORE_EXPORT core)
# ...
# UI_FILES=$(grep -R -l TIKZUI_EXPORT ui)
# for i in $UI_FILES ; do
#   echo $i
#   cp --update "$i" ./include/tikz/ui
# done
