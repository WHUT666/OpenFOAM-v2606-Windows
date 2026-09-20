/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020-2025 OpenCFD Ltd.
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
    Foam::functionObjects::multiply

Group
    grpFieldFunctionObjects

Description
    Multiplies a given list of (at least two or more) fields and outputs the
    result into a new field.

    \verbatim
        fieldResult = field1 * field2 * ... * fieldN
    \endverbatim

    Operands:
    \table
      Operand      | Type                       | Location
      input field  | vol<Type>Field(s)          | <time>/inputFields
      output file  | -                          | -
      output field | vol<Type>Field             | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    multiplyFO
    {
        // Mandatory entries
        type        multiply;
        libs        (fieldFunctionObjects);

        // Inherited entries
        fields      (<wordList>);  // (<field1> <field2> ... <fieldN>);
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd | Deflt
      type         | Type name: multiply                | word | yes  | -
      libs         | Library name: fieldFunctionObjects | word | yes  | -
      fields       | Names of the operand fields        | wordList | yes | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldsExpression.H \endlink

SourceFiles
    multiply.C
    multiplyTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_multiply_H
#define Foam_functionObjects_multiply_H

#include "fieldsExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class multiply Declaration
\*---------------------------------------------------------------------------*/

class multiply
:
    public fieldsExpression
{
    //- Helper struct to define valid operations
    template<class Type1, class Type2>
    struct is_valid_op
    {
        static constexpr bool value =
            (pTraits<Type1>::rank == 0 || pTraits<Type2>::rank == 0)
         || (pTraits<Type1>::rank == 1 && pTraits<Type2>::rank == 1);
    };


    // Private Member Functions

        //- Initialise the result field
        template<class Type>
        bool initialiseResult(const word& fieldName);

        //- Multiply the result field with the given field
        template<class Type>
        bool multiplyResult(const word& fieldName, bool& processed);

        //- Multiply two fields of given types and return true if successful
        template<class Type1, class Type2>
        bool multiplyFieldType
        (
            GeometricField<Type1, fvPatchField, volMesh>& result,
            const word& fieldName,
            bool& processed
        );

        //- Multiply the list of fields and return true if successful
        virtual bool calc();


public:

    //- Grant fieldsExpression access to private and protected members
    friend class fieldsExpression;


    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "multiply");


    // Constructors

        //- Construct from name, Time and dictionary
        multiply
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        multiply(const multiply&) = delete;

        //- No copy assignment
        void operator=(const multiply&) = delete;


    //- Destructor
    virtual ~multiply() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "multiplyTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
