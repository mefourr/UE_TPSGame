@echo off

call "%~dp0\..\confing.bat"

FOR /R "%SourceCodePath%" %%f IN (*.cpp, *.h, *.cs) DO (
    clang-format -i "%%f"
    echo %%f
) 
PAUSE