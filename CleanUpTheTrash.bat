@echo off

echo Are you sure you want to remove the collected files from the project?

set /p var="Confirm (y/n)?"
if /i "%var%"=="y" (goto Clean)
if /i "%var%"=="Y" (goto Clean)
goto endProgram

:Clean
echo Cleaning...

rmdir /s /q .\.vs\
rmdir /s /q .\Binaries\
rmdir /s /q .\DerivedDataCache\
rmdir /s /q .\Intermediate\

del "*.sln"

:endProgram
echo Closing...
rem pause