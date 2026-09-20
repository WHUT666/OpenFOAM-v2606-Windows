/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2015-2020 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public L2020icense as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::functionObjects::readFields

Group
    grpFieldFunctionObjects

Description
    Reads fields from the time directories and adds them to the mesh database
    for further post-processing.

    Operands:
    \table
      Operand        | Type             | Location
      input          | vol<Type>Field   | <time>/inputField
      output file    | -                | -
      output field   | vol<Type>Field   | Mesh database
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    readFieldsFO
    {
        // Mandatory entries
        type        readFields;
        libs        (fieldFunctionObjects);
        fields      (<wordList>);  // (<field1> <field2> ... <fieldN>);

        // Optional entries
        readOnStart <bool>;;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: readFields              | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      fields       | Names of the operand fields        | wordList |  yes  | -
      readOnStart  | Flag to start reading on start-up  | bool | no    | true
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    readFields.C
    readFieldsTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_readFields_H
#define Foam_functionObjects_readFields_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"
#include "surfaceFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class readFields Declaration
\*---------------------------------------------------------------------------*/

class readFields
:
    public fvMeshFunctionObject
{
protected:

    // Protected Data

        //- Read immediately on construction (default: true)
        bool readOnStart_;

        //- Fields to load
        wordList fieldSet_;


    // Protected Member Functions

        //- Attempt load from io, store on database if successful
        template<class FieldType>
        bool loadAndStore(const IOobject& io);

        //- Forward to loadAndStore for supported types
        template<class Type>
        bool loadField(const IOobject& io);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "readFields");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        readFields
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        readFields(const readFields&) = delete;

        //- No copy assignment
        void operator=(const readFields&) = delete;


    //- Destructor
    virtual ~readFields() = default;


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
    #include "readFieldsTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
