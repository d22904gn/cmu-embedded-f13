#!/bin/bash
sudo umount ./sdcard
sudo losetup -D
sudo modprobe -r loop
rm -Rf sdcard
