#!/bin/bash
QEMU_PATH="/home/loon/Desktop/18-349/qemu"

if [ "$#" -lt 1 ]; then
    echo "Not enough arguments!"
    exit -1
fi

CURR_PATH=`pwd`

cd $QEMU_PATH
source ./mountcard.sh
if [ ! -f ".//sdcard/uimage" ]; then
    echo "Error mounting sdcard! Trying to force a dismount before exiting..."
    exec ".//unmountcard.sh"
    exit -1
fi

cd $CURR_PATH
for FILE in "$@"
do
    FILENAME=`basename $FILE`
    cp $FILE "${QEMU_PATH}/sdcard/${FILENAME}"
done

cd $QEMU_PATH
source ./unmountcard.sh
cd $CURR_PATH

