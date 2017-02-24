set PLUGIN_FILE="finjin-exporter.gup"

mkdir ..\..\..\..\release\
mkdir ..\..\..\..\release\finjin-exporter\3dsmax
mkdir ..\..\..\..\release\finjin-exporter\3dsmax\exporters

rem x64--------
mkdir ..\..\..\..\release\finjin-exporter\3dsmax\exporters\max2015-x64
copy Max2015-Release-x64\%PLUGIN_FILE% ..\..\..\..\release\finjin-exporter\3dsmax\exporters\max2015-x64\%PLUGIN_FILE%

mkdir ..\..\..\..\release\finjin-exporter\3dsmax\exporters\max2016-x64
copy Max2016-Release-x64\%PLUGIN_FILE% ..\..\..\..\release\finjin-exporter\3dsmax\exporters\max2016-x64\%PLUGIN_FILE%
