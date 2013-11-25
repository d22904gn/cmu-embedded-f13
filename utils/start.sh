#!/bin/bash
if [ "$#" -eq 1 ]; then
    echo "mmcinit;fatload mmc 0 a3000000 kernel.bin;fatload mmc 0 a0000000 ${1}.bin;go a3000000 this are some arguments woohoo hot sex!" | xclip -selection c
else
    echo "mmcinit;fatload mmc 0 a3000000 kernel.bin;fatload mmc 0 a0000000 hello.bin;go a3000000 this are some arguments woohoo hot sex!" | xclip -selection c
fi
qemu-system-arm -nographic -M verdex -pflash flash.img -sd sdcard.img -s
