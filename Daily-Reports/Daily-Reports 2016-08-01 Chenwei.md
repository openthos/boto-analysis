# Daily Report 2016-08-01  by Chenwei

##Current Statement

Chinese Support is ready and refind is modified to support display for Chinese. As shown below.(Note: As in Emulator, No OS ICON SHOWN)
![Alt text](https://github.com/openthos/boto-analysis/blob/master/Screenshots/Screenshot%20from%202016-08-01%2017-04-48.png?raw=true)

##Todo List for BOTO
1. Background transparent of the TEXT to be figured.
2. font.efi the font support package need to be combined with Refind.

##Problem Caused by
1. Background transparent problem.
  * The image buffer filled by HII function is not transparent
  * Refind architecture access the video frambuffer directly.  
2. Packages combination problem.
  * Refind uses UDK2014 to compile the the code. If UDK2015 or current edk2 is used, It will die.
  * UDK2014 does not support HII feature.
  * Install font package to the UEFI system need HII feature.
  * Refind should be the first app loaded by uefi to act as boto
  * But to support Chinese, fontpak should be load first.
  Thus the question.

##What have been down today
A lot of codes from MdePkg and ShellPkg reviewed.  
Got the Idea about the combination of the two package.  
<pre>
  <code>
/**
  internal worker function to load and run an image via device path.

  @param ParentImageHandle      A handle of the image that is executing the specified
                                command line.
  @param DevicePath             device path of the file to execute
  @param CommandLine            Points to the NULL-terminated UCS-2 encoded string
                                containing the command line. If NULL then the command-
                                line will be empty.
  @param Environment            Points to a NULL-terminated array of environment
                                variables with the format 'x=y', where x is the
                                environment variable name and y is the value. If this
                                is NULL, then the current shell environment is used.
                            
  @param[out] StartImageStatus  Returned status from gBS->StartImage.

  @retval EFI_SUCCESS       The command executed successfully. The  status code
                            returned by the command is pointed to by StatusCode.
  @retval EFI_INVALID_PARAMETER The parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES Out of resources.
  @retval EFI_UNSUPPORTED   Nested shell invocations are not allowed.
**/
EFI_STATUS
EFIAPI
InternalShellExecuteDevicePath(
  IN CONST EFI_HANDLE               *ParentImageHandle,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN CONST CHAR16                   *CommandLine OPTIONAL,
  IN CONST CHAR16                   **Environment OPTIONAL,
  OUT EFI_STATUS                    *StartImageStatus OPTIONAL
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    StartStatus;
  EFI_STATUS                    CleanupStatus;
  EFI_HANDLE                    NewHandle;
  EFI_LOADED_IMAGE_PROTOCOL     *LoadedImage;
  LIST_ENTRY                    OrigEnvs;
  EFI_SHELL_PARAMETERS_PROTOCOL ShellParamsProtocol;
  CHAR16                        *ImagePath;
  UINTN                         Index;
  CHAR16                        *Walker;
  CHAR16                        *NewCmdLine;

  if (ParentImageHandle == NULL) {
    return (EFI_INVALID_PARAMETER);
  }

  InitializeListHead(&OrigEnvs);
  ZeroMem(&ShellParamsProtocol, sizeof(EFI_SHELL_PARAMETERS_PROTOCOL));

  NewHandle = NULL;
  
  NewCmdLine = AllocateCopyPool (StrSize (CommandLine), CommandLine);
  if (NewCmdLine == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Walker = NewCmdLine; Walker != NULL && *Walker != CHAR_NULL ; Walker++) {
    if (*Walker == L'^' && *(Walker+1) == L'#') {
      CopyMem(Walker, Walker+1, StrSize(Walker) - sizeof(Walker[0]));
    }
  }

  //
  // Load the image with:
  // FALSE - not from boot manager and NULL, 0 being not already in memory
  //
  Status = gBS->LoadImage(
    FALSE,
    *ParentImageHandle,
    (EFI_DEVICE_PATH_PROTOCOL*)DevicePath,
    NULL,
    0,
    &NewHandle);

  if (EFI_ERROR(Status)) {
    if (NewHandle != NULL) {
      gBS->UnloadImage(NewHandle);
    }
    FreePool (NewCmdLine);
    return (Status);
  }
  Status = gBS->OpenProtocol(
    NewHandle,
    &gEfiLoadedImageProtocolGuid,
    (VOID**)&LoadedImage,
    gImageHandle,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL);

  if (!EFI_ERROR(Status)) {
    //
    // If the image is not an app abort it.
    //
    if (LoadedImage->ImageCodeType != EfiLoaderCode){
      ShellPrintHiiEx(
        -1, 
        -1, 
        NULL,
        STRING_TOKEN (STR_SHELL_IMAGE_NOT_APP),
        ShellInfoObject.HiiHandle
      );
      goto UnloadImage;
    }

    ASSERT(LoadedImage->LoadOptionsSize == 0);
    if (NewCmdLine != NULL) {
      LoadedImage->LoadOptionsSize  = (UINT32)StrSize(NewCmdLine);
      LoadedImage->LoadOptions      = (VOID*)NewCmdLine;
    }

    //
    // Save our current environment settings for later restoration if necessary
    //
    if (Environment != NULL) {
      Status = GetEnvironmentVariableList(&OrigEnvs);
      if (!EFI_ERROR(Status)) {
        Status = SetEnvironmentVariables(Environment);
      }
    }

    //
    // Initialize and install a shell parameters protocol on the image.
    //
    ShellParamsProtocol.StdIn   = ShellInfoObject.NewShellParametersProtocol->StdIn;
    ShellParamsProtocol.StdOut  = ShellInfoObject.NewShellParametersProtocol->StdOut;
    ShellParamsProtocol.StdErr  = ShellInfoObject.NewShellParametersProtocol->StdErr;
    Status = UpdateArgcArgv(&ShellParamsProtocol, NewCmdLine, Efi_Application, NULL, NULL);
    ASSERT_EFI_ERROR(Status);
    //
    // Replace Argv[0] with the full path of the binary we're executing:
    // If the command line was "foo", the binary might be called "foo.efi".
    // "The first entry in [Argv] is always the full file path of the
    //  executable" - UEFI Shell Spec section 2.3
    //
    ImagePath = EfiShellGetFilePathFromDevicePath (DevicePath);
    // The image we're executing isn't necessarily in a filesystem - it might
    // be memory mapped. In this case EfiShellGetFilePathFromDevicePath will
    // return NULL, and we'll leave Argv[0] as UpdateArgcArgv set it.
    if (ImagePath != NULL) {
      if (ShellParamsProtocol.Argv == NULL) {
        // Command line was empty or null.
        // (UpdateArgcArgv sets Argv to NULL when CommandLine is "" or NULL)
        ShellParamsProtocol.Argv = AllocatePool (sizeof (CHAR16 *));
        if (ShellParamsProtocol.Argv == NULL) {
          Status = EFI_OUT_OF_RESOURCES;
          goto UnloadImage;
        }
        ShellParamsProtocol.Argc = 1;
      } else {
        // Free the string UpdateArgcArgv put in Argv[0];
        FreePool (ShellParamsProtocol.Argv[0]);
      }
      ShellParamsProtocol.Argv[0] = ImagePath;
    }

    Status = gBS->InstallProtocolInterface(&NewHandle, &gEfiShellParametersProtocolGuid, EFI_NATIVE_INTERFACE, &ShellParamsProtocol);
    ASSERT_EFI_ERROR(Status);

    ///@todo initialize and install ShellInterface protocol on the new image for compatibility if - PcdGetBool(PcdShellSupportOldProtocols)

    //
    // now start the image and if the caller wanted the return code pass it to them...
    //
    if (!EFI_ERROR(Status)) {
      StartStatus      = gBS->StartImage(
                          NewHandle,
                          0,
                          NULL
                          );
      if (StartImageStatus != NULL) {
        *StartImageStatus = StartStatus;
      }

      CleanupStatus = gBS->UninstallProtocolInterface(
                            NewHandle,
                            &gEfiShellParametersProtocolGuid,
                            &ShellParamsProtocol
                            );
      ASSERT_EFI_ERROR(CleanupStatus);

      goto FreeAlloc;
    }

UnloadImage:
    // Unload image - We should only get here if we didn't call StartImage
    gBS->UnloadImage (NewHandle);

FreeAlloc:
    // Free Argv (Allocated in UpdateArgcArgv)
    if (ShellParamsProtocol.Argv != NULL) {
      for (Index = 0; Index < ShellParamsProtocol.Argc; Index++) {
        if (ShellParamsProtocol.Argv[Index] != NULL) {
          FreePool (ShellParamsProtocol.Argv[Index]);
        }
      }
      FreePool (ShellParamsProtocol.Argv);
    }
  }

  // Restore environment variables
  if (!IsListEmpty(&OrigEnvs)) {
    CleanupStatus = SetEnvironmentVariableList(&OrigEnvs);
    ASSERT_EFI_ERROR (CleanupStatus);
  }

  FreePool (NewCmdLine);

  return(Status);
}

/**
  internal worker function to load and run an image in the current shell.

  @param CommandLine            Points to the NULL-terminated UCS-2 encoded string
                                containing the command line. If NULL then the command-
                                line will be empty.
  @param Environment            Points to a NULL-terminated array of environment
                                variables with the format 'x=y', where x is the
                                environment variable name and y is the value. If this
                                is NULL, then the current shell environment is used.
                
  @param[out] StartImageStatus  Returned status from the command line.

  @retval EFI_SUCCESS       The command executed successfully. The  status code
                            returned by the command is pointed to by StatusCode.
  @retval EFI_INVALID_PARAMETER The parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES Out of resources.
  @retval EFI_UNSUPPORTED   Nested shell invocations are not allowed.
**/
EFI_STATUS
EFIAPI
InternalShellExecute(
  IN CONST CHAR16                   *CommandLine OPTIONAL,
  IN CONST CHAR16                   **Environment OPTIONAL,
  OUT EFI_STATUS                    *StartImageStatus OPTIONAL
  )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    CleanupStatus;
  LIST_ENTRY                    OrigEnvs;

  InitializeListHead(&OrigEnvs);

  //
  // Save our current environment settings for later restoration if necessary
  //
  if (Environment != NULL) {
    Status = GetEnvironmentVariableList(&OrigEnvs);
    if (!EFI_ERROR(Status)) {
      Status = SetEnvironmentVariables(Environment);
    } else {
      return Status;
    }
  }

  Status = RunShellCommand(CommandLine, StartImageStatus);

  // Restore environment variables
  if (!IsListEmpty(&OrigEnvs)) {
    CleanupStatus = SetEnvironmentVariableList(&OrigEnvs);
    ASSERT_EFI_ERROR (CleanupStatus);
  }

  return(Status);
}

/**
  Determine if the UEFI Shell is currently running with nesting enabled or disabled.

  @retval FALSE   nesting is required
  @retval other   nesting is enabled
**/
STATIC
BOOLEAN
EFIAPI
NestingEnabled(
)
{
  EFI_STATUS  Status;
  CHAR16      *Temp;
  CHAR16      *Temp2;
  UINTN       TempSize;
  BOOLEAN     RetVal;

  RetVal = TRUE;
  Temp   = NULL;
  Temp2  = NULL;

  if (ShellInfoObject.ShellInitSettings.BitUnion.Bits.NoNest) {
    TempSize = 0;
    Temp     = NULL;
    Status = SHELL_GET_ENVIRONMENT_VARIABLE(mNoNestingEnvVarName, &TempSize, Temp);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      Temp = AllocateZeroPool(TempSize + sizeof(CHAR16));
      if (Temp != NULL) {
        Status = SHELL_GET_ENVIRONMENT_VARIABLE(mNoNestingEnvVarName, &TempSize, Temp);
      }
    }
    Temp2 = StrnCatGrow(&Temp2, NULL, mNoNestingTrue, 0);
    if (Temp != NULL && Temp2 != NULL && StringNoCaseCompare(&Temp, &Temp2) == 0) {
      //
      // Use the no nesting method.
      //
      RetVal = FALSE;
    }
  }

  SHELL_FREE_NON_NULL(Temp);
  SHELL_FREE_NON_NULL(Temp2);
  return (RetVal);
}

/**
  Execute the command line.

  This function creates a nested instance of the shell and executes the specified
  command (CommandLine) with the specified environment (Environment). Upon return,
  the status code returned by the specified command is placed in StatusCode.

  If Environment is NULL, then the current environment is used and all changes made
  by the commands executed will be reflected in the current environment. If the
  Environment is non-NULL, then the changes made will be discarded.

  The CommandLine is executed from the current working directory on the current
  device.

  @param ParentImageHandle  A handle of the image that is executing the specified
                            command line.
  @param CommandLine        Points to the NULL-terminated UCS-2 encoded string
                            containing the command line. If NULL then the command-
                            line will be empty.
  @param Environment        Points to a NULL-terminated array of environment
                            variables with the format 'x=y', where x is the
                            environment variable name and y is the value. If this
                            is NULL, then the current shell environment is used.
  @param StatusCode         Points to the status code returned by the CommandLine.

  @retval EFI_SUCCESS       The command executed successfully. The  status code
                            returned by the command is pointed to by StatusCode.
  @retval EFI_INVALID_PARAMETER The parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES Out of resources.
  @retval EFI_UNSUPPORTED   Nested shell invocations are not allowed.
  @retval EFI_UNSUPPORTED   The support level required for this function is not present.

  @sa InternalShellExecuteDevicePath
**/
EFI_STATUS
EFIAPI
EfiShellExecute(
  IN EFI_HANDLE *ParentImageHandle,
  IN CHAR16 *CommandLine OPTIONAL,
  IN CHAR16 **Environment OPTIONAL,
  OUT EFI_STATUS *StatusCode OPTIONAL
  )
{
  EFI_STATUS                Status;
  CHAR16                    *Temp;
  EFI_DEVICE_PATH_PROTOCOL  *DevPath;
  UINTN                     Size;

  if ((PcdGet8(PcdShellSupportLevel) < 1)) {
    return (EFI_UNSUPPORTED);
  }

  if (NestingEnabled()) {
    DevPath = AppendDevicePath (ShellInfoObject.ImageDevPath, ShellInfoObject.FileDevPath);

    DEBUG_CODE_BEGIN();
    Temp = ConvertDevicePathToText(ShellInfoObject.FileDevPath, TRUE, TRUE);
    FreePool(Temp);
    Temp = ConvertDevicePathToText(ShellInfoObject.ImageDevPath, TRUE, TRUE);
    FreePool(Temp);
    Temp = ConvertDevicePathToText(DevPath, TRUE, TRUE);
    FreePool(Temp);
    DEBUG_CODE_END();

    Temp = NULL;
    Size = 0;
    ASSERT((Temp == NULL && Size == 0) || (Temp != NULL));
    StrnCatGrow(&Temp, &Size, L"Shell.efi -_exit ", 0);
    StrnCatGrow(&Temp, &Size, CommandLine, 0);

    Status = InternalShellExecuteDevicePath(
      ParentImageHandle,
      DevPath,
      Temp,
      (CONST CHAR16**)Environment,
      StatusCode);

    //
    // de-allocate and return
    //
    FreePool(DevPath);
    FreePool(Temp);
  } else {
    Status = InternalShellExecute(
      (CONST CHAR16*)CommandLine,
      (CONST CHAR16**)Environment,
      StatusCode);
  }

  return(Status);
}
  </code>
</pre>

## Next Step
  1. Make clear how gBS->LoadImage works and how to build a DevicePath of font.efi will help to solve the problem 2.
  2. Crop the background image where the text will occupy, and render them together will help to solve the problem 1.
