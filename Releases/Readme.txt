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
ing installed.
1. Create a gpt partition table on the disk. You can do it with a windows insta-
  ll media or an ubuntu live media (16.04 e.g.)
  With windows install media.
    1) Boot your platform with a windows 10 install media.
    2) Click "Repair your computer" when "Install now" button appears.
    3) Choose the option "Trobleshoot-Reset your PC or see advanced options"
    4) Choose "Command Prompt" at the "Advanced options" page.
    5) Type "diskpart" and enter to run it.
    6) Run "list disk", you will get a prompt like this.
	  Disk ###  Status         Size     Free     Dyn  Gpt
  	  --------  -------------  -------  -------  ---  ---
  	  Disk 0    Online          238 GB      0 B          
    7) Run "select disk x", on my platform it's "select disk 0". You will get a
      prompt like this.
          Disk 0 is now the selected disk.
    8) Run "clean" to clean the configuration of the disk. You will get a prompt
      like this.
          Diskpart succeeded in cleaning the disk.          
    9) Run "convert gpt" to create a gpt partition table on the disk. You will 
      get a prompt like this.
          Diskpart successfully convert the selected disk to GPT format.
2. Create a EFI system partition on the disk.
    10) Run "create partition efi size=8192". You will get a prompt like this.
          Diskpart succeeded in creating the specified partition.
    11) Run "format quick fs=fat32 label=ESP" to format the partition you've ju-
      st created.

