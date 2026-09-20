/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::functionObjects::streamFunction

Group
    grpFieldFunctionObjects

Description
    Computes the stream function (i.e. https://w.wiki/Ncm).

    Operands:
    \table
      Operand        | Type                   | Location
      input          | surfaceScalarField     | <time>/inputField
      output file    | -                      | -
      output field   | pointScalarField       | <time>/outputField
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    streamFunctionFO
    {
        // Mandatory entries
        type        streamFunction;
        libs        (fieldFunctionObjects);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: streamFunction          | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    streamFunction.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_streamFunction_H
#define Foam_functionObjects_streamFunction_H

#include "fieldExpression.H"
#include "surfaceFieldsFwd.H"
#include "pointFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class streamFunction Declaration
\*---------------------------------------------------------------------------*/

class streamFunction
:
    public fieldExpression
{
    // Private Member Functions

        //- Return the stream function field
        tmp<pointScalarField> calc(const surfaceScalarField& phi) const;

        //- Calculate the stream-function and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "streamFunction");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        streamFunction
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        streamFunction(const streamFunction&) = delete;

        //- No copy assignment
        void operator=(const streamFunction&) = delete;


    //- Destructor
    virtual ~streamFunction() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
