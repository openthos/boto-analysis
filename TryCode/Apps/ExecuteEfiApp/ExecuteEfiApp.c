#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include <Protocol/LoadedImage.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

#define DEBUG
#ifdef DEBUG
  #define DbgPrint(x)     {SystemTable -> ConOut-> OutputString(SystemTable->ConOut, x);}
#else
  #define DbgPrint(x)     {}
#endif

EFI_STATUS
EFIAPI
RunEfiApp(
  IN CONST EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable,
  IN CONST CHAR16               *FilePathName,
  IN CONST CHAR16               *CommandLine OPTIONAL
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
/*  EFI_HANDLE                    NewHandle;*/
  EFI_HANDLE                    EfiAppHandle;
  EFI_LOADED_IMAGE_PROTOCOL     *EfiAppImage;
  EFI_LOADED_IMAGE_PROTOCOL     *LoadedImage;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath = NULL;



  Status = gBS->OpenProtocol(
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (void **)&LoadedImage,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
//  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Started\n");
    
  DbgPrint(L"Started\r\n");
  if(EFI_ERROR(Status))
  {
    DbgPrint(L"Failed to call gBS->OpenProtocol()\n\r");
    return Status;
  }


/*  Status = gBS->HandleProtocol(*/
/*                  ImageHandle,*/
/*                  &gEfiLoadedImageProtocolGuid,*/
/*                  (void **)&LoadedImage*/
/*  );*/
/*  */
/*  if(EFI_ERROR(Status))*/
/*  {*/
/*    DbgPrint(L"Failed to call gBS->HandleProtocol() to get EfiAppImage\r\n");*/
/*    return Status;*/
/*  }*/
  
  Status = gBS->HandleProtocol(
                  LoadedImage->DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  &DevicePath
  );
  
/*  DevicePath = DevicePathFromHandle (LoadedImage->DeviceHandle);*/
  
  CHAR16 *str = NULL;

  str = CatSPrint(NULL,L"Image device : %s\n",ConvertDevicePathToText(DevicePath, FALSE, FALSE));
  
  if(str!=NULL)
  {
    DbgPrint(str);
  }
  else
  {
    DbgPrint(L"Str is NULL\r\n");
  }
  
  str = CatSPrint(NULL,L"Image file: %s\n", ConvertDevicePathToText(LoadedImage->FilePath, TRUE, TRUE));
  
  if(str!=NULL)
  {
    DbgPrint(str);
  }
  else
  {
    DbgPrint(L"Str is NULL\r\n");
  }

  DevicePath = FileDevicePath(LoadedImage->DeviceHandle,FilePathName);
  Status = gBS->LoadImage(
    FALSE,
    ImageHandle,
    DevicePath,
    NULL,
    0,
    &EfiAppHandle);

  if (EFI_ERROR(Status)) {
    if (EfiAppHandle != NULL) {
      gBS->UnloadImage(EfiAppHandle);
    }
  }
  
  Status = gBS->OpenProtocol(
                  EfiAppHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (void **)&EfiAppImage,
                  NULL,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  
  if(EFI_ERROR(Status))
  {
    DbgPrint(L"Failed to call gBS->OpenProtocol() for EfiAppHandle\n\r");
    return Status;
  }

                  
  Status = gBS->HandleProtocol(
                  EfiAppImage->DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  &DevicePath
  );

  str = CatSPrint(NULL,L"Toload Image device : %s\n",ConvertDevicePathToText(DevicePath, FALSE, FALSE));
  
  if(str!=NULL)
  {
    DbgPrint(str);
  }
  else
  {
    DbgPrint(L"Str is NULL\r\n");
  }
  
  str = CatSPrint(NULL,L"Toload Image file: %s\n", ConvertDevicePathToText(EfiAppImage->FilePath, TRUE, TRUE));
  
  if(str!=NULL)
  {
    DbgPrint(str);
  }
  else
  {
    DbgPrint(L"Str is NULL\r\n");
  }

/*  Status = gBS->OpenProtocol(*/
/*                  EfiAppHandle,*/
/*                  &gEfiLoadedImageProtocolGuid,*/
/*                  (void **)&EfiAppImage,*/
/*                  gImageHandle,*/
/*                  NULL,*/
/*                  EFI_OPEN_PROTOCOL_GET_PROTOCOL*/
/*                  );*/
/*  */
/*  if(EFI_ERROR(Status))*/
/*  {*/
/*    DbgPrint(L"Failed to call gBS->OpenProtocol() for EfiAppHandle\n\r");*/
/*    return Status;*/
/*  }*/

  ASSERT(EfiAppImage->LoadOptionsSize == 0);
  
  if (CommandLine != NULL) {
    EfiAppImage->LoadOptionsSize  = (UINT32)StrSize(CommandLine);
    EfiAppImage->LoadOptions      = (VOID*)CommandLine;
  }

  if (EfiAppImage->ImageCodeType == EfiLoaderCode)
  {
    Status = gBS->StartImage(
                    EfiAppHandle,
                    0,
                    NULL
                  );
    if(EFI_ERROR(Status))
    {
      gBS->UnloadImage(EfiAppHandle);
    }
  }
                  
  return(Status);
}

EFI_STATUS
EFIAPI
UefiMain(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SystemTable -> ConOut-> OutputString(SystemTable->ConOut, L"HelloWorld\r\n");
  return RunEfiApp(ImageHandle, SystemTable, L"\\HelloWorld.efi", NULL);
}

