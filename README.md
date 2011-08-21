# ShhLion

Mac OS X 10.7 (Lion) kernel extension to suppress a high pitch sound in CPU idle mode.

ONLY tested on MacBook (13-inch Mid 2007).

Implementation based on:
https://discussions.apple.com/thread/461317?start=210&tstart=0

## Download

[From GitHub](https://github.com/downloads/larsxschneider/ShhLion/ShhLion.kext.zip)

## Installation

Attention: You are about to install a kernel extension. This might crash your system. Be sure you know what your're doing :-)

1. Extract ShhLion.kext.zip
2. sudo cp -R ShhLion.kext /System/Library/Extensions/
3. sudo kextload /System/Library/Extensions/ShhLion.kext

## Author

Lars Schneider <larsxschneider+github@gmail.com>





