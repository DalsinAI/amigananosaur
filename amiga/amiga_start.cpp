// Nanosaur for GFOS: the program's version string and its stack.
// MIT, Copyright (c) 2026 AmiCradle Team.

// libnix gives the program this much stack when it starts.
extern "C" unsigned long __stack = 512 * 1024;

extern "C" const char ns_amiga_version[] __attribute__((used)) =
    "$VER: Nanosaur 1.4.4 (16.11.2026)";
