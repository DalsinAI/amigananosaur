/* std::filesystem's space() (libstdc++'s fs_ops.o) calls statvfs(), which
 * libnix lacks; the stove's libstdc++ was built against a header that
 * declares it as a C++ function. The game never asks for free space, but
 * fs_ops.o comes in with exists() and is_directory(), so the link needs
 * the name: it fails as statvfs does where a file system cannot say.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <errno.h>
#include <sys/statvfs.h>

int statvfs(const char *, struct statvfs *)
{
	errno = ENOSYS;
	return -1;
}
