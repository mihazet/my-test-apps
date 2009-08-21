@echo off

REM generate database file from SQL script.
if exist SimCube.db del SimCube.db
sqlite3 SimCube.db < SimCube.sql

REM copy database file to executable paths
for /f %%d in ('dir /b/ad ..\bin') do (
    if not %%d == .svn copy /Y SimCube.db ..\bin\%%d
)
