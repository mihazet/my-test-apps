;-------------------------------------------------------------------------
; Realta Updater (Un)Installer
;
; $Id$
;-------------------------------------------------------------------------

;-------------------------------------
; Include header

    !include "MUI2.nsh"

;-------------------------------------

	!define COMPANY		"Delta Electronics, Inc."
	!define APPNAME		"T2Updater"
	!define APPVERSION	"1.0.0"

;-------------------------------------
; General

    ; name and file
    Name "T2 Updater"
    OutFile "${APPNAME}-Installer-${APPVERSION}.exe"

    ; default installation directory
    InstallDir "$PROGRAMFILES\${COMPANY}\${APPNAME}"
    
    ;Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\${COMPANY}\${APPNAME}" ""

    ;Request application privileges for Windows Vista
    RequestExecutionLevel user

;--------------------------------
;Interface Settings

	; Page header
	!define MUI_ICON stdInstaller.ico
	; Interface resources
	!define MUI_UI "${NSISDIR}\Contrib\UIs\modern.exe"
	!define MUI_UI_COMPONENTSPAGE_SMALLDESC "${NSISDIR}\Contrib\UIs\modern_smalldesc.exe"
	; Installer welcome/finish page
	!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-nsis.bmp"
	; Uninstaller welcome/finish page
	!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall-nsis.bmp"
	; License page
	; Component page
	!define MUI_COMPONENTSPAGE_CHECKBITMAP "${NSISDIR}\Contrib\Graphics\Checks\modern.bmp"
	;!define MUI_COMPONENTSPAGE_NODESC
	!define MUI_COMPONENTSPAGE_SMALLDESC
	; Directory page
	!define MUI_DIRECTORYPAGE_BGCOLOR "FFFFFF"
	; Start menu folder page
	!define MUI_STARTMENUPAGE_BGCOLOR "FFFFFF"
	; Installation page
	!define MUI_INSTFILESPAGE_COLORS /windows
	!define MUI_INSTFILESPAGE_PROGRESSBAR smooth
	; Installer finish page
	!define MUI_FINISHPAGE_NOAUTOCLOSE
	; Abort warning
	!define MUI_ABORTWARNING
	!define MUI_ABORTWARNING_CANCEL_DEFAULT
	; Uninstaller abort warning
	!define MUI_UNABORTWARNING
	!define MUI_UNABORTWARNING_CANCEL_DEFAULT

	BRANDINGTEXT "${COMPANY}"

;--------------------------------
;Language Selection Dialog Settings

	;Remember the installer language
	!define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
	!define MUI_LANGDLL_REGISTRY_KEY "Software\${COMPANY}\${APPNAME}" 
	!define MUI_LANGDLL_REGISTRY_VALUENAME "InstallerLanguage"
	!define MUI_LANGDLL_WINDOWTITLE "Select preferred language"
	!define MUI_LANGDLL_INFO "Please select language used during installing procedure"
	!define MUI_LANGDLL_ALWAYSSHOW
	;!define MUI_LANGDLL_ALLLANGUAGES
  
;--------------------------------
;Pages
	;Var StartMenuFolder

	!insertmacro MUI_PAGE_INIT
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
	!insertmacro MUI_PAGE_COMPONENTS
	;!insertmacro MUI_PAGE_STARTMENU page_id "$StartMenuFolder"
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_PAGE_FINISH

	!insertmacro MUI_UNPAGE_WELCOME
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
	!insertmacro MUI_UNPAGE_FINISH
  
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
InstType Full
InstType Minimal
;--------------------------------
;Installer Sections

Section "Updater" MainApp

	SectionIn 1 2 RO
	SetOutPath "$INSTDIR"

	;ADD YOUR OWN FILES HERE...
	File "..\..\bin\VC9ProjectDebug\Updater.exe"
	
	;Store installation folder
	WriteRegStr HKCU "Software\${COMPANY}\${APPNAME}" "" $INSTDIR

	;Create uninstaller
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	CreateDirectory "$SMPROGRAMS\${COMPANY}\${APPNAME}"
	CreateShortCut "$SMPROGRAMS\${COMPANY}\${APPNAME}\Updater.lnk" "$INSTDIR\Updater.exe"
	CreateShortCut "$SMPROGRAMS\${COMPANY}\${APPNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	CreateShortCut "$QUICKLAUNCH\Updater.lnk" "$INSTDIR\Updater.exe"
	
SectionEnd

SubSection "Language files" Locale

Section "English" locale_en

	SectionIn 1
	CreateDirectory "$INSTDIR\locale\en"
	SetOutPath "$INSTDIR\locale\en"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\en\Updater.mo"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\en\wxstd.mo"	

SectionEnd

Section "Chinese (Traditional)" locale_zh_TW

	SectionIn 1
	CreateDirectory "$INSTDIR\locale\zh_TW"
	SetOutPath "$INSTDIR\locale\zh_TW"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\zh_TW\Updater.mo"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\zh_TW\wxstd.mo"	

SectionEnd

Section "Chinese (Simplified)" locale_zh_CN

	SectionIn 1
	CreateDirectory "$INSTDIR\locale\zh_CN"
	SetOutPath "$INSTDIR\locale\zh_CN"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\zh_CN\Updater.mo"
	File /nonfatal "..\..\bin\VC9ProjectDebug\locale\zh_CN\wxstd.mo"	

SectionEnd

SubSectionEnd

;--------------------------------
;Installer Functions

Function .onInit

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Descriptions

	;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGAUGE SPECIFIC
	;Language strings
	!include "lang-english.nsh"
	!include "lang-simpchinese.nsh"
	!include "lang-tradchinese.nsh"

	;Assign language strings to sections
	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${MainApp} $(DESC_MainApp)
	!insertmacro MUI_DESCRIPTION_TEXT ${Locale} $(DESC_Locale)
	!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

	;ADD YOUR OWN FILES HERE...
	Delete "$INSTDIR\Updater.exe"
	Delete "$INSTDIR\appoptions.db"
	Delete "$INSTDIR\Uninstall.exe"  
	RMDir /r "$INSTDIR\locale"
	RMDir /r "$INSTDIR\report"
	RMDir "$INSTDIR"
	
	Delete "$SMPROGRAMS\${COMPANY}\${APPNAME}\Updater.lnk"
	Delete "$SMPROGRAMS\${COMPANY}\${APPNAME}\Uninstall.lnk"
	Delete "$QUICKLAUNCH\Updater.lnk"
	RMDir "$SMPROGRAMS\${COMPANY}\${APPNAME}"
	RMDir "$SMPROGRAMS\${COMPANY}"
	
	DeleteRegKey /ifempty HKCU "Software\${COMPANY}\${APPNAME}"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

	!insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd
