#Weekly Report 2016 08/01-2016/08/05

###Date Reported.
2016-08-08

###Reporter
Chenwei01 from THTF company.

##Issues to be solved within the week.
1. Mix Refind with the font package font.efi.  Refind could only build with UDK2014, but font.efi pack is an EDK2(2016) project.  
   A caller should be created to load font.efi for Refind to show Chinese.
2. Make the text's background transparent. Text directly shown has a solid background. This means the whole background of the Refind will be infected by the text. 

##Procedure
1. Monday to Wednesday A lot of core code of edk2 is reviewed and UEFI spec 2.6 is read carefully
   Each UEFI application has a Handle Image.  We can got a device handle for Image, and then build a device path.
   Device Path is the key to load and run some within a UEFI application
   
2. Thursday, The background area where to show text is cropped. The cropped image and buffer built with the text is merged together.
   Then the background of the text dispeared. 
   
Some debug job is done friday and satday. Got a final ver0.1 as below.  
![BOTO](https://github.com/openthos/boto-analysis/blob/master/Screenshots/Cibershot%20from%202016-08-06%2007-52-41.jpg)
