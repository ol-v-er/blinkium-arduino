@ECHO off

SET /A COUNT=0
SET /A binErr=0
SET /A carErr=0
SET /A nbTests=%1

ECHO "Resultats des tests" > result.txt

:compareBin
CALL compareF.bat %COUNT%
SET /A COUNT = %COUNT%+1 
GOTO :affectErrBin

:affectErrBin
FOR /F "" %%G in (nbErreurs.txt) DO SET /A binErr=%%G & GOTO :compareCar

:compareCar
CALL compareF.bat %COUNT%
SET /A COUNT = %COUNT%+1 
GOTO :affectErrCar

:affectErrCar
FOR /F "" %%G in (nbErreurs.txt) DO SET /A carErr=%%G & GOTO :writeResults

:writeResults
SET /A line=%COUNT%/2
SET /A errCar=%carErr%/8
IF %binErr% == 0 (
FOR /F "skip=%line%" %%I in (times.txt) DO ECHO OK;%binErr%;%errCar%;%%I >> result.txt & GOTO :testFin
)
FOR /F "skip=%line%" %%I in (times.txt) DO ECHO KO;%binErr%;%errCar%;%%I >> result.txt & GOTO :testFin
:testFin
SET /A n=%nbTests%*2
IF %COUNT% LSS %n% (
GOTO :compareBin
)
ECHO "Tests Termines"
PAUSE
GOTO cleanup

:cleanup
DEL nbErreurs.txt
DEL receivedT.txt
DEL expectedT.txt
GOTO :eof


