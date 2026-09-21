// dirent.h - MSVC shim. The real dirent.h does not exist under MSVC;
// this header is found first on the include path (cmake/compat) and maps
// the small subset used by OpenFOAM tools/tests onto the UCRT
// _findfirst/_findnext API.
//
// Supports: DIR, struct dirent { d_name }, opendir, readdir, closedir.
// NOT supported: d_type, d_ino, telldir/seekdir/rewinddir.

#ifndef FOAM_COMPAT_DIRENT_H
#define FOAM_COMPAT_DIRENT_H

#ifdef _MSC_VER

#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

struct dirent
{
    char d_name[260];  // _MAX_FNAME
};

typedef struct DIR
{
    intptr_t handle;
    struct _finddata_t findData;
    struct dirent entry;
    int first;         // findData not yet returned
} DIR;

static __inline DIR* opendir(const char* name)
{
    DIR* dir = (DIR*)malloc(sizeof(DIR));
    char pattern[4096];

    if (!dir) { errno = ENOMEM; return NULL; }

    _snprintf(pattern, sizeof(pattern), "%s\\*", name);
    pattern[sizeof(pattern)-1] = '\0';

    dir->handle = _findfirst(pattern, &dir->findData);
    if (dir->handle == -1)
    {
        free(dir);
        errno = ENOENT;
        return NULL;
    }

    dir->first = 1;
    return dir;
}

static __inline struct dirent* readdir(DIR* dir)
{
    if (!dir || dir->handle == -1) return NULL;

    if (dir->first)
    {
        dir->first = 0;
    }
    else if (_findnext(dir->handle, &dir->findData) != 0)
    {
        return NULL;
    }

    strncpy(dir->entry.d_name, dir->findData.name, sizeof(dir->entry.d_name));
    dir->entry.d_name[sizeof(dir->entry.d_name)-1] = '\0';
    return &dir->entry;
}

static __inline int closedir(DIR* dir)
{
    if (!dir) return -1;
    if (dir->handle != -1) _findclose(dir->handle);
    free(dir);
    return 0;
}

#else
#error "This dirent.h shim is only for MSVC"
#endif

#endif // FOAM_COMPAT_DIRENT_H
