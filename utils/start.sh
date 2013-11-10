#!/bin/bash
cat gumstix_cmds | xclip -selection c
qemu-system-arm -nographic -M verdex -pflash flash.img -sd sdcard.img -s
