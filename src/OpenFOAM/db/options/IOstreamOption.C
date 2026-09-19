/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2025 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "IOstreamOption.H"
#include "debug.H"
#include <dictionary.H>
#include "Enum.H"
#include "Switch.H"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

const Foam::IOstreamOption::versionNumber Foam::IOstreamOption::currentVersion;

const Foam::Enum
<
    Foam::IOstreamOption::floatFormat
>
Foam::IOstreamOption::floatFormatNames
({
    { floatFormat::general, "general" },
    { floatFormat::fixed, "fixed" },
    { floatFormat::scientific, "scientific" },
});

const Foam::Enum
<
    Foam::IOstreamOption::streamFormat
>
Foam::IOstreamOption::formatNames
({
    { streamFormat::ASCII, "ascii" },
    { streamFormat::BINARY, "binary" },
    // No selection by name: UNKNOWN_FORMAT
});


// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

Foam::IOstreamOption::floatFormat
Foam::IOstreamOption::floatFormatEnum
(
    const word& fmtName,
    const floatFormat deflt
)
{
    if (fmtName.empty())
    {
        // Empty string (no-op)
    }
    else if (auto iter = floatFormatNames.cfind(fmtName); iter.good())
    {
        return iter.val();
    }
    else
    {
        // Emit warning for bad input

        auto& err = WarningInFunction
            << "Unknown float format '" << fmtName << "' using ";

        if (auto iter = floatFormatNames.cfind(deflt); iter.good())
        {
            err << '\'' << iter.key() << '\'';
        }
        else
        {
            err << "value=" << int(deflt);
        }
        err << " from " << floatFormatNames << nl;
    }

    return deflt;
}


Foam::IOstreamOption::floatFormat
Foam::IOstreamOption::floatFormatEnum
(
    const word& key,
    const dictionary& dict,
    const floatFormat deflt
)
{
    return floatFormatNames.getOrDefault(key, dict, deflt, true);  // warnOnly
}


Foam::IOstreamOption::streamFormat
Foam::IOstreamOption::formatEnum
(
    const word& fmtName,
    const streamFormat deflt
)
{
    if (fmtName.empty())
    {
        // Empty string (no-op)
    }
    else if (auto iter = formatNames.cfind(fmtName); iter.good())
    {
        return iter.val();
    }
    else
    {
        // Emit warning for bad input

        auto& err = WarningInFunction
            << "Unknown stream format '" << fmtName << "' using ";

        if (auto iter = formatNames.cfind(deflt); iter.good())
        {
            err << '\'' << iter.key() << '\'';
        }
        else
        {
            err << "value=" << int(deflt);
        }
        err << " from " << formatNames << nl;
    }

    return deflt;
}


Foam::IOstreamOption::streamFormat
Foam::IOstreamOption::formatEnum
(
    const word& key,
    const dictionary& dict,
    const streamFormat deflt
)
{
    return formatNames.getOrDefault(key, dict, deflt, true);  // warnOnly
}


Foam::IOstreamOption::compressionType
Foam::IOstreamOption::compressionEnum
(
    const word& compName,
    const compressionType deflt
)
{
    if (compName.empty())
    {
        // Empty string (no-op)
    }
    else if (Switch sw = Switch::find(compName); sw.good())
    {
        return
        (
            sw
          ? compressionType::COMPRESSED
          : compressionType::UNCOMPRESSED
        );
    }
    else
    {
        // Emit warning

        WarningInFunction
            << "Unknown compression specifier '" << compName
            << "' using compression " << (deflt ? "on" : "off") << nl;
    }

    return deflt;
}


Foam::IOstreamOption::compressionType
Foam::IOstreamOption::compressionEnum
(
    const word& key,
    const dictionary& dict,
    const compressionType deflt
)
{
    return
    (
        Switch(key, dict, Switch(bool(deflt)), true)  // warnOnly
      ? compressionType::COMPRESSED
      : compressionType::UNCOMPRESSED
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::IOstreamOption::versionNumber::versionNumber(const std::string& verNum)
:
    versionNumber(readFloat(verNum))
{}


Foam::IOstreamOption::versionNumber::versionNumber(const token& tok)
:
    versionNumber()
{
    if (tok.isStringType())
    {
        (*this) = versionNumber(tok.stringToken());
    }
    else if (tok.isNumber())
    {
        // Accept integer or floating-point
        // Eg, '2.0' becomes '2' after foamDictionary -expand
        (*this) = versionNumber(float(tok.number()));
    }
    else
    {
        WarningInFunction
            << "Wrong token for version - expected word/number, found "
            << tok.info() << nl;
    }
}


Foam::IOstreamOption::versionNumber::versionNumber
(
    const word& key,
    const dictionary& dict
)
:
    versionNumber()
{
    token tok;

    if (dict.readIfPresent<token>(key, tok, keyType::LITERAL))
    {
        (*this) = versionNumber(tok);
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const IOstreamOption::streamFormat& fmt
)
{
    // Silently ignores unnamed formats
    os << IOstreamOption::formatNames[fmt];
    return os;
}


Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const IOstreamOption::versionNumber& ver
)
{
    // Emit unquoted char sequence (eg, word)
    // for correct behaviour when sending in parallel

    os.writeQuoted(ver.str(), false);
    return os;
}


// ************************************************************************* //
