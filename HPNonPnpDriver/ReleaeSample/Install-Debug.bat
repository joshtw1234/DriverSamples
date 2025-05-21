echo %CD%
SET MYPATH=%~dp0
echo %MYPATH:~0,-1%

xcopy %MYPATH%DebugDlls\*.* C:\Windows\System32 /Y/D

pause