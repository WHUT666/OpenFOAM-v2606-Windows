/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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
    Foam::functionObjects::dsmcFields

Group
    grpLagrangianFunctionObjects

Description
    This function object calculates and outputs the intensive fields:
    - UMean
    - translationalT
    - internalT
    - overallT
    from averaged extensive fields from a DSMC calculation.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    dsmcFieldsFO
    {
        // Mandatory entries
        type        dsmcFields;
        libs        (lagrangianFunctionObjects);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                | Type | Reqd | Deflt
      type      | Type name: dsmcFields      | word | yes  | -
      libs      | Library name: lagrangianFunctionObjects | word | yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    dsmcFields.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_dsmcFields_H
#define Foam_functionObjects_dsmcFields_H

#include "fvMeshFunctionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class dsmcFields Declaration
\*---------------------------------------------------------------------------*/

class dsmcFields
:
    public fvMeshFunctionObject
{

    // Private Member Functions

        //- No copy construct
        dsmcFields(const dsmcFields&) = delete;

        //- No copy assignment
        void operator=(const dsmcFields&) = delete;


public:

    //- Runtime type information
    TypeNameApi(lagrangianFunctionObjects_API, "dsmcFields");


    // Constructors

        //- Construct from name, Time and dictionary
        dsmcFields
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~dsmcFields() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary&);

        //- Execute the function-object operations, currently does nothing
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
