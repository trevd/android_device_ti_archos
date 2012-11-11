#!/bin/sh
#### Dirty Build and deploy script for rapid testing

#a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/qt 
if [ $1 -eq 4 ]; then
echo " build push chmod run"
mm && \
a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/checkfs /sbin/checkfs 
	a s chmod +x ./sbin/checkfs  
a s /sbin/checkfs
fi
if [ $1 -eq 1 ]; then
	a s /sbin/checkfs
fi
if [ $1 -eq 5 ]; then
mm && \
a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/checkfs /sbin/checkfs 
a s chmod +x ./sbin/checkfs  
	a s update_ramdisk
fi
if [ $1 -eq 2 ]; then
a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/checkfs /sbin/checkfs 
a s chmod +x ./sbin/checkfs  
	a s update_ramdisk
fi
if [ $1 -eq 3 ]; then
echo " build push chmod update"
mm && \
a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/checkfs /sbin/checkfs &&
	a s chmod +x ./sbin/checkfs  &&
	a s update_ramdisk
fi
#a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/readmem /sbin/readmem 
#a pu /media/android/build/android-omapzoom/out/target/product/archos/root/sbin/reboot2 /sbin/reboot2 
#a s chmod +x ./sbin/readmem
#a s chmod +x ./sbin/reboot2
#a s chmod +x ./qt
#a s ./qt

