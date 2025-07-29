param (
	[int]$IsDebugRoot = 0
)

if ($IsDebugRoot -eq 1){
	$DebugROOT='Release'
}
else{
	$DebugROOT='Debug'
}


xcopy /Y/D/S/E ..\exe\x64\$DebugROOT\*.exe .
xcopy /Y/D/S/E ..\sys\x64\$DebugROOT\HPNonPnpDriver\*.sys .
xcopy /Y/D/S/E ..\exe\x64\$DebugROOT\*.pdb .
xcopy /Y/D/S/E ..\sys\x64\$DebugROOT\*.pdb .
xcopy /Y/D/S/E "C:\Program Files (x86)\Windows Kits\8.0\redist\wdf\x64\WdfCoInstaller01011.dll" .
xcopy /Y/D/S/E ..\exe\x64\$DebugROOT\*.inf .

#xcopy /Y/D/S/E . \\$RemotIP\$RROOT\HPDriver\z-HPNonPnpDriver\
pause