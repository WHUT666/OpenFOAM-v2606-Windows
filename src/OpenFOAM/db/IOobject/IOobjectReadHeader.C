/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019-2026 OpenCFD Ltd.
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

#include "IOobject.H"
#include <dictionary.H>
#include "foamVersion.H"
#include "fileOperation.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::IOstreamOption Foam::IOobject::parseHeader(const dictionary& headerDict)
{
    IOstreamOption streamOpt;  // == (ASCII, currentVersion)

    // Treat "version" as optional
    if (token tok; headerDict.readIfPresent("version", tok))
    {
        streamOpt.version(tok);
    }

    // Treat "format" as mandatory for now
    streamOpt.format(headerDict.get<word>("format"));

    // The "class" entry is mandatory
    headerClassName_ = headerDict.get<word>("class");

    // The "object" entry is mandatory, but not actually used here
    (void)headerDict.get<word>("object");

    // The "note" entry is optional
    headerDict.readIfPresent("note", note_);

    // The "arch" information may be missing
    sizeofLabel_ = static_cast<unsigned char>(sizeof(label));
    sizeofScalar_ = static_cast<unsigned char>(sizeof(scalar));

    if (string arch; headerDict.readIfPresent("arch", arch))
    {
        if (auto val = foamVersion::labelByteSize(arch); val > 0)
        {
            sizeofLabel_ = static_cast<unsigned char>(val);
        }
        if (auto val = foamVersion::scalarByteSize(arch); val > 0)
        {
            sizeofScalar_ = static_cast<unsigned char>(val);
        }
    }

    return streamOpt;
}


bool Foam::IOobject::readHeader(dictionary& headerDict, Istream& is)
{
    if (IOobject::debug)
    {
        InfoInFunction << "Reading header for file " << is.name() << endl;
    }

    // Check Istream not already bad
    if (!is.good())
    {
        if (isReadRequired())
        {
            FatalIOErrorInFunction(is)
                << " stream not open for reading essential object from file "
                << is.relativeName()
                << exit(FatalIOError);
        }

        if (IOobject::debug)
        {
            SeriousIOErrorInFunction(is)
                << " stream not open for reading from file "
                << is.relativeName() << endl;
        }

        return false;
    }

    token firstToken(is);

    if (is.good() && firstToken.isWord("FoamFile"))
    {
        headerDict.read(is, false);  // Read sub-dictionary content

        IOstreamOption streamOpt = parseHeader(headerDict);

        is.format(streamOpt.format());
        is.version(streamOpt.version());
        is.setLabelByteSize(sizeofLabel_);
        is.setScalarByteSize(sizeofScalar_);
    }
    else
    {
        IOWarningInFunction(is)
            << "First token could not be read or is not 'FoamFile'"
            << nl << nl
            << "Check header is of the form:" << nl << endl;

        writeHeader(Info);

        // Mark as not read
        headerClassName_.clear();

        return false;
    }

    // Check stream is still OK
    objState_ = (is.good() ? objectState::GOOD : objectState::BAD);

    if (IOobject::debug)
    {
        Info<< " .... read - state: "
            << (objState_ == objectState::GOOD ? "good" : "bad")
            << endl;

    }

    if (objState_ == objectState::BAD)
    {
        if (isReadRequired())
        {
            FatalIOErrorInFunction(is)
                << " stream failure while reading header"
                << " on line " << is.lineNumber()
                << " of file " << is.relativeName()
                << " for essential object:" << name()
                << exit(FatalIOError);
        }

        if (IOobject::debug)
        {
            InfoInFunction
                << "Stream failure while reading header"
                << " on line " << is.lineNumber()
                << " of file " << is.relativeName() << endl;
        }

        return false;
    }

    return true;
}


bool Foam::IOobject::readHeader(Istream& is)
{
    dictionary headerDict;
    return IOobject::readHeader(headerDict, is);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::IOobject::readAndCheckHeader
(
    const bool isGlobal,
    const word& typeName,
    const bool checkType,
    const bool search,
    const bool verbose
)
{
    // Mark as not yet read. cf, IOobject::readHeader()
    headerClassName_.clear();

    bool ok = false;        // Local status
    bool mismatch = false;  // Matching expected vs read type?
    fileName fName;         // The resolved file name

    // Everyone check or just master
    const bool masterOnly
    (
        isGlobal
     && IOobject::fileModificationChecking_masterOnly()
    );

    const auto& handler = Foam::fileHandler();

    if (masterOnly)
    {
        if (UPstream::master())
        {
            // Force master-only header reading
            const auto oldParRun = UPstream::parRun(false);

            fName = handler.filePath(isGlobal, *this, typeName, search);
            ok = handler.readHeader(*this, fName, typeName);

            UPstream::parRun(oldParRun);

            mismatch = (ok && checkType && !isHeaderClass(typeName));
            if (mismatch)
            {
                ok = false;
            }
        }

        // Make sure all processors know about the read information.
        // Note: should ideally be inside fileHandler...
        Pstream::broadcasts
        (
            UPstream::worldComm,
            ok,
            headerClassName_,
            note_
        );
    }
    else
    {
        // All read header
        fName = handler.filePath(isGlobal, *this, typeName, search);
        ok = handler.readHeader(*this, fName, typeName);

        mismatch = (ok && checkType && !isHeaderClass(typeName));
        if (mismatch)
        {
            ok = false;
        }
    }

    // Warn if the header class does not match the expected input.
    // For master-only reading, only reports on the master.
    if (mismatch && verbose)
    {
        WarningInFunction
            << "Unexpected class name \"" << headerClassName()
            << "\" expected \"" << typeName
            << "\" when reading " << fName << endl;
    }

    return ok;
}


// ************************************************************************* //
