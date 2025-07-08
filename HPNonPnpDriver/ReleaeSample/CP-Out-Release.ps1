param (
    [string]$RemotIP,
	[int]$IsOMENRoot = 0,
	[int]$IsDebugRoot = 0
)

if ($IsDebugRoot -eq 1){
	$DebugROOT='Release'
}
else{
	$DebugROOT='Debug'
}

if ($IsOMENRoot -eq 1){
	$RROOT='1-OMENDev'
}
else{
	$RROOT='1-JoshDev'
}

$PARAM='/Y/D/S'


xcopy $PARAM ..\exe\x64\$DebugROOT\*.exe .
xcopy $PARAM ..\sys\x64\$DebugROOT\HPNonPnpDriver\*.sys .
xcopy $PARAM ..\exe\x64\$DebugROOT\*.pdb .
xcopy $PARAM ..\sys\x64\$DebugROOT\*.pdb .
xcopy $PARAM "C:\Program Files (x86)\Windows Kits\8.0\redist\wdf\x64\WdfCoInstaller01011.dll" .
xcopy $PARAM ..\exe\x64\$DebugROOT\*.inf .

xcopy $PARAM . \\$RemotIP\$RROOT\HPDriver\z-HPNonPnpDriver\
pause