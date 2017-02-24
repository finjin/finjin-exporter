@rem This file copies all the previously built release files (generated with build.bat) into the "release" directory


rmdir /Q /S release
mkdir release


rem 3DS Max----------------------------
:3DSMAX
if not exist 3dsmax goto MAYA

rem Exporter------
cd 3dsmax\exporter\project\windows-vs2012
call copy.bat
cd ..\..\..\..

rem Installer--------
cd 3dsmax\installer\project\windows-vs2012
call copy.bat
cd ..\..\..\..


rem Maya----------------------------
:MAYA
if not exist maya goto END

rem Exporter------
cd maya\exporter\project\windows-vs2012
call copy.bat
cd ..\..\..\..

rem Installer--------
cd maya\installer\project\windows-vs2012
call copy.bat
cd ..\..\..\..


rem END--------------------------------
:END
