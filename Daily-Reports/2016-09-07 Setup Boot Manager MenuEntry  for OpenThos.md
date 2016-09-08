# Setup Boot Manager MenuEntry  for OpenThos

## Boot Modes for OpenThos
There are 4 boot modes for OpenThos. They are **install**, **liveboot**, **hdimgboot**, **hdboot**.  
The boot mode is determined by BOOT_MODE item in the boot options. 

## install mode
_The **install** mode is used on the install media only._   
The install media should be formatted as a fat32 partition.  And the partition should be labeled as ***OTO_INSTDSK***  
Directory Structure for live media should be.
<pre>
/
├─efi
│  └─boot
│      ├─bootx64.efi               #grub2 efi bootmanager
│      └─grub.cfg                  #grub2 efi configuration file
└─OpenThos
   ├─kernel
   ├─initrd.img
   ├─install.img
   ├─system.img
   └─boto.zip                      #Boto boot manager package to be installed
</pre>

BOOT_MODE item should be  set as  below
`
BOOT_MODE=install
`  
There are two install methods: install to partition, install to ".img" file. And it is determined by INST_MODE item.  
Thus for **install to partition** method, the  boot option should be:  
`
BOOT_MODE=install   INST_MODE=hd
`  
And for install to ".img" file** method, the  boot option should be:    
`
BOOT_MODE=install   INST_MODE=hdimg
`

## liveboot
_The **live** mode is used on the install media only._   
The install media should be formatted as a fat32 partition.  And the partition should be labeled as "**OTO\_INSTDSK***"  
Directory Structure for live media should be.
<pre>
/
├─efi
│  └─boot
│      ├─bootx64.efi               #grub2 efi bootmanager
│      └─grub.cfg                  #grub2 efi configuration file
└─OpenThos
   ├─kernel
   ├─initrd.img
   ├─install.img
   ├─system.img
   └─boto.zip                      #Boto boot manager package to be installed
</pre>
BOOT\_MODE item should be  set as  below  
`
BOOT\_MODE=liveboot
`  
And there is no append boot option item necessary.
## hdboot
*The **hdboot** is used on the installed platform*  
**GPT** partition table supported only  
BOOT_MODE item should be  set as  below  
`
BOOT_MODE=hdboot
`  
Append boot option items are: RAMDISK\_HD\_UUID, RAMDISK\_IMG, SYSTEM\_HD\_UUID, DATA\_HD\_UUID
### RAMDISK\_HD\_UUID
RAMDISK\_HD\_UUID is the UUID of the partition contains the file ramdisk.img on the hard disk.  
The partition is ESP partition usually.
### RAMDISK\_IMG
RAMDISK\_IMG is the full path of the file ramdisk.img  
It is typically set as:  
`
RAMDISK_IMG=OpenThos/ramdisk.img
`  
### SYSTEM\_HD\_UUID
SYSTEM\_HD\_UUID is the UUID of the partition that the install process extracts system.img to and will be mounted as /android/system
### DATA\_HD\_UUID
DATA\_HD\_UUID is the UUID of the partition that the install process have created and will be mounted as /android/data.  
**Notice: the data partition should be formatted with ext2, ext3 or ext4 format. For some unknown reason, if ntfs and vfat used, the system will be depressed.**  

## hdimgboot
*The **hdimgboot** is used on the installed platform*  
**GPT** partition table supported only  
BOOT_MODE item should be  set as  below  
`
BOOT_MODE=hdimgboot
`  
Append boot option items are: RAMDISK\_HD\_UUID, RAMDISK\_IMG, SYSTEM\_HD\_UUID, DATA\_HD\_UUID
### RAMDISK\_HD\_UUID
RAMDISK\_HD\_UUID is the UUID of the partition contains the file ramdisk.img on the hard disk.  
The partition is ESP partition usually.
### RAMDISK_IMG
RAMDISK\_IMG is the full path of the file ramdisk.img  
It is typically set as:  
`
RAMDISK_IMG=OpenThos/ramdisk.img
`  
### SYSTEM\_HD\_UUID
SYSTEM\_HD\_UUID is the UUID of the partition that the install process copies system.img to.
### SYSTEM\_IMG
SYSTEM\_IMG is the full path that the install process copy system.img to.  
It is typically set as:  
`
RAMDISK_IMG=OpenThos/system.img
`  
### DATA\_HD\_UUID
DATA\_HD\_UUID is the UUID of the partition that the install process have created and will generate a data.img file on it.
**Notice: the data partition should be formatted with ext2, ext3, ext4. If fat32 used, the install process will be failed to generate file larger than 2GB.**  
### DATA_IMG
DATA_IMG is the full path of file data.img.   


