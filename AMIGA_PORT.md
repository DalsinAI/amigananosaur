Nanosaur for GFOS
=================

A port of Nanosaur, Pangea Software's 1998 3D dinosaur action game, as
updated by Iliyas Jorio, from https://github.com/jorio/Nanosaur at commit
a149c77ee196434bbd1e8f2d3762a2320e5d8a42 (version 1.4.4 and later fixes,
the last on SDL 2), for GFOS: AmigaOS 3.2 with GFOpenUp, on a 68040 or
68060 with an FPU, on GFOpenUp's SDL 2 and OpenGL.

You are a Nanosaur sent back 65 million years with a jet pack and guns,
to bring back the eggs of five dinosaur kinds before the asteroid
strikes.

Status: see the pull request (stove build and lab).


What you need
-------------

- GFOS: AmigaOS 3.2 with GFOpenUp, which installs LIBS:GFOpenGPU/
  SDL2.module and GL.module.
- A 68040 or 68060 with an FPU (not an LC040 or EC040), an RTG screen of
  640 x 480 or more (or the AmiCradle lab), and about 48 MB of free
  memory.
- About 25 MB on the disk.


How to play
-----------

Run Install-Nanosaur (or copy the Nanosaur drawer where you want it),
then double-click Nanosaur, or start it from a Shell from anywhere: the
game makes its own drawer its current directory while it runs. The keys
are in the drawer's Controls file. Your settings and high scores are kept
in the drawer's User drawer (User/Nanosaur); Install-Nanosaur keeps it
when it updates the game.


Building
--------

    make -f amiga/Makefile.amigaos -j4 STOVE=/path/to/os32-gcc16-open6

or, in Cradle, stove_build with the os32-gcc16-open6 stove and target
package (package/build-amiga.sh), which also packs
Nanosaur-VERSION-68040.lha.

    tests/host/run.sh

runs the host tests of the port's change to Pomme (no SDL needed).


What was changed
----------------

- The game draws with GL 1 (fixed pipeline) on GFOpenGPU's GL.module, in
  the GL 2.0 compatibility context upstream already asks SDL for.
- __BIG_ENDIAN__ is defined, so Pomme and the game read the Mac data
  without byte swapping, as on a PowerPC Mac.
- src/Boot.cpp: the Amiga build makes PROGDIR: its current directory
  first and makes User/ there when missing (amiga/amiga_dir.cpp), then
  finds Data/ in it.
- extern/Pomme/src/Files/Files.cpp: on AmigaOS the preferences folder
  is the drawer's User directory. tests/host/test_prefs.cpp checks it
  with the calls the game makes.
- amiga/fs_space.cpp: the statvfs() that libstdc++'s std::filesystem
  needs to link (libnix has none; the game never asks for free space).
- amiga/amiga_start.cpp: a 512 KB stack and the $VER string.

Designed, tested and delivered by The AmiCradle Team.
