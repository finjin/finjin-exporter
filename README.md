# finjin-exporter
The Finjin exporter plug-ins for 3DS Max and Maya generate 3D assets for use with the Finjin engine.

## Highlights
* Support for 3DS Max and Maya.
* Full scene export.
* More...

## Project Dependencies
* [finjin-common](https://github.com/finjin/finjin-common)
* [finjin-engine](https://github.com/finjin/finjin-engine)

## Documentation
* [finjin-documentation](https://github.com/finjin/finjin-documentation)

## Repository Structure
```
LICENSE.txt                 License file.
README.md                   This file.
configure.cfg               Source code configuration file. Modify this by hand when you start development for the very first time.
configure.py                Configuration script. Run this once when you start development for the very first time.
3dsmax/                     3DS Max-specific source tree.
  exporter/                 Exporter source tree.
  installer/                Installer source tree.
maya/                       Maya-specific source tree.
  exporter/                 Exporter source tree.
  installer/                Installer source tree.
common/                     Shared code.
  assets/images/            Images used by the various projects.
  project/wxformbuilder/    wxFormBuilder project used for creating various user interface elements.
  src/                      Shared source files.
tools/                      Various utility scripts.
wxcpp/library/src/          Include and implementation files for use with wxWidgets.
```
