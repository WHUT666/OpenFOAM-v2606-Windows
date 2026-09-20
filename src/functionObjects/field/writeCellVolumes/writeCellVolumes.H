/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
    Copyright (C) 2016-2020 OpenCFD Ltd.
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
    Foam::functionObjects::writeCellVolumes

Group
    grpFieldFunctionObjects

Description
    Writes the cell-volumes \c volScalarField.

    Operands:
    \table
      Operand        | Type           | Location
      input          | -              | -
      output file    | -              | -
      output field   | volScalarField | <time>/V
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    writeCellVolumesFO
    {
        // Mandatory entries
        type        writeCellVolumes;
        libs        (fieldFunctionObjects);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property   | Description                        | Type | Reqd  | Deflt
      type       | Type name: writeCellVolumes        | word |  yes  | -
      libs       | Library name: fieldFunctionObjects | word |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    writeCellVolumes.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_writeCellVolumes_H
#define Foam_functionObjects_writeCellVolumes_H

#include "fvMeshFunctionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class writeCellVolumes Declaration
\*---------------------------------------------------------------------------*/

class writeCellVolumes
:
    public fvMeshFunctionObject
{
public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "writeCellVolumes");


    // Constructors

        //- Construct from name, Time and dictionary
        writeCellVolumes
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        writeCellVolumes(const writeCellVolumes&) = delete;

        //- No copy assignment
        void operator=(const writeCellVolumes&) = delete;


    //- Destructor
    virtual ~writeCellVolumes() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations
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
