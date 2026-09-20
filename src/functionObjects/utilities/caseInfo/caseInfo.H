/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2023 OpenCFD Ltd.
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

Class
    Foam::functionObjects::caseInfo

Description
    Collects and writes case information to file.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    caseInfoFO
    {
        // Mandatory entries
        type            caseInfo;
        libs            (utilityFunctionObjects);

        // Optional entries
        lookupMode      <word>; // none | warn | error;
        writeFormat     <word>; // dictionary | json;

        dictionaries
        {
            USolver // User-specified names
            {
                // Look up using registered name
                name        "fvSolution";

                // Optionally limit to specific entries
                include
                (
                    "solvers/U/solver"
                );
            }
            fvSchemes
            {
                name        "fvSchemes";

                // include all entries by default
            }
            timeScheme
            {
                name        "fvSchemes";

                include
                (
                    "/ddtSchemes/default"
                );
            }

            turbulence
            {
                name        "turbulenceProperties";

                // include all entries by default
            }
            controlDict
            {
                // Look up using file path
                path        "<case>/system/controlDict";

                include
                (
                    "application"
                    "deltaT"
                    "startTime"
                    "endTime"
                );
            }
        }

        functionObjects (minMax1); // v2306 only

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: caseInfo                | word | yes  | -
      libs     | Library name: utilityFunctionObjects | word | yes | -
      lookupMode    | Lookup mode                   | word | no  | warn
      writeFormat   | Write format                  | word | no  | -
      dictionaries  | Dictionaries to process       | word | no  | -
      functionObjects | Function objects to process | word | no  | -
    \endtable

    Options for the \c writeFormat entry:
    \verbatim
      dictionary    | OpenFOAM native dictionary format
      json          | JSON output format
    \endverbatim

    Options for the \c lookupMode entry:
    \verbatim
      none          | Do not report missing entries.
      warn          | Warn about missing entries.
      error         | Error and abort on missing entries.
    \endverbatim

    The inherited entries are elaborated in:
      - \link stateFunctionObject.H \endlink
      - \link writeFile.H \endlink

SourceFiles
    caseInfo.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_caseInfo_H
#define Foam_functionObjects_caseInfo_H

#include "IOdictionary.H"
#include "writeFile.H"
#include "stateFunctionObject.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class fvMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class caseInfo Declaration
\*---------------------------------------------------------------------------*/

class caseInfo
:
    public IOdictionary,
    public stateFunctionObject,
    public writeFile
{
public:

    // Public Enumerations

        //- Write format enumeration
        enum class writeFormat
        {
            dict,
            json
        };

        //- Lookup mode enumeration
        enum class lookupMode
        {
            none,
            warn,
            error
        };


private:

    // Private Data

        //- Write format names
        static utilityFunctionObjects_API const Enum<writeFormat> writeFormatNames_;

        //- Lookup mode names
        static utilityFunctionObjects_API const Enum<lookupMode> lookupModeNames_;

        //- Write/output format, e.g. dictionary, JSON
        writeFormat writeFormat_;

        //- Lookup mode when reading entries
        lookupMode lookupMode_;

        //- Dictionaries
        dictionary dictionaries_;

        //- List of function objects to process
        wordList functionObjectNames_;


    // Private Member Functions

        //- Report
        void report(const string& str) const;

        //- Process dictionary
        void processDict
        (
            dictionary& dict,
            const dictionary& targetDict,
            const entry* includePtr,
            const entry* excludePtr
        ) const;


protected:

    // Protected Member Functions

        //- No copy construct
        caseInfo(const caseInfo&) = delete;

        //- No copy assignment
        void operator=(const caseInfo&) = delete;


    // Write data

        //- Write case meta data
        void writeMeta(dictionary& dict) const;

        //- Write registered dictionaries
        void writeRegisteredDicts
        (
            const objectRegistry& obr,
            dictionary& dict,
            dictionary& dictionaries
        ) const;

        //- Write file-based dictionaries
        void writeFileDicts
        (
            dictionary& dict,
            dictionary& dictionaries
        ) const;

        //- Write function object results
        void writeFunctionObjects(dictionary& dict) const;

        //- Write mesh statistics
        void writeMeshStats(const polyMesh& mesh, dictionary& dict) const;

        //- Write mesh patches
        void writePatches(const fvMesh& mesh, dictionary& dict) const;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "caseInfo");


    // Constructors

        //- Construct from name, Time and dictionary
        caseInfo
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~caseInfo() = default;


    // Member Functions

        using regIOobject::read;
        using regIOobject::write;

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations (no-op)
        virtual bool execute();

        //- Write the function-object results
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //