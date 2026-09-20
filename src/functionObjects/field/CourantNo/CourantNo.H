/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
    Copyright (C) 2015-2020 OpenCFD Ltd.
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
    Foam::functionObjects::CourantNo

Group
    grpFieldFunctionObjects

Description
    Computes the Courant number field for time-variant simulations.

    Operands:
    \table
      Operand           | Type           | Location
      input             | -              | -
      output file       | -              | -
      output field      | volScalarField | <time>/outputField
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    CourantNoFO
    {
        // Mandatory entries
        type        CourantNo;
        libs        (fieldFunctionObjects);

        // Optional entries
        rho         <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property   | Description                        | Type | Reqd  | Deflt
      type       | Type name: CourantNo               | word |  yes  | -
      libs       | Library name: fieldFunctionObjects | word |  yes  | -
      rho        | Name of density field              | word |  no   | rho
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    CourantNo.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_CourantNo_H
#define Foam_functionObjects_CourantNo_H

#include "fieldExpression.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class CourantNo Declaration
\*---------------------------------------------------------------------------*/

class CourantNo
:
    public fieldExpression
{
    // Private Data

        //- Name of density field
        word rhoName_;


    // Private Member Functions

        //- Calculate the Courant number field and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "CourantNo");


    // Constructors

        //- Construct from name, Time and dictionary
        CourantNo
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        CourantNo(const CourantNo&) = delete;

        //- No copy assignment
        void operator=(const CourantNo&) = delete;


    //- Destructor
    virtual ~CourantNo() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
