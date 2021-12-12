clang++ -target x86_64-pc-windows-gnu ./ans.cpp -O3 -D LOCAL
if %ERRORLEVEL% NEQ 0 goto FAILURE

@REM cd tools
@REM call cargo run --release --bin a.exe in.txt out.txt
@REM call a.exe < .\tools\in\0000.txt > out.txt 2> log.txt
@REM if %ERRORLEVEL% NEQ 0 goto FAILURE
powershell -C (Measure-Command {"((Get-Content .\tools\in\0000.txt | .\a.exe > out.txt))"}).TotalMilliseconds
if %ERRORLEVEL% NEQ 0 goto FAILURE


goto END

:FAILURE
echo ERROR STOP: %ERRORLEVEL%

:END
exit /b
