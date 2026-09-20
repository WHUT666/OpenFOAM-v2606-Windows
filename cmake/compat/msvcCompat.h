// msvcCompat.h - MSVC compatibility shims for OpenFOAM (force-included /FI)
//
// Provides the POSIX-ish bits that wmake/MinGW builds take for granted
// but that MSVC lacks. Keep this header light: it is included by EVERY
// translation unit.

#ifndef FOAM_MSVC_COMPAT_H
#define FOAM_MSVC_COMPAT_H

#ifdef _MSC_VER

#ifdef __cplusplus
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cerrno>
#include <cstdint>
#else
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#endif
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include <process.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <winsock2.h>   // gethostname (pulls windows.h subset)

// ---------------------------------------------------------------------------
// Type shims
// ---------------------------------------------------------------------------
#ifndef _PID_T_
#define _PID_T_
typedef int pid_t;
#endif
#ifndef _SSIZE_T_
#define _SSIZE_T_
typedef long long ssize_t;
#endif
typedef int uid_t;
typedef int gid_t;
typedef long useconds_t;
typedef long suseconds_t;
// UCRT sys/types.h does not provide mode_t
typedef int mode_t;

// sys/stat.h bits that MSVC names differently or lacks
#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#endif
#ifndef S_IWUSR
#define S_IWUSR _S_IWRITE
#endif
#ifndef S_IXUSR
#define S_IXUSR _S_IEXEC
#endif
#ifndef S_IRWXU
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#endif
#ifndef S_IRGRP
#define S_IRGRP S_IRUSR
#endif
#ifndef S_IWGRP
#define S_IWGRP S_IWUSR
#endif
#ifndef S_IXGRP
#define S_IXGRP S_IXUSR
#endif
#ifndef S_IROTH
#define S_IROTH S_IRUSR
#endif
#ifndef S_IWOTH
#define S_IWOTH S_IWUSR
#endif
#ifndef S_IXOTH
#define S_IXOTH S_IXUSR
#endif
#ifndef S_IRWXG
#define S_IRWXG S_IRWXU
#endif
#ifndef S_IRWXO
#define S_IRWXO S_IRWXU
#endif
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFMT) == _S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(m) (((m) & _S_IFMT) == _S_IFREG)
#endif
#ifndef S_ISLNK
#define S_ISLNK(m) (0)
#endif
#ifndef S_ISCHR
#define S_ISCHR(m) (((m) & _S_IFMT) == _S_IFCHR)
#endif
#ifndef S_ISFIFO
#define S_ISFIFO(m) (((m) & _S_IFMT) == _S_IFIFO)
#endif
#ifndef S_ISSOCK
#define S_ISSOCK(m) (0)
#endif
#ifndef S_ISBLK
#define S_ISBLK(m) (0)
#endif

// access() mode bits
#ifndef F_OK
#define F_OK 0
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef R_OK
#define R_OK 4
#endif
#ifndef X_OK
#define X_OK 4   // no real exec bit on Windows; treat like read
#endif

// ---------------------------------------------------------------------------
// Function name shims (POSIX -> MSVC)
//
// NOTE: the UCRT already declares the common POSIX names (read, write,
// close, access, dup, mkdir, stat, strdup, ...) as nonstandard aliases.
// DO NOT map them with #define - macros rewrite C++ member calls too
// (os_.write -> os_._write), which binds to renamed extern-template
// declarations in the MSVC STL and fails at link (LNK2001 on
// std::basic_ostream::_write etc). Only shims for names the UCRT lacks.
// ---------------------------------------------------------------------------
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <process.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef FOAM_NO_MSVC_FUNC_MAP
static __inline int strcasecmp(const char* a, const char* b) { return _stricmp(a, b); }
static __inline int strncasecmp(const char* a, const char* b, size_t n) { return _strnicmp(a, b, n); }
static __inline char* strtok_r(char* s, const char* d, char** c) { return strtok_s(s, d, c); }
#define alloca     _alloca
#endif

// lstat == stat on Windows (no symlinks followed anyway)
#ifndef lstat
#define lstat(p,b) ::stat(p,b)
#endif

// ---------------------------------------------------------------------------
// Missing functions as small inlines (C++ only)
// ---------------------------------------------------------------------------
#ifdef __cplusplus
namespace Foam { namespace msvcCompat {

inline int setenv_(const char* name, const char* value, int /*overwrite*/)
{
    return _putenv_s(name, value ? value : "");
}
inline int unsetenv_(const char* name)
{
    return _putenv_s(name, "");
}
inline struct tm* gmtime_r_(const time_t* t, struct tm* out)
{
    return (::gmtime_s(out, t) == 0) ? out : nullptr;
}
inline struct tm* localtime_r_(const time_t* t, struct tm* out)
{
    return (::localtime_s(out, t) == 0) ? out : nullptr;
}
inline char* strerror_r_(int errnum, char* buf, size_t buflen)
{
    return (::strerror_s(buf, buflen, errnum) == 0) ? buf : nullptr;
}

}} // namespace Foam::msvcCompat
#endif // __cplusplus

#ifndef setenv
#define setenv    Foam::msvcCompat::setenv_
#endif
#ifndef unsetenv
#define unsetenv  Foam::msvcCompat::unsetenv_
#endif
#ifndef gmtime_r
#define gmtime_r  Foam::msvcCompat::gmtime_r_
#endif
#ifndef localtime_r
#define localtime_r Foam::msvcCompat::localtime_r_
#endif
#ifndef strerror_r
#define strerror_r Foam::msvcCompat::strerror_r_
#endif

// NOTE: sleep()/usleep() are NOT macro-mapped here. OSspecific.H declares
// Foam::sleep()/Foam::usleep() and the MSwindows implementation defines them;
// a macro would rewrite those declarations and cause redefinition errors.

// gethostname is in winsock
#pragma comment(lib, "ws2_32.lib")

// ---------------------------------------------------------------------------
// Windows SDK macro clashes with OpenFOAM identifiers.
// Including <windows.h> here (early) and undefining the offenders ensures
// they stay undefined for the rest of the TU (later windows.h includes
// are include-guarded and won't reintroduce them).
// ---------------------------------------------------------------------------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#undef IGNORE
#undef STRICT
#undef ERROR
#undef WARN
#undef DEFAULT
#undef min
#undef max
#undef small          /* rpcndr.h defines 'small' as a macro */
#undef interface      /* COM headers */
#undef near           /* windef.h legacy segmented-memory keywords */
#undef far
#undef NO_DATA        /* winsock2.h: WSANO_DATA clashes with polySurface::NO_DATA enum */
#undef HOST_NOT_FOUND /* winsock2.h - potential identifier clash */
#undef TRY_AGAIN      /* winsock2.h - potential identifier clash */
#undef NO_RECOVERY    /* winsock2.h - potential identifier clash */
#undef DELETE         /* winnt.h access right clashes with setAction::DELETE */
#undef DUPLICATE      /* nb30.h */
#undef DIFFERENCE     /* WinUser.h */
#undef OPTIONAL       /* CertBCli.h */
#undef IN             /* rpc headers - setOperation::IN etc. */
#undef OUT            /* rpc headers */
#undef INOUT          /* rpc headers */

// ---------------------------------------------------------------------------
// GNU C extensions
// ---------------------------------------------------------------------------
#ifndef __attribute__
#define __attribute__(x)
#endif

#ifndef __builtin_expect
#define __builtin_expect(cond, val) (cond)
#endif

// restrict keyword
#ifndef __restrict
#define __restrict __restrict
#endif
#ifndef __restrict__
#define __restrict__ __restrict
#endif

// M_PI and friends
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#endif // _MSC_VER

// Per-library dllexport/dllimport macros (<Target>_API) for shared builds.
// Self-guarded: all macros expand to nothing unless FOAM_SHARED_LIBS.
#include "foamApi.h"

#endif // FOAM_MSVC_COMPAT_H
