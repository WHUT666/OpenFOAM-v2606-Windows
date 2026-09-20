/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::functionObjects::surfaceInterpolate

Group
    grpFieldFunctionObjects

Description
    Linearly interpolates volume fields to generate surface fields.

    Operands:
    \table
      Operand        | Type                  | Location
      input          | vol<Type>Field        | <time>/inputFields
      output file    | -                     | -
      output field   | surfaceTypeField      | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    surfaceInterpolateFO
    {
        // Mandatory entries
        type        surfaceInterpolate;
        libs        (fieldFunctionObjects);
        fields      ((<inpField1> <outField1>) ... (<inpFieldN> <outFieldN>));

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                           | Type | Reqd | Deflt
      type         | Type name: surfaceInterpolate         | word |  yes  | -
      libs         | Library name: fieldFunctionObjects    | word |  yes  | -
      fields       | Names of operand and output fields    | wordList | yes | -
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    surfaceInterpolate.C
    surfaceInterpolateTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_surfaceInterpolate_H
#define Foam_functionObjects_surfaceInterpolate_H

#include "fvMeshFunctionObject.H"
#include "surfaceFieldsFwd.H"
#include "Tuple2.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                    Class surfaceInterpolate Declaration
\*---------------------------------------------------------------------------*/

class surfaceInterpolate
:
    public fvMeshFunctionObject
{
protected:

    // Protected Data

        //- Fields to process
        List<Tuple2<word, word>> fieldSet_;


    // Protected Member Functions

        //- Linearly interpolate volume fields to generate surface fields
        template<class Type>
        void interpolateFields();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "surfaceInterpolate");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        surfaceInterpolate
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        surfaceInterpolate(const surfaceInterpolate&) = delete;

        //- No copy assignment
        void operator=(const surfaceInterpolate&) = delete;


    //- Destructor
    virtual ~surfaceInterpolate() = default;


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
    #include "surfaceInterpolateTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
