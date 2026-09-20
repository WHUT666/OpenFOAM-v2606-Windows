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
    Foam::functionObjects::add

Group
    grpFieldFunctionObjects

Description
    Sums a given list of (at least two or more) fields and outputs the result
    into a new field. All input fields must have matching sizes and compatible
    dimensions; otherwise, an error will be raised.

    \verbatim
        fieldResult = field1 + field2 + ... + fieldN
    \endverbatim

    Operands:
    \table
      Operand      | Type                          | Location
      input        | {vol,surface}<Type>Field(s)   | <time>/inputFields
      output file  | -                             |-
      output field | {vol,surface}<Type>Field      | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    addFO
    {
        // Mandatory entries
        type            add;
        libs            (fieldFunctionObjects);

        // Inherited entries
        fields          (<wordList>);  // (<field1> <field2> ... <fieldN>);
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd | Deflt
      type         | Type name: add                     | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields | Names of operand fields (at least two) | wordList | yes | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldsExpression.H \endlink

SourceFiles
    add.C
    addTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_add_H
#define Foam_functionObjects_add_H

#include "fieldsExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                           Class add Declaration
\*---------------------------------------------------------------------------*/

class add
:
    public fieldsExpression
{
    // Private Member Functions

        //- Add the list of fields of the specified type
        //- and return the result
        template<class GeoFieldType>
        tmp<GeoFieldType> calcFieldType() const;

        //- Add the list of fields and return true if successful
        virtual bool calc();


public:

    //- Grant fieldsExpression access to private and protected members
    friend class fieldsExpression;


    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "add");


    // Constructors

        //- Construct from name, Time and dictionary
        add
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        add(const add&) = delete;

        //- No copy assignment
        void operator=(const add&) = delete;


    //- Destructor
    virtual ~add() = default;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "addTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
