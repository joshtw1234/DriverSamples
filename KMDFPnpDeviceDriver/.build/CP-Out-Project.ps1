param (
    [string]$RemotIP,
	[int]$IsOMENRoot = 0
)

$CPPARAM='/Y /D /S /E'
$ROOTPATH='..\'
$REMOTPATH='SDV-JOSHDRIVER-Final\'
Echo $RemotIP

Echo \\$RemotIP\$REMOTPATH
#\\15.38.111.199\1-JoshDev\1-DEBUGOMEN\1-OMENGUI
if ($IsOMENRoot -eq 1){
	$RROOT='1-OMENDev'
}
else{
	$RROOT='1-JoshDev'
}

xcopy '/Y/D/S/E' $ROOTPATH\*.* \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.dll \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.exe \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.config \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.json \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.pak \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.png \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.jpg \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.wav \\$RemotIP\$RROOT\$REMOTPATH
#xcopy '/Y/D/S/E' $ROOTPATH\*.otf \\$RemotIP\$RROOT\$REMOTPATH

pause
