!define PRODUCT_NAME "aupinstall"
!define PRODUCT_VERSION "1.0.2"
!define pkgdir "C:\Qt\Qt_project\build-AutoUpdateClient-Desktop_Qt_5_9_1_MSVC2015_64bit-Release"

VIProductVersion 1.0.1.0
VIAddVersionKey "ProductVersion" ${PRODUCT_VERSION}

Name "${PRODUCT_NAME}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"

;SilentInstall silent

Section	
  SetOutPath $INSTDIR
  SetRegView 64
  WriteRegStr HKLM "SOFTWARE\AutoUpdateClient" "Version" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "SOFTWARE\AutoUpdateClient" "InstallDir" "$INSTDIR"
  File "${pkgdir}\auplib.dll"
  File "${pkgdir}\AutoUpdateClient.exe"
  Sleep 2000
  Quit
SectionEnd