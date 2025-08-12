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
$PATH_SOURCE='..\x64\'
$PATH_OUT_LOCAL='..\ReleasePkg\'

xcopy $PARAM $PATH_SOURCE\$DebugROOT\KMDFPnpDeviceDriver\*.* $PATH_OUT_LOCAL
xcopy $PARAM $PATH_SOURCE\$DebugROOT\*.pdb $PATH_OUT_LOCAL

#.\Inf2Cat-tool.ps1 $PATH_OUT_LOCAL


xcopy $PARAM $PATH_OUT_LOCAL\*.* \\$RemotIP\$RROOT\HPDriver\z-JoshPnpDriver\

pause