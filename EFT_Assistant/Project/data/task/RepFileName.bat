@echo off

rem ------- 置換前・置換後の文字列を設定 -------
set TARGET=.txt.txt
set REPLACE_WITH=.txt
rem -----------------------------------------

for /r %%F in ( * ) do call :sub "%%F", "%%~nxF"
pause
exit /b

:sub
set FILE_PATH=%1
set FILE_NAME=%2
echo %FILE_NAME%
echo %FILE_NAME%
call set FILE_NAME=%%FILE_NAME:%TARGET%=%REPLACE_WITH%%%
ren %1 %FILE_NAME%

goto :EOF