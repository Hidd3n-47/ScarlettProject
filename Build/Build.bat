@echo off
setlocal

:: Get the current script directory
set "currentDir=%~dp0"
for %%i in ("%~dp0..") do set "generateSolutionDir=%%~fi"

:: Change to build directory
cd /d "%currentDir%"

:: Run the directory creation script.
call CreateBuildDirectories.bat

:: Run RTTI generator.
echo ===================================================================================================================
echo ^| Creating RTTI - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -^|
echo ===================================================================================================================
::python "GenerateRTTI.py"
cd GenerateRTTI\dist\GenerateRTTI
call GenerateRTTI.exe
echo ===================================================================================================================

:: Regenerate solution files.
cd /d "%generateSolutionDir%"
call GenerateSolution.bat

pause
