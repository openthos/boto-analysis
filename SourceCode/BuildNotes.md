# BOTO: HOW TO BUILD
## Consists
Boto consists of below modules:
### ExecuteEfiApp
ExecuteEfiApp is the caller to the other modules.
### BotoFont
BotoFont is the language support package.
### Refind (Boto Edition)
Boto is branch based on refind. Original Refind does not support Chinese Character. This is why a boto branch is created.

## Dependencies
### Ubuntu 16.04.1 X64
Ubuntu 16.04.1 is the platform to build everthing. If possible please use a real platform, but not a VMware one to avoid potential problem. As on my VMware one, the ENVIRONMENT variable will not refresh well. "source edksetup.sh BaseTools" keeps complaining to me with "bash:  /home/chanuei/edk2/BaseTools/BuildEnv: No such file or directory".
Packages build-essential, iasl, nasm is necessary.  To install them, execute
below command.
```bash
user@machine:~$ sudo apt-get install build-essential iasl nasm uuid-dev xorg-dev git p7zip-full
```
Maybe other edition of Ubuntu or other Linux distributions could works well.  How ever Ubuntu 16.04.1 is recommended.
### edk2
ExecuteEfiApp and Font are based on edk2.
Perhaps the tianocore managed edition works well.  Run command as below to get it.
```bash
user@machine:~$ git clone https://github.com/tianocore/edk2.git
```
If error occurs while you are trying to build RunEfiApp and BotoFont with tianocore managed garage, please download edk2.tar.gz here. Untar the package after download.  Open a terminal and run:
```bash
user@machine:~$ wget https://github.com/openthos/boto-analysis/raw/master/SourceCode/edk2.7z
user@machine:~$ 7z x edk2.7z
user@machine:~$ cd edk2/BaseTools
user@machine:BaseTools$ make -C Source/C
user@machine:BaseTools$ make
```
Edit Conf/target.txt
```patch
Conf/target.txt.patch
	--- Conf/target.txt.orig	2016-08-17 16:11:45.381949000 +0800
	+++ Conf/target.txt	2016-08-17 16:22:32.531987802 +0800
	@@ -23,7 +23,7 @@
	 #                                               build. This line is required if and only if the current
	 #                                               working directory does not contain one or more description
	 #                                               files.
	-ACTIVE_PLATFORM       = Nt32Pkg/Nt32Pkg.dsc
	+ACTIVE_PLATFORM       = BotoPkg/BotoPkg.dsc

	 #  TARGET                List       Optional    Zero or more of the following: DEBUG, RELEASE, NOOPT
	 #                                               UserDefined; separated by a space character.
	@@ -31,7 +31,7 @@
	 #                                               valid targets specified in the platform description file
	 #                                               will attempt to be built. The following line will build
	 #                                               DEBUG platform target.
	-TARGET                = DEBUG
	+TARGET                = RELEASE

	 #  TARGET_ARCH           List       Optional    What kind of architecture is the binary being target for.
	 #                                               One, or more, of the following, IA32, IPF, X64, EBC, ARM
	@@ -47,7 +47,7 @@
	 #                                               If not specified, then all valid architectures specified
 	 #                                               in the platform file, for which tools are available, will be
	 #                                               built.
	-TARGET_ARCH           = IA32
	+TARGET_ARCH           = X64

	 #  TOOL_DEFINITION_FILE  Filename  Optional   Specify the name of the filename to use for specifying
	 #                                             the tools to use for the build.  If not specified,
	@@ -57,7 +57,7 @@
	 #  TAGNAME               List      Optional   Specify the name(s) of the tools_def.txt TagName to use.
	 #                                             If not specified, all applicable TagName tools will be
	 #                                             used for the build.  The list uses space character separation.
	-TOOL_CHAIN_TAG        = MYTOOLS
	+TOOL_CHAIN_TAG        = GCC49

	 # MAX_CONCURRENT_THREAD_NUMBER  NUMBER  Optional  The number of concurrent threads. Recommend to set this
	 #                                                 value to one more than the number of your compurter
```
### UDK2014
To build Refind, you should have UDK2014 installed to /usr/local.  Open another terminal other than the one used to setup edk2, and run commands listed below.
```bash
user@machine:~$ wget http://downloads.sourceforge.net/project/edk2/UDK2014_Releases/UDK2014.SP1.P1\
/UDK2014.SP1.P1.Complete.MyWorkSpace.zip
user@machine:~$ mkdir temp
user@machine:~$ unzip UDK2014.SP1.P1.Complete.MyWorkSpace.zip -d temp/
user@machine:~$ sudo chmod a+w /usr/local/UDK2014
user@machine:~$ unzip temp/UDK2014.SP1.P1.MyWorkSpace.zip -d /usr/local/UDK2014/
user@machine:~$ tar xfv temp/BaseTools\(Unix\).tar -C /usr/local/UDK2014/MyWorkSpace/
user@machine:~$ rm -rf temp
user@machine:~$ cd /usr/local/UDK2014/MyWorkSpace/BaseTools
user@machine:BaseTools$ make -C Source/C
user@machine:BaseTools$ make
user@machine:MyWorkSpace$ cd ..
user@machine:MyWorkSpace$ source edksetup.sh BaseTools
```
Edit /usr/local/UDK2014/MyWorkSpace/Conf/target.txt
```
Conf/target.txt.patch
	--- Conf/target.txt.orig	2016-08-17 16:11:45.381949000 +0800
	+++ Conf/target.txt	2016-08-17 16:22:32.531987802 +0800
	@@ -23,7 +23,7 @@
	 #                                               build. This line is required if and only if the current
	 #                                               working directory does not contain one or more description
	 #                                               files.
	-ACTIVE_PLATFORM       = Nt32Pkg/Nt32Pkg.dsc
	+ACTIVE_PLATFORM       = MdePkg/MdePkg.dsc

	 #  TARGET                List       Optional    Zero or more of the following: DEBUG, RELEASE, NOOPT
	 #                                               UserDefined; separated by a space character.
	@@ -31,7 +31,7 @@
	 #                                               valid targets specified in the platform description file
	 #                                               will attempt to be built. The following line will build
	 #                                               DEBUG platform target.
	-TARGET                = DEBUG
	+TARGET                = RELEASE

	 #  TARGET_ARCH           List       Optional    What kind of architecture is the binary being target for.
	 #                                               One, or more, of the following, IA32, IPF, X64, EBC, ARM
	@@ -47,7 +47,7 @@
	 #                                               If not specified, then all valid architectures specified
 	 #                                               in the platform file, for which tools are available, will be
	 #                                               built.
	-TARGET_ARCH           = IA32
	+TARGET_ARCH           = X64

	 #  TOOL_DEFINITION_FILE  Filename  Optional   Specify the name of the filename to use for specifying
	 #                                             the tools to use for the build.  If not specified,
	@@ -57,7 +57,7 @@
	 #  TAGNAME               List      Optional   Specify the name(s) of the tools_def.txt TagName to use.
	 #                                             If not specified, all applicable TagName tools will be
	 #                                             used for the build.  The list uses space character separation.
	-TOOL_CHAIN_TAG        = MYTOOLS
	+TOOL_CHAIN_TAG        = GCC49

	 # MAX_CONCURRENT_THREAD_NUMBER  NUMBER  Optional  The number of concurrent threads. Recommend to set this
	 #                                                 value to one more than the number of your compurter
```
Resume the job in the terminal. Run "build" under the directory /usr/local/UDK2014/MyWorkSpace
```bash
user@machine:MyWorkSpace$ build
```
As the building process succeeded, UDK2014 is ready for you to build Refind.
## Download the source code of BOTO
### Download from the git tree
### Download .tar.gz from [SourceCode](.)
The source code .tar.gz file under [SourceCode](.) are named like BotoPkg_Src-x.y.z.tar.gz, where x.y.z stands for a version number. For v0.0.1 the file will be BotoPkg_Src-0.0.1.tar.gz.  
Just click the version under [SourceCode](.) that you want to use, and press *Download* button after the page updated.  
Move the file BotoPkg_Src-x.y.z.tar.gz to your home directory and then run command listed below to untar the file into the directory edk2.
```bash
user@machine:~$ tar xfv BotoPkg_Src-x.y.z.tar.gz -C edk2
```
Now a subdirectory named *BotoPkg* will appears in the *edk2* directory.
## Build Boto Modules
### How to build RunEfiApp
It very simple to build RunEfiApp, just run build command like below within a termial.
```bash
user@machine:edk2$ build -p BotoPkg/BotoPkg.dsc -m BotoPkg/Apps/RunEfiApp/RunEfiApp.inf
```
If your are prompt with "No command 'build' found", it means that you are in the terminall without edk2 environment initialized. Just run command listed below to initialize the environment.
```bash
user@machine:edk2$ source edksetup.sh BaseTools
```
### How to build BotoFont
It very simple to build BotoFont, just run build command like below within a termial.
```bash
user@machine:edk2$ build -p BotoPkg/BotoPkg.dsc -m BotoPkg/fonts/BotoFont/font.inf
```
If your are prompt with "No command 'build' found", it means that you are in the terminall without edk2 environment initialized. Just run command listed below to initialize the environment.
```bash
user@machine:edk2$ source edksetup.sh BaseTools
```
### How to build Refind (Boto Edition)
It's easier than easier for you to build Boto edition Refind. Just type "make" and press ENTER in the terminal under the directory that stands for the source code of Boto Edition Refind.
```bash
user@machine:refind_0.10.3$ make
```
## Install Boto to the real platform
There no independent installer provided. If you want to integrate boto with your OS product, the OS installer should be modified to have boto installed.
For test purpose you can install Boto by a Windows 10 USB Install Media or Ubuntu 16.04.1 USB Install Media. Please refer sections within  [Readme.txt](https://github.com/openthos/boto-analysis/blob/master/Releases/alpha1/Readme.txt) from the [binary Releases/Alpha1](../Releases/alpha1).

## FOR MORE INFORMATION
Please contact me for more information if neccessary.
Mail: [chanuei@sina.com](mailto:chanuei@sina.com) or [chenwei01@thtfpc.com](mailto:chenwei01@thtfpc.com)
