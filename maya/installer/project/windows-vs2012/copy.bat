set APPLICATION_FILE="finjin-exporter-installer.exe"

mkdir ..\..\..\..\release\
mkdir ..\..\..\..\release\finjin-exporter\maya

rem x64--------
mkdir ..\..\..\..\release\finjin-exporter\maya\exporters
copy Release-x64\%APPLICATION_FILE% ..\..\..\..\release\finjin-exporter\maya\%APPLICATION_FILE%
