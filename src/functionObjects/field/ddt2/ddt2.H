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
    Foam::functionObjects::ddt2

Group
    grpFieldFunctionObjects

Description
    Computes the magnitude or magnitude squared of the Eulerian time derivative
    of an input volume field for time-variant simulations
    (not appropriate to steady-state simulations).

    The result can be further used for determining e.g. variance or RMS values.

    Operands:
    \table
      Operand           | Type           | Location
      input             | vol<Type>Field   | <time>/inputFields
      output file       | -              | -
      output field      | vol<Type>Field   | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    ddt2FO
    {
        // Mandatory entries
        type            ddt2;
        libs            (fieldFunctionObjects);

        // Optional entries
        result          d@@dt2;
        mag             <bool>;

        // Inherited entries
        fields          <wordList>;  // (<field1> <field2> ... <fieldN>);
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: ddt2                    | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields       | Names of the operand fields        | wordList | yes | -
      mag          | Compute 'mag' instead of 'magSqr'  | bool |  no   | false
      result       | Name of results     | word |  no   | magSqr(ddt2(@@))
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

    A list of fields can contain exact names or regular expressions.
    The token '\@\@' in the result name is replaced by the name of the source
    field. In the special case of a single source field (specified as
    a non-regex), the '\@\@' token checking is suppressed.

    The function object will skip over fields that appear to have
    already been processed (ie, their names are similar to the output names).

SourceFiles
    ddt2.C
    ddt2Templates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_ddt2_H
#define Foam_functionObjects_ddt2_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"
#include "OFstream.H"
#include "regExp.H"
#include "HashSet.H"
#include "wordRes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                            Class ddt2 Declaration
\*---------------------------------------------------------------------------*/

class ddt2
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Name of fields to process
        wordRes selectFields_;

        //- Formatting for the result fields
        word resultName_;

        //- Avoid processing the same field twice
        mutable regExp denyField_;

        //- Hashed names of result fields
        wordHashSet results_;

        //- Flat to use 'mag' instead of 'magSqr'
        //  Cannot be adjusted during the simulation since it alters the
        //  dimensions of the output field
        const bool mag_;


    // Private Member Functions

        //- Accept unless field name appears to have already been processed
        bool accept(const word& fieldName) const;

        //- Apply for the volume field type
        template<class FieldType>
        int apply(const word& inputName, int& state);

        //- Process by trying to apply for various volume field types
        int process(const word& inputName);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "ddt2");


    // Constructors

        //- Construct from name, Time and dictionary
        ddt2
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        ddt2(const ddt2&) = delete;

        //- No copy assignment
        void operator=(const ddt2&) = delete;


    //- Destructor
    virtual ~ddt2() = default;


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
    #include "ddt2Templates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
