/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
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
    Foam::functionObjects::mag

Group
    grpFieldFunctionObjects

Description
    Computes the magnitude of an input field.

    Operands:
    \table
      Operand      | Type                     | Location
      input        | {vol,surface}<Type>Field | <time>/inputField
      output file  | -                        | -
      output field | {vol,surface}ScalarField | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

    With the \c subRegion option, also supports fields on function object
    surface output (e.g. sampledSurfaces).

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    magFO
    {
        // Mandatory entries
        type            mag;
        libs            (fieldFunctionObjects);
        field           <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: mag                     | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      field        | Name of the operand field          | word |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    mag.C
    magTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_mag_H
#define Foam_functionObjects_mag_H

#include "fieldExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                           Class mag Declaration
\*---------------------------------------------------------------------------*/

class mag
:
    public fieldExpression
{
    // Private Member Functions

        //- Calculate the magnitude of the field and register the result
        template<class Type>
        bool calcMag();

        //- Calculate the magnitude of the field and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "mag");


    // Constructors

        //- Construct from name, Time and dictionary
        mag
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        mag(const mag&) = delete;

        //- No copy assignment
        void operator=(const mag&) = delete;


    //- Destructor
    virtual ~mag() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "magTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
