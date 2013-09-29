#!/bin/bash
mkdir sdcard
sudo modprobe loop max_part=15
sudo losetup -D
sleep 0.5
sudo losetup -f ./sdcard.img
sleep 0.5
sudo mount -t vfat -o uid=`id -u $USER`,gid=`id -g $USER`,dmask=077,fmask=177,rw /dev/loop0p1 ./sdcard
