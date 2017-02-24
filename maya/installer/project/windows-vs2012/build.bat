set SLN_FILE="finjin-exporter-installer.sln"

set OLD_PATH=%PATH%
call "%VS110COMNTOOLS%vsvars32.bat"
@echo %SLN_FILE%

rem x64--------
@devenv %SLN_FILE% /build "Release|x64"

set PATH=%OLD_PATH%