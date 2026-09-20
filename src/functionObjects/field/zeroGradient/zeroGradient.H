/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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
    Foam::functionObjects::zeroGradient

Group
    grpFieldFunctionObjects

Description
    Creates a volume field with zero-gradient boundary conditions from another
    volume field.

    The result can be used, for example, to post-process near-wall field values.

    Operands:
    \table
      Operand        | Type                | Location
      input          | vol<Type>Field      | <time>/inputField
      output file    | -                   | -
      output field   | vol<Type>Field      | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    zeroGradientFO
    {
        // Mandatory entries
        type        zeroGradient;
        libs        (fieldFunctionObjects);
        fields      (<field1> ... <fieldN>); \\(U "(T|k|epsilon|omega)");

        // Optional entries
        result      @@<name>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                           | Type | Reqd | Deflt
      type         | Type name: zeroGradient               | word |  yes  | -
      libs         | Library name: fieldFunctionObjects    | word |  yes  | -
      fields       | Name of the operand fields            | wordList | yes | -
      result       | Name of the output field    | word | no | zeroGradient(@@)
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

Note
    - A list of fields can contain exact names or regular expressions.
    The token '\@\@' in the result name is replaced by the name of the source
    field. In the special case of a single source field (specified as
    a non-regex), the '\@\@' token checking is suppressed.
    - The function object will skip over fields that would not benefit
    i.e., only processor, empty, zeroGradient, symmetry patches.
    This check should also prevent processing fields multiple times.

SourceFiles
    zeroGradient.C
    zeroGradientTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_zeroGradient_H
#define Foam_functionObjects_zeroGradient_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"
#include "OFstream.H"
#include "wordRes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                        Class zeroGradient Declaration
\*---------------------------------------------------------------------------*/

class zeroGradient
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Name of fields to process
        wordRes selectFields_;

        //- Formatting for the result fields
        word resultName_;

        //- Hashed names of result fields, and their type
        HashTable<word> results_;


    // Private Member Functions

        //- Accept unless field only has constraint patches
        //  (ie, empty/zero-gradient/processor)
        //  This should also avoid fields that were already processed by
        //  zeroGradient.
        template<class Type>
        static bool accept(const GeometricField<Type, fvPatchField, volMesh>&);

        //- Apply for the volume field type
        template<class Type>
        int apply(const word& inputName, int& state);

        //- Process by trying to apply for various volume field types
        int process(const word& inputName);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "zeroGradient");


    // Constructors

        //- Construct from name, Time and dictionary
        zeroGradient
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        zeroGradient(const zeroGradient&) = delete;

        //- No copy assignment
        void operator=(const zeroGradient&) = delete;


    //- Destructor
    virtual ~zeroGradient() = default;


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

#ifdef NoRepository
    #include "zeroGradientTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
