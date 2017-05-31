SET /A COUNT=1
SET /A T1=%TIME:~0,2%*3600+%TIME:~3,2%*60+%TIME:~6,2%
SET /A T2=0
SET /A T=0

ECHO %T% > times.txt
SET /A T1=%TIME:~0,2%*3600+%TIME:~3,2%*60+%TIME:~6,2%
FOR /F "tokens=1,2 skip=%COUNT% delims=TAB" %%G IN (expected.txt) DO CALL test.bat %%G %%H & GOTO :loop
PAUSE

:launchTests
@echo on
SET /A T2=%TIME:~0,2%*3600+%TIME:~3,2%*60+%TIME:~6,2%
SET /A T = %T2%-%T1%
ECHO %T% >> times.txt
SET /A T1=%TIME:~0,2%*3600+%TIME:~3,2%*60+%TIME:~6,2%
FOR /F "tokens=1,2 skip=%COUNT% delims=TAB" %%G IN (expected.txt) DO CALL test.bat %%G %%H & GOTO :loop
PAUSE
SET /A p=(%COUNT%-1)/2
CALL generateresults.bat %p%
GOTO :eof

:loop
@echo off
adb shell dumpsys activity top | FINDSTR ".*Blink_Activity.*" >nul
IF %ERRORLEVEL% == 0 GOTO :loop
SET /A COUNT = %COUNT%+2
GOTO :launchTests
