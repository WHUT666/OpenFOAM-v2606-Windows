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
    Foam::functionObjects::subtract

Group
    grpFieldFunctionObjects

Description
    Subtracts a given list of (at least one or more) fields from a field and
    produces a new field, where the fields possess the same sizes and
    dimensions.

    \verbatim
        fieldResult = field1 - field2 - ... - fieldN
    \endverbatim

    Operands:
    \table
      Operand      | Type                          | Location
      input        | {vol,surface}<Type>Field(s)   | <time>/inputFields
      output file  | -                             | -
      output field | {vol,surface}<Type>Field      | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    subtractFO
    {
        // Mandatory entries
        type            subtract;
        libs            (fieldFunctionObjects);
        fields          (<wordList>);  // (<field1> <field2> ... <fieldN>);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: subtract                | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields       | Names of the operand fields        | wordList |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldsExpression.H \endlink

SourceFiles
    subtract.C
    subtractTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_subtract_H
#define Foam_functionObjects_subtract_H

#include "fieldsExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                           Class subtract Declaration
\*---------------------------------------------------------------------------*/

class subtract
:
    public fieldsExpression
{
    // Private Member Functions

        //- Subtract the list of fields of the specified type
        //- and return the result
        template<class GeoFieldType>
        tmp<GeoFieldType> calcFieldType() const;

        //- Subtract the list of fields and return true if successful
        virtual bool calc();


public:

    //- Grants fieldsExpression access to private and protected members
    friend class fieldsExpression;


    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "subtract");


    // Constructors

        //- Construct from name, Time and dictionary
        subtract
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        subtract(const subtract&) = delete;

        //- No copy assignment
        void operator=(const subtract&) = delete;


    //- Destructor
    virtual ~subtract() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "subtractTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
