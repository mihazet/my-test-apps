@echo off
REM
REM Collect all mo files for software packaging 
REM
REM $Id$
REM

REM Define configurable variables
set TempFile=TempMoList
set InputFolder=..\locale
set OutputFolder=locale
set AppName=Updater
set executeBinaryParentPath=..\bin

REM Remove previous output
if not %OutputFolder% == %InputFolder% if exist %OutputFolder% rmdir /S /Q %OutputFolder%

REM Create a temp file to list all the mo in locale folder
dir /B %InputFolder%\*.mo > %TempFile%
REM for /F "tokens=1 delims=." %%i in (%TempFile%) do mkdir %OutputFolder%\%%i
REM for /F "tokens=1 delims=." %%i in (%TempFile%) do copy %InputFolder%\%%i.mo %OutputFolder%\%%i\%AppName%.mo
for /F "tokens=1 delims=." %%i in (%TempFile%) do (
    mkdir %OutputFolder%\%%i
    copy %InputFolder%\%%i.mo %OutputFolder%\%%i\%AppName%.mo
)


REM Copy output data to execute binary path for test purpose
dir /B %executeBinaryParentPath% > %TempFile%
for /F "tokens=1" %%i in (%TempFile%) do xcopy /S /F /Y /I %OutputFolder% %executeBinaryParentPath%\%%i\%OutputFolder%

REM Remove temp file
del %TempFile%
