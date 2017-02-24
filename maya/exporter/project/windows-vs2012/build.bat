set SLN_FILE="finjin-exporter.sln"

set OLD_PATH=%PATH%
call "%VS110COMNTOOLS%vsvars32.bat"
@echo %SLN_FILE%

rem x64--------
@devenv %SLN_FILE% /build "Maya2015-Release|x64"
@devenv %SLN_FILE% /build "Maya2016-Release|x64"

set PATH=%OLD_PATH%