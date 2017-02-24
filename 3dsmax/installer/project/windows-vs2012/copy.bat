set APPLICATION_FILE="finjin-exporter-installer.exe"

mkdir ..\..\..\..\release\
mkdir ..\..\..\..\release\finjin-exporter\3dsmax

rem x64--------
mkdir ..\..\..\..\release\finjin-exporter\3dsmax\exporters
copy Release-x64\%APPLICATION_FILE% ..\..\..\..\release\finjin-exporter\3dsmax\%APPLICATION_FILE%
