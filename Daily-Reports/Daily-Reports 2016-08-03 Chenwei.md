# Daily Report 2016-08-01  by Chenwei

##Achievement Today.

As last report mentioned. If we want to have refind Chinese supported, we should load font.efi before the main function loop of refind
 is executed. But unfortunetely, Refind is an UDK2014 based UEFI-Application. We can't build font.efi's soure code with refind into a
 single UEFI-Application. Just because HII features need by font.efi is not provided by UDK2014.  
 
Now as ten-thousands of code refered from the edk source code tree. 

![Alt text](https://github.com/openthos/boto-analysis/blob/master/Screenshots/Referenced%20%20Code%202016-08-03%2017-25-15.png?raw=true)

A loader application is built technically. Code is uploaded to "TryCode" Directory

https://github.com/openthos/boto-analysis/blob/master/TryCode/Apps/ExecuteEfiApp/ExecuteEfiApp.c

As this App runs it will load anothler efi app called \HelloWord.efi on the same disk. And then run it.

The screenshot is as below.

![Alt text](https://github.com/openthos/boto-analysis/blob/master/Screenshots/Screenshot%20from%202016-08-03%2014-51-02.png?raw=false)

The last line is printed by HelloWorld.efi

##Todo List for BOTO
1. Merge codes tryed today to Refind to let Refind Chinese Powered
2. Make Text area transparent to show the background image.
