/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2021 OpenCFD Ltd.
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
    Foam::functionObjects::reference

Group
    grpFieldFunctionObjects

Description
    Computes a field whose values are offset to a reference value obtained by
    from a Function1.

    The field values are calculated using:

    \f[
        r_c = s(f_{c}(t) - f_p + f_{off})
    \f]

    where
    \vartable
        r_c             | field values at cell
        s               | optional scale factor (default = 1)
        f_{c}(t)        | current field values at cell at this time
        f_p             | reference value
        f_{off}         | offset field value (default = 0)
    \endvartable

    Operands:
    \table
      Operand       | Type                       | Location
      input         | {vol,surface}<Type>Field   | <time>/inputField
      output file   | -                          | -
      output field  | {vol,surface}<Type>Field   | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    referenceFO
    {
        // Mandatory entries
        type        reference;
        libs        (fieldFunctionObjects);
        field       <word>;
        refValue    <Function1>;

        // Optional entries
        scale       <scalar>;
        offset      <scalar>;

        // Inherited entries
        ...
    }
    \endverbatim

    Where the entries comprise:
    \table
        Property     | Description                | Type | Reqd  | Deflt
        type         | Type name: reference       | word | yes   | -
        field        | Name of the operand field  | word | yes   | -
        refValue     | Function1 reference value  | Function1 | yes | -
        offset       | Offset value               | type-value | no  | zero
        scale        | Scale factor               | scalar | no  | 1
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

SourceFiles
    reference.C
    referenceTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_reference_H
#define Foam_functionObjects_reference_H

#include "fieldExpression.H"
#include "point.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class reference Declaration
\*---------------------------------------------------------------------------*/

class reference
:
    public fieldExpression
{
    // Private Data

        //- Local copy of dictionary used for construction
        dictionary localDict_;

        //- Scale factor
        scalar scale_;


    // Private Member Functions

        //- Calculate the reference field and return true if successful
        virtual bool calc();

        //- Utility function to calc a given type of field
        template<class Type>
        bool calcType();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "reference");


    // Constructors

        //- Construct from name, Time and dictionary
        reference
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        reference(const reference&) = delete;

        //- No copy assignment
        void operator=(const reference&) = delete;


    //- Destructor
    virtual ~reference() = default;


    // Public Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "referenceTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
