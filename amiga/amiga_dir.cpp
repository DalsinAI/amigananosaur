// The AmigaOS side of Nanosaur for GFOS: see amiga_dir.hpp.
// MIT, Copyright (c) 2026 AmiCradle Team.
#include "amiga_dir.hpp"

#include <cstdlib>

#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>

static BPTR s_progdir_lock = 0;
static BPTR s_old_dir = 0;
static bool s_entered = false;

static void amiga_leave_progdir()
{
    if (!s_entered) {
        return;
    }

    CurrentDir(s_old_dir);
    UnLock(s_progdir_lock);

    s_entered = false;
}

void amiga_enter_progdir()
{
    if (s_entered) {
        return;
    }

    s_progdir_lock = Lock((CONST_STRPTR) "PROGDIR:", SHARED_LOCK);

    if (!s_progdir_lock) {
        return;
    }

    s_old_dir = CurrentDir(s_progdir_lock);
    s_entered = true;

    atexit(amiga_leave_progdir);
}

void amiga_make_user_dir()
{
    BPTR lock = Lock((CONST_STRPTR) "User", SHARED_LOCK);

    if (!lock) {
        lock = CreateDir((CONST_STRPTR) "User");
    }

    if (lock) {
        UnLock(lock);
    }
}
