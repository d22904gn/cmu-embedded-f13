#!/bin/bash
sync
sudo umount ./sdcard
sleep 0.5
sudo kpartx -d /dev/loop0
sleep 0.5
sudo losetup -d /dev/loop0
rm -Rf sdcard
