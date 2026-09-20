/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2020 OpenCFD Ltd.
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
    Foam::functionObjects::timeActivatedFileUpdate

Group
    grpUtilitiesFunctionObjects

Description
    Performs a file copy/replacement once a specified time has been reached.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    fileUpdateFO
    {
        // Mandatory entries
        type              timeActivatedFileUpdate;
        libs              (utilityFunctionObjects);

        fileToUpdate      <fileName>;  // "<system>/fvSolution";
        timeVsFile        <Tuple2<scalar, fileName>>;
        timeVsFile
        (
            (-1   "<system>/fvSolution.0")
            (0.10 "<system>/fvSolution.10")
            (0.20 "<system>/fvSolution.20")
            (0.35 "<system>/fvSolution.35")
        );

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                          | Type | Reqd | Deflt
      type         | Type name: timeActivatedFileUpdate   | word | yes  | -
      libs         | Library name: utilityFunctionObjects | word | yes  | -
      fileToUpdate | Name of the file to update           | fileName | yes | -
      timeVsFile   | List of times vs files    | scalar, fileName | yes | -
    \endtable

    The inherited entries are elaborated in:
      - \link timeFunctionObject.H \endlink

SourceFiles
    timeActivatedFileUpdate.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_timeActivatedFileUpdate_H
#define Foam_functionObjects_timeActivatedFileUpdate_H

#include "timeFunctionObject.H"
#include "Tuple2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class Time;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                   Class timeActivatedFileUpdate Declaration
\*---------------------------------------------------------------------------*/

class timeActivatedFileUpdate
:
    public functionObjects::timeFunctionObject
{
    // Private Member Data

        //- Name of file to update
        fileName fileToUpdate_;

        //- List of times vs filenames
        List<Tuple2<scalar, fileName>> timeVsFile_;

        //- Index of last file copied
        label lastIndex_;

        //- Has anything been copied?
        bool modified_;


    // Private Member Functions

        //- Update file
        void updateFile();

        //- No copy construct
        timeActivatedFileUpdate(const timeActivatedFileUpdate&) = delete;

        //- No copy assignment
        void operator=(const timeActivatedFileUpdate&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "timeActivatedFileUpdate");


    // Constructors

        //- Construct from name, Time and dictionary
        timeActivatedFileUpdate
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~timeActivatedFileUpdate() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary&);

        //- Execute the function-object operations
        virtual bool execute();

        //- Write the function-object results (no-op)
        virtual bool write();

        //- Did any file get changed during execution?
        virtual bool filesModified() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
