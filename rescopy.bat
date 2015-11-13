@REM Inserti in Code-blocks Project root
@REM Pre-Build Steps: cmd /c rescopy
@%WINDIR%\system32\robocopy res\ bin\Debug\ /MIR /XF *.exe /NFL /NDL /NJH /NJS /nc /ns /np
@SET ERRNO = %ERRORLEVEL%
@if %ERRORLEVEL% EQU 16 echo RES COPY RESULT: ***FATAL ERROR*** & goto error
@if %ERRORLEVEL% EQU 15 echo RES COPY RESULT: OKCOPY + FAIL + MISMATCHES + XTRA & goto error
@if %ERRORLEVEL% EQU 14 echo RES COPY RESULT: FAIL + MISMATCHES + XTRA & goto error
@if %ERRORLEVEL% EQU 13 echo RES COPY RESULT: OKCOPY + FAIL + MISMATCHES & goto error
@if %ERRORLEVEL% EQU 12 echo RES COPY RESULT: FAIL + MISMATCHES& goto error
@if %ERRORLEVEL% EQU 11 echo RES COPY RESULT: OKCOPY + FAIL + XTRA & goto end
@if %ERRORLEVEL% EQU 10 echo RES COPY RESULT: FAIL + XTRA & goto error
@if %ERRORLEVEL% EQU 9 	echo RES COPY RESULT: OKCOPY + FAIL & goto end
@if %ERRORLEVEL% EQU 8 	echo RES COPY RESULT: FAIL & goto error
@if %ERRORLEVEL% EQU 7 	echo RES COPY RESULT: OKCOPY + MISMATCHES + XTRA & goto end
@if %ERRORLEVEL% EQU 6 	echo RES COPY RESULT: MISMATCHES + XTRA & goto end
@if %ERRORLEVEL% EQU 5 	echo RES COPY RESULT: OKCOPY + MISMATCHES & goto end
@if %ERRORLEVEL% EQU 4 	echo RES COPY RESULT: MISMATCHES & goto end
@if %ERRORLEVEL% EQU 3 	echo RES COPY RESULT: OKCOPY + XTRA & goto end
@if %ERRORLEVEL% EQU 2 	echo RES COPY RESULT: No Change + XTRA & goto end
@if %ERRORLEVEL% EQU 1 	echo RES COPY RESULT: OKCOPY & goto end
@if %ERRORLEVEL% EQU 0 	echo RES COPY RESULT: No Change & goto end
:error
@EXIT /B %ERRNO%
:end
