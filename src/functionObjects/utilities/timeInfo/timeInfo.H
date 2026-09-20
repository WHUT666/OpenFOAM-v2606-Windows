/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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
    Foam::functionObjects::timeInfo

Description
    Writes the run time (time-step), cpuTime and clockTime -
    optionally with cpuTime and clockTime change for each time step.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    timeInfoFO
    {
        // Mandatory entries
        type            timeInfo;
        libs            (utilityFunctionObjects);

        // Optional entries
        perTimeStep     <bool>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                          | Type | Reqd | Deflt
      type         | Type name: timeActivatedFileUpdate   | word | yes  | -
      libs         | Library name: utilityFunctionObjects | word | yes  | -
      perTimeStep   | Write value per interval/step       | bool | no   | false
    \endtable

    The inherited entries are elaborated in:
      - \link timeFunctionObject.H \endlink
      - \link writeFile.H \endlink

Note
    The initial per-step value is likely to be inaccurate and should
    mostly be ignored.

SourceFiles
    timeInfo.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_timeInfo_H
#define Foam_functionObjects_timeInfo_H

#include "timeFunctionObject.H"
#include "writeFile.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class timeInfo Declaration
\*---------------------------------------------------------------------------*/

class timeInfo
:
    public timeFunctionObject,
    public writeFile
{
    // Private Member Data

        //- The cpuTime from last write/time-step (for perTimeStep)
        scalar cpuTime0_;

        //- The clockTime from last write/time-step (for perTimeStep)
        scalar clockTime0_;

        //- Flag to write cpuTime and clockTime per time-step
        bool perTimeStep_;


protected:

    // Protected Member Functions

        //- Output file header information
        virtual void writeFileHeader(Ostream& os);

        //- No copy construct
        timeInfo(const timeInfo&) = delete;

        //- No copy assignment
        void operator=(const timeInfo&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "timeInfo");


    // Constructors

        //- Construct from name, Time and dictionary
        timeInfo
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~timeInfo() = default;


    // Member Functions

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
