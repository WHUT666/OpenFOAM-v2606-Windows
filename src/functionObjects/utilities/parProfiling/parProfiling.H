/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2023 OpenCFD Ltd.
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
    Foam::functionObjects::parProfiling

Group
    grpUtilitiesFunctionObjects

Description
    Simple (simplistic) mpi-profiling.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    profilingFO
    {
        // Mandatory entries
        type                parProfiling;
        libs                (utilityFunctionObjects);

        // Optional entries
        detail              <int>;  // 0;

        // Inherited entries
        // Report stats on exit only (instead of every time step)
        executeControl  onEnd;
        writeControl    none;
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: parProfiling            | word | yes  | -
      libs     | Library name: utilityFunctionObjects | word | yes | -
      detail   | The reporting level                | int  | no   | 0
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    parProfiling.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_parProfiling_H
#define Foam_functionObjects_parProfiling_H

#include "functionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                        Class parProfiling Declaration
\*---------------------------------------------------------------------------*/

class parProfiling
:
    public functionObject
{
    // Private Data

        //- The reporting level
        //  0: summary, 1: per-proc times, 2: per-proc times/counts
        int reportLevel_;

public:

    // Generated Methods

        //- No copy construct
        parProfiling(const parProfiling&) = delete;

        //- No copy assignment
        void operator=(const parProfiling&) = delete;


    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "parProfiling");


    // Constructors

        //- Construct from name, Time and dictionary. Enables profilingPstream
        parProfiling
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor. Disables profilingPstream
    virtual ~parProfiling();


    // Member Functions

        //- Report the current profiling information
        void report();

        //- Execute the function-object operations
        virtual bool execute();

        //- Write the function-object results (no-op)
        virtual bool write();

        //- Disables profilingPstream
        virtual bool end();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
