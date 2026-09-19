// unistd.h - MSVC shim. The real unistd.h does not exist under MSVC;
// this header is found first on the include path (cmake/compat) and maps
// to the equivalent UCRT facilities.

#ifndef FOAM_COMPAT_UNISTD_H
#define FOAM_COMPAT_UNISTD_H

#ifdef _MSC_VER

#include "msvcCompat.h"
#include <io.h>
#include <process.h>
#include <direct.h>
#include <stdlib.h>

#else
#error "This unistd.h shim is only for MSVC"
#endif

#endif // FOAM_COMPAT_UNISTD_H
