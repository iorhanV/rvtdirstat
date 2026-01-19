# RvtDirStat - Revit Backup Cleanup Tool

## Description

RvtDirStat is a specialised fork of WinDirStat designed to help BIM managers and Revit users manage storage space by identifying large Revit projects and their associated backup files. 

Like WinDirStat, it displays directories and files in a [treemap](https://en.wikipedia.org/wiki/Treemap), where larger areas represent larger files. In addition to the standard directory tree and extension views, RvtDirStat features a dedicated Revit view that groups Revit files and helps distinguish between primary project files and secondary backups.

### Major features

* **Specialised Revit View:** Dedicated list for .rvt files with intelligent grouping.
* **Backup Detection:** Automatically identifies and highlights Revit backup files.
* **Three Classic Views:** Directory Tree, Treemap, and Extension views.
* **Cleanup Actions:** Built-in and user-defined actions (Open, Delete, etc.).

For changes in recent versions, please check out [the change log](CHANGELOG.md).

### Installation

* 📦 Download the latest version from the [release page](https://github.com/rvtdirstat/rvtdirstat/releases/) (Note: ensure you are using the RvtDirStat fork releases).
* 📦 Build from source using the instructions below.

## Copyright / Licenses

* Copyright © WinDirStat Team ([windirstat.net](https://windirstat.net/))
* Revit-specific enhancements by RvtDirStat contributors.

The application itself is distributed under the terms of the [GPL v2](rvtdirstat/res/license.txt), but parts of the source code are also available under more lenient license terms.

The logo and all derivatives are available under the terms of the Creative
Commons license [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).

## Building

RvtDirStat can be built with Visual Studio 2022 or later. A Visual Studio solution file can be loaded from `rvtdirstat.sln`.

## Contributing

You can contribute by responding to issues, developing source code, or developing translations.

## Compatibility

RvtDirStat has been developed for and tested on the following operating systems:

* Windows 10
* Windows 11
* Windows Server 2016 / 2019 / 2022 / 2025

## Logo

![RvtDirStat logo](rvtdirstat/logos/logo_256px.png)
