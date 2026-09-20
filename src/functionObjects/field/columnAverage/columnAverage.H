/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2020 OpenCFD Ltd.
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
    Foam::functionObjects::columnAverage

Group
    grpFieldFunctionObjects

Description
    Computes the arithmetic average of given quantities along columns of cells
    in a given direction for structured-like layered meshes. It is, for example,
    useful for channel-like cases where spanwise average of a field is desired.
    However, the \c columnAverage function object does not operate on arbitrary
    unstructured meshes.

    For each patch face, calculates the average value of all cells attached in
    the patch face normal direction, and then pushes the average value back
    to all cells in the column.

    Operands:
    \table
      Operand           | Type               | Location
      input             | vol<Type>Field     | <time>/inputField
      output file       | -                  | -
      output field      | vol<Type>Field     | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    columnAverageFO
    {
        // Mandatory entries
        type            columnAverage;
        libs            (fieldFunctionObjects);
        patches         (<wordRes>);   // (<patch1> <patch2> ... <patchN>);
        fields          (<wordList>);  // (<field1> <field2> ... <fieldN>);

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: columnAverage           | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      patches      | Names of patches to collapse onto  | wordRes  |  yes  | -
      fields       | Names of the operand fields        | wordList |  yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    columnAverage.C
    columnAverageTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_columnAverage_H
#define Foam_functionObjects_columnAverage_H

#include "volFieldsFwd.H"
#include "fvMeshFunctionObject.H"
#include "volFieldSelection.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class globalIndex;
class meshStructure;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                        Class columnAverage Declaration
\*---------------------------------------------------------------------------*/

class columnAverage
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Patches on which to collapse the fields (in sorted order)
        labelList patchIDs_;

        //- Fields to collapse
        volFieldSelection fieldSet_;

        //- Global addressing and mesh structure for column-based addressing
        mutable autoPtr<globalIndex> globalFaces_;
        mutable autoPtr<globalIndex> globalEdges_;
        mutable autoPtr<globalIndex> globalPoints_;
        mutable autoPtr<meshStructure> meshStructurePtr_;


    // Private Member Functions

        //- Create the column average field name
        const word averageName(const word& fieldName) const;

        //- Return the column-based addressing
        const meshStructure& meshAddressing(const polyMesh&) const;

        //- Calculate the averaged field and return true if successful
        template<class Type>
        bool columnAverageField(const word& fieldName);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "columnAverage");


    // Constructors

        //- Construct from name, Time and dictionary
        columnAverage
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        columnAverage(const columnAverage&) = delete;

        //- No copy assignment
        void operator=(const columnAverage&) = delete;


    //- Destructor
    virtual ~columnAverage() = default;


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
    #include "columnAverageTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
