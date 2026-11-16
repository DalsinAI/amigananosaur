// Host tests of the port's change to Pomme's FindFolder: on AmigaOS the
// preferences folder is the drawer's User directory (the game has made its
// drawer the current directory), so the game's prefs and high scores land
// in User/Nanosaur. Also reads a big-endian AIFF header field through
// Pomme's byte-order helpers, as the game's data are read.
// Built by run.sh with -D__amigaos__ (the Amiga branch) on the host.
// MIT, Copyright (c) 2026 AmiCradle Team.
#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#include "Pomme.h"
#include "PommeFiles.h"
#include "PommeInit.h"
#include "Utilities/bigendianstreams.h"
#include "Utilities/memstream.h"

static int s_fails = 0;

static void check(bool ok, const std::string& what)
{
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", what.c_str());
    if (!ok) {
        ++s_fails;
    }
}

static bool is_dir(const char* p)
{
    struct stat st;
    return stat(p, &st) == 0 && S_ISDIR(st.st_mode);
}

int main(int argc, char** argv)
{
    if (argc < 2 || chdir(argv[1]) != 0) {
        std::printf("FAIL usage: test_prefs SCRATCH_DIR\n");
        return 1;
    }

    // What amiga_make_user_dir() does on the Amiga, before Pomme starts.
    mkdir("User", 0755);

    Pomme::Init();

    short vref = -1;
    long dir_id = -1;
    OSErr err = FindFolder(kOnSystemDisk, kPreferencesFolderType, kDontCreateFolder, &vref, &dir_id);
    check(err == noErr, "FindFolder(prefs) noErr (" + std::to_string(err) + ")");

    long created = -1;
    err = DirCreate(vref, dir_id, "Nanosaur", &created);
    check(err == noErr || err == dupFNErr, "DirCreate Nanosaur in prefs (" + std::to_string(err) + ")");
    check(is_dir("User/Nanosaur"), "User/Nanosaur made in the drawer");

    FSSpec spec;
    err = FSMakeFSSpec(vref, dir_id, ":Nanosaur:Prefs", &spec);  // as the game does
    check(err == noErr || err == fnfErr, "FSMakeFSSpec prefs file (" + std::to_string(err) + ")");

    short ref = 0;
    FSpDelete(&spec);
    err = FSpCreate(&spec, 'NAno', 'Pref', -1);
    check(err == noErr, "FSpCreate prefs file (" + std::to_string(err) + ")");
    err = FSpOpenDF(&spec, fsWrPerm, &ref);
    check(err == noErr, "FSpOpenDF prefs file (" + std::to_string(err) + ")");
    long count = 4;
    err = FSWrite(ref, &count, "PREF");
    check(err == noErr && count == 4, "FSWrite 4 bytes");
    FSClose(ref);
    struct stat st;
    check(stat("User/Nanosaur/Prefs", &st) == 0 && st.st_size == 4, "User/Nanosaur/Prefs is 4 bytes");

    // Big-endian data: 0x0001AC44 is 44100 Hz-ish integer part, as an AIFF holds it.
    const char be[4] = {0x00, 0x01, (char)0xAC, 0x44};
    memstream ms((Ptr)be, 4);
    Pomme::BigEndianIStream bis(ms);
    check(bis.Read<int32_t>() == 0x0001AC44, "big-endian int32 read");

    Pomme::Shutdown();

    std::printf("%s (%d failed)\n", s_fails ? "FAIL" : "PASS", s_fails);
    return s_fails ? 1 : 0;
}
