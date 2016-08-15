################################################################################
# Title: BOTO Release Notes 
# Version: Alpha1.
# Date: 2016/08/12
# By: David Chan (also Chenwei)
# Mail: Chenwei01@thtfpc.com
################################################################################

[Note]
--------------------------------------------------------------------------------
*********FOR TEST ONLY, DO NOT USE WITH THE PLATFORM YOU ARE WORKING ON*********
*********FOR TEST ONLY, DO NOT USE WITH THE PLATFORM YOU ARE WORKING ON*********
*********FOR TEST ONLY, DO NOT USE WITH THE PLATFORM YOU ARE WORKING ON*********

[Dependencies]
--------------------------------------------------------------------------------
1. A X86 64bit PC
2. UEFI Environment
3. UEFI boot mode activated
4. Secure boot deactivated

[Notice]
--------------------------------------------------------------------------------
#### IF THE SCREEN IS NOT 1920x1080 PLEASE MODIFY efi\boot\refind ####
@line: 142
  Change "resolution 1920 1080" with the resolution of you screen.
#### Some 1366x768 screen may have no 1366x768 mode in UEFI environment ####
  Try 1280x768.



[How to install BOTO]
--------------------------------------------------------------------------------
  It is assumed that you are going to install it on bare platform without anyth-
ing installed.  You can do it with a windows install media or an ubuntu live me-
dia (16.04 e.g.)
## Do it with Winows 10 UEFI Install USB drive. ##
-----------------------------------------------
1. Prepair a Windows 10 Install USB drive with boto stored on your working PC.
  Create a esp folder on the USB drive. Download boto_alpha1.tar.gz and ext-
ract efi folder into esp folder.
2. Create a gpt partition table on the disk.
    1) Boot your platform with a windows 10 UEFI install USB drive.
    2) Click "Repair your computer" when "Install now" button appears.
    3) Choose the option "Trobleshoot-Reset your PC or see advanced options"
    4) Choose "Command Prompt" at the "Advanced options" page.
    5) Type "diskpart" and enter to run it.
    6) Run "list disk", you will get a prompt like this.
	  Disk ###  Status         Size     Free     Dyn  Gpt
  	  --------  -------------  -------  -------  ---  ---
  	  Disk 0    Online          120 GB   120 GB          
  	  Disk 1    Online           14 GB      0 B          
    7) Run "select disk x". X is 0 on my platform, as Disk is the USB Drive . Y-
    ou will get a prompt like this.
          Disk 0 is now the selected disk.
    8) Run "clean" to clean the configuration of the disk. You will get a prompt
      like this.
          Diskpart succeeded in cleaning the disk.          
    9) Run "convert gpt" to create a gpt partition table on the disk. You will 
      get a prompt like this.
          Diskpart successfully convert the selected disk to GPT format.
3. Create a EFI system partition on the disk.
    10) Run "create partition efi size=8192". You will get a prompt like this.
          Diskpart succeeded in creating the specified partition.
    11) Run "format quick fs=fat32 label=ESP" to format the partition you've ju-
      st created.
4. Copy boto into the EFI system partition.
    12) Run "assign" to assign a drive letter to the EFI system partition.
    13) Run "list volume" to check the drive letter created.
          Volume ###  Ltr  Label    Fs     Type        Size    Status    Info 
          ----------  ---  -------  -----  ----------  ------  --------  -------
          Volume 0     D                   DVD-ROM        0 B  No Media           
          Volume 1     C   W10 USB  FAT32  Removable    14 GB  Healthy            
          Volume 2     E   ESP      FAT32  Partition     8 GB  Healthy   Hidden  
        We know the drive e: is the drive for the partition we've just created. 
      And drive c: is the drive for the windows 10 UEFI install USB drive.
    14) Type "exit" and press enter to leave diskpart and go back to the cmd.exe
    15) Run "xcopy /sy c:\ESP\* e:\" to copy boto into the EFI system partition.
    16) Type "diskpart" and press enter to run diskpart.
    17) Run "select disk 0" to select the disk with efi system partition.
    18) Run "Select partition 1" to select the efi system partition.
    19) Run "remove" to delete the drive letter of the efi system partition.
    20) Close the window cmd.exe and choose "Turn off your PC".
5. Now BOTO is stanby to work for you.
    
## Do it with UBUNTU 16.04 UEFI Install Media ##
--------------------------------------------------
1. Prepair an UBUNTU 16.04 UEFI USB drive with boto stored on your working pc.
    Create a esp folder on the Ubuntu USB drive. Download boto_alpha1.tar.gz and
  extract efi folder into esp folder.
2. Create a gpt partition table on the disk.
    1) Boot your platform with the ubuntu 16.04 install usb drive.
    2) Choose "Try Ubuntu without installing" when GNU GRUB menu appears.
    3) After the live Ubuntu environment ready, open a terminal.
    4) Execute "lsblk", you will get a prompt like this.
          NAME    MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
          sda       8:0    0  120G  0 disk           
          sdb       8:16   1 14.4G  0 disk           
          └─sdb1    8:17   1 14.4G  0 part /cdrom
          sr0      11:0    1 1024M  0 rom            
          loop0     7:0    0  1.4G  1 loop /rofs
       Now we know /dev/sda is the disk we will install boto.	  
    5) Execute "sudo parted /dev/sda". 
    6) Run command "mktable gpt" and answer "Yes" to create a new empty GPT par-
	  tition table. 
    7) Type "quit" and press enter to exit the parted.
3. Create a EFI system partition on the disk.
    8) Run "mkpart primary 1049K 83839656K" to create a 8GB primary partition f-
    rom the start of the disk.
    8) Run "print" to review the partition created.
          Number  Start  End     Size   File system  Name      Flags
           1      1049KB 8390MB  8389MB fat32        primary
    9) Run "toggle 1 esp" to convert the partion to EFI format.
    10) press "quit" press enter to quit the sfdisk environment.   
    11) Run "sudo mkfs.vfat -F 32 -n ESP /dev/sda1" to format EFI system partition.
4. Copy boto into the EFI system partition.
    12) Run "sudo mount /dev/sda1 /mnt" to mount EFI system partition.
    13) Run "sudo cp -av /rofs/ESP/* mnt" to copy boto to the hard disk.
    14) Run "sync;sync;sync" to flush buffer.
    15) Run "sudo umount /mnt" to umount EFI system partition.
    And turn of the platform, the boto is stanby now.
    
[How to install Windows]
--------------------------------------------------------------------------------
  While BOTO is ready, you can install windows now. Windows' installer will keep 
BOTO as the bootloader as it found ESP:\efi\boot\bootx64.efi.

[How to install OpenTHOS]
--------------------------------------------------------------------------------
  Install OpenTHOS with the live usb, but do not install grub or grub efi.
  After succedded the installing, move evthing form the "Android_X86_YYYY_MM_DD"
Direcotry to the root of the ext4 partition and rename kernel to otoKernel.
