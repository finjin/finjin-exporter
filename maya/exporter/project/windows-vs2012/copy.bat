set PLUGIN_FILE="finjin-exporter.mll"

mkdir ..\..\..\..\release\
mkdir ..\..\..\..\release\finjin-exporter\maya
mkdir ..\..\..\..\release\finjin-exporter\maya\exporters

rem x64--------
mkdir ..\..\..\..\release\finjin-exporter\maya\exporters\maya2015-x64
copy Maya2015-Release-x64\%PLUGIN_FILE% ..\..\..\..\release\finjin-exporter\maya\exporters\maya2015-x64\%PLUGIN_FILE%

mkdir ..\..\..\..\release\finjin-exporter\maya\exporters\maya2016-x64
copy Maya2016-Release-x64\%PLUGIN_FILE% ..\..\..\..\release\finjin-exporter\maya\exporters\maya2016-x64\%PLUGIN_FILE%
