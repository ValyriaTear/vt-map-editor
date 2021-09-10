 _   _       _            _         _____
| | | |     | |          (_)       |_   _|
| | | | __ _| |_   _ _ __ _  __ _    | | ___  __ _ _ __
| | | |/ _` | | | | | '__| |/ _` |   | |/ _ \/ _` | '__|
\ \_/ / (_| | | |_| | |  | | (_| |   | |  __/ (_| | |
 \___/ \__,_|_|\__, |_|  |_|\__,_|   \_/\___|\__,_|_|
                __/ |
               |___/


**Development blog:** http://valyriatear.blogspot.com
**Bug Tracker:** https://github.com/ValyriaTear/ValyriaTear/issues
**Forums:** http://forum.freegamedev.net/viewforum.php?f=76

Valyria Tear is an open-source single-player medieval-fantasy 2D J-RPG based on an extended (http://www.allacrost.org/) engine and inspired by classic console RPGs.

The player incarnates Bronann, a young boy forced to take part into the struggle for the possession of a mysterious crystal.
This powerful artifact will lead him to discover the actual threat set upon his world, but also courage and love.

The game has all the features you can expect from JRPG classics: entertaining story development, colourful map exploration, active side-view battles, character management, puzzles... It is also translated in several languages.

## Licensing

**Copyright (C) 2012-2021 by Bertram
Copyright (C) 2004-2011 by The Allacrost Project**

The source code is licensed under the *GNU GPL*. It is free software and you may modify it and/or redistribute it under the terms of this license.
See http://www.gnu.org/copyleft/gpl.html for details.

## Compilation procedure

**Unpacking**

- Unpack the vt-map-editor archive at some nice location.
- Unpack the luabind archive in `src/` folder (to obtain `src/luabind`)
- Unpack the vt-utils archive in the `src/` folder (to obtain the following: `src/utils`)

**Linux compilation**

Once the packages are unpacked as described hereabove,
- Install the dependencies (See dependencies if in doubt).
- run `cmake . && make` from the top-level directory at the prompt.

You'll then be able to run the editor by typing: `src/vt-editor`

**Windows compilation**

The windows compilation method used is mostly inspired from the one used to compile the main game,
only with with mingw qt5 packages installed first.

- ** MSys/MinGW **

1. Download and install MSys2
   a. Download MSYS2 from https://msys2.github.io, either the 64 bit or the
      32 bit version, depending on which type of build you want to create
   b. Depending on your version, install to C:\msys64 or C:\msys32
   c. Run C:\msys64\mingw64.exe or C:\msys32\mingw32.exe
   d. Follow the update steps:
      https://github.com/msys2/msys2/wiki/MSYS2-installation#iii-updating-packages.
      Running 'pacman -Syuu' repeatedly and following the instructions on screen
      should do it.
2. Install the toolchain and the dependencies
   a. Install the mingw64 toolchain (or mingw32 toolchain)
      * 64bit: pacman -S mingw-w64-x86_64-toolchain
      * 32bit: pacman -S mingw-w64-i686-toolchain
   b. Install CMake and all dependencies
      * 64 bit:
        pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-qt5 mingw-w64-x86_64-glew mingw-w64-x86_64-libpng mingw-w64-x86_64-libzip mingw-w64-x86_64-lua
      * 32 bit
        pacman -S mingw-w64-i686-cmake mingw-w64-i686-ninja mingw-w64-i686-qt5 mingw-w64-i686-glew mingw-w64-i686-libpng mingw-w64-i686-libzip mingw-w64-i686-lua
      * If any of the dependencies should be missing from the lists above,
        you can search for them on http://repo.msys2.org/mingw/
3. Configure the build
   * 64bit:
     cmake -G "Ninja" -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe .
   * 32 bit:
    cmake -G "Ninja" -DCMAKE_C_COMPILER=C:/msys32/mingw32/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys32/mingw32/bin/g++.exe .
4. Run the build: ninja

## Dependencies

You will need the following in order to compile and run the editor:

- **lua** (5.1.x - 5.2.x - 5.3.x) - (liblua)
- **Qt 5.8.x+**. E.g.: On Debian 9: qt5-default liblua5.3-dev libgl1-mesa-dev libqt5opengl5-dev
