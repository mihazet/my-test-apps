;-------------------------------------------------------------------------
; SimCube (Un)Installer
;
; $Id$
;-------------------------------------------------------------------------

;-------------------------------------
; Include header

    !include "MUI2.nsh"

;-------------------------------------
; General

    ; name and file
    Name "SimCube"
    OutFile "SimCube-Installer.exe"

    ; default installation directory
    InstallDir "$LOCALAPPDATA\SimCube"
    
    ;Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\Delta Electronics, Inc.\SimCube" ""

    ;Request application privileges for Windows Vista
    RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  ;!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  ;!define MUI_LANGDLL_REGISTRY_KEY "Software\Delta Electronics, Inc.\SimCube" 
  ;!define MUI_LANGDLL_REGISTRY_VALUENAME "InstallerLanguage"
  
;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Delta Electronics, Inc.\SimCube" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Descriptions

  ;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGAUGE SPECIFIC
  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."
  LangString DESC_SecDummy ${LANG_SIMPCHINESE} "?"
  LangString DESC_SecDummy ${LANG_TRADCHINESE} "Ác¤¤´ú¸Õ"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Delta Electronics, Inc.\SimCube"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd
