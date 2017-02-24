
rem Output directories-------------------------------------
rmdir /S /Q release


rem 3DS Max---------------------------
:3DSMAX
if not exist 3dsmax goto MAYA

cd 3dsmax\exporter\project\windows-vs2012
call clean.bat
cd ..\..\..\..

cd 3dsmax\installer\project\windows-vs2012
call clean.bat
cd ..\..\..\..


rem Maya---------------------------
:MAYA
if not exist maya goto END

@rem cd maya\exporter\src\mel
@rem call clean.bat
@rem cd ..\..\..\..

cd maya\exporter\project\windows-vs2012
call clean.bat
cd ..\..\..\..

cd maya\installer\project\windows-vs2012
call clean.bat
cd ..\..\..\..


:END
