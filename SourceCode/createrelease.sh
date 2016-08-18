#!/bin/bash

GCC_VERSION=49

ARCH=X64

BOTO_REFIND_VERSION=0.10.3

DEST_DIR=efi/boot

BOTOPKG_BIN_TYPE=RELEASE  #If BotoPkg is build in DEBUG MODE, use debug here.

BOTOPKG_BIN_DIR=edk2/Build/BotoPkg/${BOTOPKG_BIN_TYPE}_GCC${GCC_VERSION}/${ARCH}

if [ -d ${DEST_DIR} ]; then #Check if the directory efi/boot already exist.
  rm -rf ${DEST_DIR}
fi

mkdir -p ${DEST_DIR}

cp -av refind-${BOTO_REFIND_VERSION}/drivers_x64 ${DEST_DIR}
cp -av refind-${BOTO_REFIND_VERSION}/refind/refind_x64.efi  ${DEST_DIR}/botoX64.efi
cp -av refind-${BOTO_REFIND_VERSION}/icons ${DEST_DIR}
cp -av refind-${BOTO_REFIND_VERSION}/themes ${DEST_DIR}
cp -av refind-${BOTO_REFIND_VERSION}/refind.conf ${DEST_DIR}

cp -av ${BOTOPKG_BIN_DIR}/font.efi ${DEST_DIR}
cp -av ${BOTOPKG_BIN_DIR}/RunEfiApp.efi ${DEST_DIR}/bootx64.efi

tar zcfv boto_bin.tar.gz ${DEST_DIR}
rm -rf efi




