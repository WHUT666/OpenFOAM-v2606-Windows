/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2026 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Minidump writer for hard crashes on Windows.

    Installs a vectored exception handler (first-chance) that captures
    EXCEPTION_POINTERS at the faulting instruction and writes a
    "foam-crash-<pid>-<ticks>.dmp" next to the case before the regular
    signal machinery prints its stack trace. The dump can be opened in
    Visual Studio or WinDbg with full registers/locals when PDBs are
    available.

    Disabled by setting the environment variable FOAM_NO_CRASH_DUMP.

\*---------------------------------------------------------------------------*/

#include "MSwindows.H"
#include "error.H"

// Windows name clash with OpenFOAM messageStream
#undef DebugInfo

#include <windows.h>

// msvcCompat.h undefines the SAL annotation macros (IN/OUT/INOUT/OPTIONAL)
// because they clash with OpenFOAM identifiers - dbghelp.h needs them.
// Restore them just for the include and drop them again afterwards.
#define IN
#define OUT
#define INOUT
#define OPTIONAL
#include <dbghelp.h>
#undef IN
#undef OUT
#undef INOUT
#undef OPTIONAL

#include <cstdio>
#include <cstring>

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace Foam
{
namespace MSwindows
{

static void writeMiniDump(EXCEPTION_POINTERS* exc)
{
    char env[8];
    if (::GetEnvironmentVariableA("FOAM_NO_CRASH_DUMP", env, sizeof(env)))
    {
        return;
    }

    char name[64];
    std::snprintf
    (
        name, sizeof(name),
        "foam-crash-%lu-%llu.dmp",
        static_cast<unsigned long>(::GetCurrentProcessId()),
        static_cast<unsigned long long>(::GetTickCount64())
    );

    const HANDLE file =
        ::CreateFileA
        (
            name,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

    if (file == INVALID_HANDLE_VALUE)
    {
        return;
    }

    MINIDUMP_EXCEPTION_INFORMATION info;
    std::memset(&info, 0, sizeof(info));
    info.ThreadId = ::GetCurrentThreadId();
    info.ExceptionPointers = exc;
    info.ClientPointers = FALSE;

    const MINIDUMP_TYPE type = static_cast<MINIDUMP_TYPE>
    (
        MiniDumpWithDataSegs
      | MiniDumpWithIndirectlyReferencedMemory
      | MiniDumpScanMemory
      | MiniDumpWithThreadInfo
    );

    ::MiniDumpWriteDump
    (
        ::GetCurrentProcess(),
        ::GetCurrentProcessId(),
        file,
        type,
        exc ? &info : nullptr,
        nullptr,
        nullptr
    );

    ::CloseHandle(file);

    char cwd[MAX_PATH];
    if (::GetCurrentDirectoryA(sizeof(cwd), cwd))
    {
        std::fprintf(stderr, "Crash dump written: %s\\%s\n", cwd, name);
    }
    else
    {
        std::fprintf(stderr, "Crash dump written: %s\n", name);
    }
    std::fflush(stderr);
}


static LONG CALLBACK crashDumpHandler(EXCEPTION_POINTERS* exc)
{
    switch (exc->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        case EXCEPTION_ILLEGAL_INSTRUCTION:
        case EXCEPTION_STACK_OVERFLOW:
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
        case EXCEPTION_INT_OVERFLOW:
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_FLT_OVERFLOW:
        case EXCEPTION_FLT_UNDERFLOW:
        case EXCEPTION_FLT_INVALID_OPERATION:
        case EXCEPTION_FLT_STACK_CHECK:
        case EXCEPTION_IN_PAGE_ERROR:
            writeMiniDump(exc);
            break;

        default:
            break;
    }

    // Let the regular signal handlers (stack trace, re-raise) continue
    return EXCEPTION_CONTINUE_SEARCH;
}

} // End namespace MSwindows
} // End namespace Foam


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::MSwindows::installCrashDump()
{
    static bool installed = false;
    if (installed)
    {
        return;
    }
    installed = true;

    ::AddVectoredExceptionHandler(1, &crashDumpHandler);
}


// ************************************************************************* //
