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

python E:\Programming\ScarlettProject\Build\GenerateRTTI\GenerateRTTI.py

echo ===================================================================================================================

:: Regenerate solution files.
cd /d "%generateSolutionDir%"
call GenerateSolution.bat

pause
