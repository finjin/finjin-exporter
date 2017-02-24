@rem This file builds all versions of the Finjin exporters and installers.


rem 3DS Max---------------------------
:3DSMAX
if not exist 3dsmax goto MAYA

cd 3dsmax\exporter\project\windows-vs2012
call build.bat
cd ..\..\..\..

cd 3dsmax\installer\project\windows-vs2012
call build.bat
cd ..\..\..\..


rem Maya---------------------------
:MAYA
if not exist maya goto END

cd maya\exporter\project\windows-vs2012
call build.bat
cd ..\..\..\..

cd maya\installer\project\windows-vs2012
call build.bat
cd ..\..\..\..


:END
