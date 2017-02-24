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
rmdir /S /Q Max2015-Release-x64
rmdir /S /Q Max2016-Release-x64
