rem IDE files----------------------------------------------
del *.user
del *.ncb
del *.log
del /AH *.suo
del *.pdb
del *.lib
del *.ilk
del *.sdf
del *.db

rem Output directories-------------------------------------
rmdir /S /Q ipch

rem x64--------
rmdir /S /Q Debug-Win32
rmdir /S /Q Release-Win32
rmdir /S /Q Debug-x64
rmdir /S /Q Release-x64
