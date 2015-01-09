#!/bin/sh
/usr/sbin/venc_receiver 0 1 /tmp/test00.jpg 1 
cd /tmp/
serverip=`/usr/bin/fw_printenv -n serverip`
tftp -p $serverip -r test00.jpg
sync
