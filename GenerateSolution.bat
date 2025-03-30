@echo off
setlocal enabledelayedexpansion
echo Creating directories for build
echo ===================================================================================================================
:: Get the directory where the .bat file is located.
set rootDir=%~dp0

:: Set paths where new directories will be created ('bin/Dev', 'bin/Release', 'bin-int/Dev', 'bin-int/Release').
set binDevDir       =%rootDir%\bin\Dev
set binRelDir       =%rootDir%\bin\Release
set binDevTestDir   =%binDevDir%\Tests
set binRelTestDir   =%binRelDir%\Tests

set intDevDir       =%rootDir%\bin-int\Dev
set intRelDir       =%rootDir%\bin-int\Release
set intDevTestDir   =%intDevDir%\Tests
set intRelTestDir   =%intRelDir%\Tests

:: Make sure the base directories exist.
if not exist "%binDevDir%" mkdir "%binDevDir%"
if not exist "%binRelDir%" mkdir "%binRelDir%"
if not exist "%binDevTestDir%" mkdir "%binDevTestDir%"
if not exist "%binRelTestDir%" mkdir "%binRelTestDir%"

if not exist "%intDevDir%" mkdir "%intDevDir%"
if not exist "%intRelDir%" mkdir "%intDevDir%"
if not exist "%intDevTestDir%" mkdir "%intDevTestDir%"
if not exist "%intRelTestDir%" mkdir "%intRelTestDir%"

:: Recursively loop through all directories starting from the .bat file's directory.
for /r "%rootDir%" %%d in (.) do (
    set dirName=%%~nd

    :: Skip the directory if its name is "ScarlettProject".
    if /i "!dirName!"=="ScarlettProject" (
        echo Skipping directory: !dirName!
    ) else (
        :: Check if premake5.lua exists in the current directory.
        if exist "%%d\premake5.lua" (
            :: Check if the directory name contains "Test".
            echo !dirName! | findstr /i "Test" >nul
            if !errorlevel! equ 0 (
                :: If "Test" is found, create the directories in the 'Tests' subfolder.
                if not exist "%binDevTestDir%\!dirName!" (
                    mkdir "%binDevTestDir%\!dirName!"
                    echo Created directory: %binDevTestDir%\!dirName!
                ) else (
                    echo Directory 'bin/Dev/Tests/!dirName!' exists, skipping...
                )
                if not exist "%binRelTestDir%\!dirName!" (
                    mkdir "%binRelTestDir%\!dirName!"
                    echo Created directory: %binRelTestDir%\!dirName!
                ) else (
                    echo Directory 'bin/Release/Tests/!dirName!' exists, skipping...
                )
                if not exist "%intDevTestDir%\!dirName!" (
                    mkdir "%intDevTestDir%\!dirName!"
                    echo Created directory: %intDevTestDir%\!dirName!
                ) else (
                    echo Directory 'bin-int/Dev/Tests/!dirName!' exists, skipping...
                )
                if not exist "%intRelTestDir%\!dirName!" (
                    mkdir"%intRelTestDir%\!dirName!"
                    echo Created directory: %intRelTestDir%\!dirName!
                ) else (
                    echo Directory 'bin-int/Release/Tests/!dirName!' exists, skipping...
                )
            ) else (
                :: Otherwise, create the directories in the respective 'bin' folders.
                if not exist "%binDevDir%\!dirName!" (
                    mkdir "%binDevDir%\!dirName!"
                    echo Created directory: %binDevDir%\!dirName!
                ) else (
                    echo Directory 'bin/Dev/!dirName!' exists, skipping...
                )
                if not exist "%binRelDir%\!dirName!" (
                    mkdir "%binRelDir%\!dirName!"
                    echo Created directory: %binRelDir%\!dirName!
                ) else (
                    echo Directory 'bin/Release/!dirName!' exists, skipping...
                )
                if not exist "%intDevDir%\!dirName!" (
                    mkdir "%intDevDir%\!dirName!"
                    echo Created directory: %intDevDir%\!dirName!
                ) else (
                     echo Directory 'bin-int/Dev/!dirName!' exists, skipping...
                 )
                if not exist "%intRelDir%\!dirName!" (
                    mkdir "%intRelDir%\!dirName!" 
                    echo Created directory: %intRelDir%\!dirName!
                ) else (
                    echo Directory 'bin-int/Release/!dirName!' exists, skipping...
                )
            )
        )
    )
)

echo Done creating directories for Scarlett Project.

echo ===================================================================================================================

echo Generating Solution files
call deps\premake\premake5.exe vs2022
echo Done generating solution files

echo ===================================================================================================================

endlocal

pause