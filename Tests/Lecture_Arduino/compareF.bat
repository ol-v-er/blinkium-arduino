IF %1 == 0 (
	FOR /F "" %%G IN (expected.txt) DO ECHO %%G > expectedT.txt & GOTO :suite 

	:suite
	FOR /F "" %%G IN (received.txt) DO ECHO %%G > receivedT.txt & GOTO :compare
)

FOR /F "skip=%1" %%G IN (expected.txt) DO ECHO %%G > expectedT.txt & GOTO :exportRec
:exportRec
FOR /F "skip=%1" %%G IN (received.txt) DO ECHO %%G > receivedT.txt & GOTO :compare
	

:compare
FC /B expectedT.txt receivedT.txt | find /c "0000" > nbErreurs.txt
GOTO :eof



