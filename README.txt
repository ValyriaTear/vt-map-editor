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

**Copyright (C) 2012-2015 by Bertram  
Copyright (C) 2004-2011 by The Allacrost Project**

The source code is licensed under the *GNU GPL*. It is free software and you may modify it and/or redistribute it under the terms of this license.  
See http://www.gnu.org/copyleft/gpl.html for details.

## Compilation procedure

**Linux compilation**
Once you've unpacked the tarball, just run `cmake . && make ` from the top-level directory at the prompt.

You'll then be able to run the editor by typing: `src/vt-editor`

## Dependencies

You will need the following in order to compile and run the editor:

- **lua** (5.1.x - 5.2.x - 5.3.x) - (liblua)
- **Qt 4.8.x** (only if you wish to compile the map editor)

Note that the editor can't be built using C::B. Use **Qt-Creator** and the **MapEditor.pro** file for that.
Be sure also to read the instructions within that file, as **RTTI support** must be **enabled**, and **unicode disabled** in the **compile flags**.
