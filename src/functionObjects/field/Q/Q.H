/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
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
    Foam::functionObjects::Q

Group
    grpFieldFunctionObjects

Description
    Computes the second invariant of the velocity gradient tensor
    \f$[s^{-2}]\f$.

    \f[
        \vec Q = \frac{1}{2}
        [ (tr (\nabla \vec U) )^2
        - tr (\nabla \vec U \cdot \nabla \vec U) ]
    \f]

    where
    \vartable
        \vec U       | velocity [m/s]
    \endvartable

    Operands:
    \table
      Operand        | Type           | Location
      input          | volVectorField | <time>/inputField
      output file    | -              | -
      output field   | volScalarField | <time>/outputField
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    QFO
    {
        // Mandatory entries
        type        Q;
        libs        (fieldFunctionObjects);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: Q                       | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    Q.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_Q_H
#define Foam_functionObjects_Q_H

#include "fieldExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class Q Declaration
\*---------------------------------------------------------------------------*/

class Q
:
    public fieldExpression
{
    // Private Member Functions

        //- Calculate the Q field and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "Q");


    // Constructors

        //- Construct from name, Time and dictionary
        Q
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        Q(const Q&) = delete;

        //- No copy assignment
        void operator=(const Q&) = delete;


    //- Destructor
    virtual ~Q() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
