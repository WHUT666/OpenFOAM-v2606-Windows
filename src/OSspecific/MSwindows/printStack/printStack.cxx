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

    OpenFOAM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "Ostream.H"

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

#include <mutex>
#include <sstream>

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace
{

// DbgHelp (Sym*) functions are documented as single-threaded per process.
// In a crash context the lock may already be held by a dead thread, so only
// ever try_lock() once and fall back to module+offset rendering, which uses
// kernel32 calls alone.
std::mutex dbghelpMutex;

// One-time DbgHelp initialisation.
// SYMOPT_DEFERRED_LOADS keeps it cheap: module symbol tables are only
// loaded when a frame in that module is actually resolved.
// Invade=TRUE registers all currently loaded modules - call lazily at
// first use rather than from a static initializer so that dlOpen'd
// plugins are also covered.
bool initSymbols(HANDLE proc)
{
    static const bool initialised = [proc]()
    {
        ::SymSetOptions
        (
            SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES
          | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_NO_PROMPTS
        );
        return (::SymInitialize(proc, nullptr, TRUE) != 0);
    }();
    return initialised;
}


// Module basename + offset for an address - kernel32 only, no DbgHelp
bool moduleInfo(const void* addr, std::string& name, DWORD64& offset)
{
    HMODULE mod = nullptr;
    if
    (
        !::GetModuleHandleExA
        (
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
          | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            static_cast<LPCSTR>(addr), &mod
        )
    )
    {
        return false;
    }

    char buf[MAX_PATH];
    if (!::GetModuleFileNameA(mod, buf, MAX_PATH))
    {
        return false;
    }

    const char* base = buf;
    for (const char* p = buf; *p; ++p)
    {
        if (*p == '\\' || *p == '/') base = p + 1;
    }

    name = base;
    offset = reinterpret_cast<DWORD64>(addr) - reinterpret_cast<DWORD64>(mod);
    return true;
}


// Render raw stack addresses to a stream
void render_stack(std::ostream& os, void* const frames[], const int numFrames)
{
    const HANDLE proc = ::GetCurrentProcess();

    std::unique_lock<std::mutex> lock(dbghelpMutex, std::try_to_lock);
    const bool haveSym = lock.owns_lock() && initSymbols(proc);

    for (int i = 0; i < numFrames; ++i)
    {
        const DWORD64 addr = reinterpret_cast<DWORD64>(frames[i]);

        os << '#' << i << "  ";

        bool resolved = false;
        std::string modName, lineInfo;

        if (haveSym)
        {
            char storage[sizeof(SYMBOL_INFO) + MAX_SYM_NAME];
            auto* si = reinterpret_cast<SYMBOL_INFO*>(storage);
            si->SizeOfStruct = sizeof(SYMBOL_INFO);
            si->MaxNameLen = MAX_SYM_NAME;

            DWORD64 disp = 0;
            if (::SymFromAddr(proc, addr, &disp, si))
            {
                os << si->Name;
                if (disp)
                {
                    os << "+0x" << std::hex << disp << std::dec;
                }
                resolved = true;
            }

            IMAGEHLP_MODULE64 modInfo{};
            modInfo.SizeOfStruct = sizeof(modInfo);
            if (::SymGetModuleInfo64(proc, addr, &modInfo))
            {
                modName = modInfo.ModuleName;
            }

            IMAGEHLP_LINE64 line{};
            line.SizeOfStruct = sizeof(line);
            DWORD ldisp = 0;
            if (::SymGetLineFromAddr64(proc, addr, &ldisp, &line))
            {
                lineInfo = line.FileName;
                lineInfo += ':';
                lineInfo += std::to_string(line.LineNumber);
            }
        }

        if (resolved)
        {
            if (!modName.empty())
            {
                os << "  [" << modName << ']';
            }
        }
        else
        {
            // Fallback: module+offset (or bare address)
            std::string mod;
            DWORD64 off = 0;
            if (moduleInfo(frames[i], mod, off))
            {
                os << mod << "+0x" << std::hex << off << std::dec;
            }
            else
            {
                os << "0x" << std::hex << addr << std::dec;
            }
        }

        // file:line may resolve even when the symbol name does not
        if (!lineInfo.empty())
        {
            os << "  " << lineInfo;
        }

        os << '\n';
    }
}


// Capture caller frames. Frame 0 is this function's return address -
// FramesToSkip=2 additionally skips printStack/safePrintStack, so the
// first reported frame is the caller of printStack (eg the signal
// handler, or abort()).
int capture_stack(void* frames[], const int size)
{
    const int maxN = (size > 0 && size < 128) ? size : 128;
    return static_cast<int>(::CaptureStackBackTrace(2, maxN, frames, nullptr));
}

} // End anonymous namespace


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Note: demangle requires symbols only - without extra '(' etc.
Foam::word Foam::error::demangle(const char* symbol)
{
    if (!symbol || !*symbol)
    {
        return word();
    }

    char buf[4096];
    const DWORD len = ::UnDecorateSymbolName
    (
        symbol, buf, DWORD(sizeof(buf)), UNDNAME_COMPLETE
    );

    if (len)
    {
        // No strip. We wish to print like 'char*' without surrounding quotes
        return word(buf, false);
    }

    // Undecoration failed (not an MSVC-mangled name): pass through
    return word(symbol, false);
}


void Foam::error::safePrintStack(std::ostream& os, int size)
{
    void* frames[128];
    const int numFrames = capture_stack(frames, size);

    os << "[stack trace]\n"
          "=============" << std::endl;

    if (numFrames > 0)
    {
        render_stack(os, frames, numFrames);
    }

    os << "=============" << std::endl;
}


void Foam::error::printStack(Foam::Ostream& os, int size)
{
    void* frames[128];
    const int numFrames = capture_stack(frames, size);

    std::ostringstream buf;
    buf << "[stack trace]\n"
           "=============\n";

    if (numFrames > 0)
    {
        render_stack(buf, frames, numFrames);
    }

    buf << "=============" << std::endl;

    os << buf.str().c_str();
}


// ************************************************************************* //
