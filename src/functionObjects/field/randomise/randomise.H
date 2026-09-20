/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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
    Foam::functionObjects::randomise

Group
    grpFieldFunctionObjects

Description
    Adds a random component to an input field, with a specified perturbation
    magnitude.

    Operands:
    \table
      Operand        | Type             | Location
      input          | vol<Type>Field   | <time>/inputField
      output file    | -                | -
      output field   | vol<Type>Field   | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    randomiseFO
    {
        // Mandatory entries
        type            randomise;
        libs            (fieldFunctionObjects);
        magPerturbation <scalar>;
        field           <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                          | Type | Reqd  | Deflt
      type         | Type name: randomise                 | word |  yes  | -
      libs         | Library name: fieldFunctionObjects   | word |  yes  | -
      magPerturbation | The magnitude of the perturbation | scalar |  yes | -
      field        | Name of the operand field            | word | yes   | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    randomise.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_randomise_H
#define Foam_functionObjects_randomise_H

#include "fieldExpression.H"
#include "volRegion.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                           Class randomise Declaration
\*---------------------------------------------------------------------------*/

class randomise
:
    public fieldExpression,
    public volRegion
{
    // Private Data

        //- The magnitude of the perturbation
        scalar magPerturbation_;


    // Private Member Functions

        //- Calculate randomised field and register result
        //  Defined as file-local template implementation
        template<class Type>
        bool calcTemplate();


protected:

    // Protected Member Functions

        //- Calculate the randomised field and return true if successful
        virtual bool calc();

        //- No copy construct
        randomise(const randomise&) = delete;

        //- No copy assignment
        void operator=(const randomise&) = delete;


public:


    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "randomise");


    // Constructors

        //- Construct from name, Time and dictionary
        randomise
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~randomise() = default;


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
