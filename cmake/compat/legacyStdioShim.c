/*
 * Shim for objects compiled against the pre-VS2015 (non-UCRT) stdio ABI.
 *
 * Legacy-compiled objects (e.g. conda-forge metis) resolve stdin/stdout/
 * stderr through __imp___iob_func / __iob_func, which returned a pointer to
 * the CRT's internal FILE array. The UCRT removed it; the Microsoft
 * legacy_stdio_definitions.lib only covers the printf-family, not __iob_func.
 *
 * Emulate the old behaviour: return a snapshot array of the three standard
 * streams. Callers only use entries [0..2] as FILE* for fprintf/fputs etc.
 * The UCRT _iobuf struct layout is readable, so copying the structs is safe
 * for output operations (the copied handles share the real fd/buffer).
 */
#include <stdio.h>

static FILE _legacy_iob[3];

FILE* __cdecl _legacy_iob_init(void)
{
    _legacy_iob[0] = *stdin;
    _legacy_iob[1] = *stdout;
    _legacy_iob[2] = *stderr;
    return _legacy_iob;
}

/* __imp_ slot: legacy objects call the function through this pointer */
FILE* (*__imp___iob_func)(void) = _legacy_iob_init;

/* Direct symbol too (non-imported references) */
FILE* __iob_func(void)
{
    return _legacy_iob_init();
}
