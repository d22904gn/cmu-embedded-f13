#!/bin/bash
mkdir sdcard
sudo losetup -f ./sdcard.img
sleep 0.5
sudo kpartx -a /dev/loop0
sleep 0.5
sudo mount -t vfat -o uid=`id -u $USER`,gid=`id -g $USER`,dmask=077,fmask=177,rw /dev/mapper/loop0p1 ./sdcard
