!define PRODUCT_NAME "aupinstall"
!define PRODUCT_VERSION "1.01"
!define pkgdir "C:\Qt\Qt_project\build-AutoUpdateClient-Desktop_Qt_5_9_1_MSVC2015_64bit-Release"

Name "${PRODUCT_NAME}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"

;SilentInstall silent

Section	
  SetOutPath $INSTDIR
  File "${pkgdir}\auplib.dll"
  File "${pkgdir}\AutoUpdateClient.exe"
  Sleep 2000
  Quit
SectionEnd