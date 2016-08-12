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


[How to install]
--------------------------------------------------------------------------------
  It is assumed that you are going to install it on bare platform without anyth-
ing installed.  You can do it with a windows install media or an ubuntu live me-
dia (16.04 e.g.)
## Do with Winows 10 UEFI Install USB drive. ##
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
          Volume ###  Ltr  Label        Fs     Type        Size     Status     Info    
          ----------  ---  -----------  -----  ----------  -------  ---------  --------
          Volume 0     D                       DVD-ROM         0 B  No Media           
          Volume 1     C   W10 USB      FAT32  Removable     14 GB  Healthy            
          Volume 2     E   ESP          FAT32  Partition      8 GB  Healthy    Hidden  
        We know the drive e: is the drive for the partition we've just created. And
      drive c: is the drive for the windows 10 UEFI install USB drive.
    14) Type "exit" and press enter to leave diskpart and go back to the cmd.exe
    15) Run "xcopy /sy c:\ESP\efi e:\" to copy boto into the EFI system partition.
    16) Type "diskpart" and press enter to run diskpart.
    17) Run "select disk 0" to select the disk with efi system partition.
    18) Run "Select partition 1" to select the efi system partition.
    19) Run "remove" to delete the drive letter assigned to the efi system partition.
    20) Close the window cmd.exe and choose "Turn off your PC".
5. Now BOTO is stanby to work for you.
    
## Do with UBUNTU 16.04 UEFI Install Media ##
--------------------------------------------------
1. Prepair an UBUNTU 16.04 UEFI USB drive with boto stored on your working pc.
  Create a esp folder on the Ubuntu USB drive. Download boto_alpha1.tar.gz and ext-
ract efi folder into esp folder.
2. Create a gpt partition table on the disk.
    1) Boot your platform with the ubuntu 16.04 install media, cdrom or a usb drive.
    2) Choose "Try Ubuntu without installing" when GNU GRUB menu appears.
    3) After the live Ubuntu environment ready, open a terminal.
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
          Volume ###  Ltr  Label        Fs     Type        Size     Status     Info    
          ----------  ---  -----------  -----  ----------  -------  ---------  --------
          Volume 0     D                       DVD-ROM         0 B  No Media           
          Volume 1     C   W10 USB      FAT32  Removable     14 GB  Healthy            
          Volume 2     E   ESP          FAT32  Partition      8 GB  Healthy    Hidden  
        We know the drive e: is the drive for the partition we've just created. And
      drive c: is the drive for the windows 10 UEFI install USB drive.
    14) Type "exit" and press enter to leave diskpart and go back to the cmd.exe
    15) Run "xcopy /sy c:\ESP\efi e:\" to copy boto into the EFI system partition.
    16) Type "diskpart" and press enter to run diskpart.
    17) Run "select disk 0" to select the disk with efi system partition.
    18) Run "Select partition 1" to select the efi system partition.
    19) Run "remove" to delete the drive letter assigned to the efi system partition.

