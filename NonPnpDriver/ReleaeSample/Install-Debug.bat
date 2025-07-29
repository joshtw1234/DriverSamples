echo %CD%
SET MYPATH=%~dp0
echo %MYPATH:~0,-1%

xcopy %MYPATH%DebugDlls\*.* C:\Windows\System32 /Y/D

sc stop HPNonPnpDriver
sc delete HPNonPnpDriver

del "%WINDIR%\System32\Drivers\*HPNonPnpDriver*"

xcopy /Y/D ".\HPNonPnpDriver.sys" "%WINDIR%\System32\Drivers\"

sc create HPNonPnpDriver binpath= "%WINDIR%\System32\Drivers\HPNonPnpDriver.sys" type= kernel start= auto
sc start HPNonPnpDriver

pause