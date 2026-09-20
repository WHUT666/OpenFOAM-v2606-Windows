/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
    Copyright (C) 2022 OpenCFD Ltd.
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
    Foam::timeVaryingMappedFixedValuePointPatchField

Description
    A time-varying form of a mapped fixed value boundary condition.

Usage
    \table
        Property     | Description                      | Required | Default
        setAverage   | Use average value                | no | false
        perturb      | Perturb points for regular geometries | no | 1e-5
        points       | Name of points file              | no | points
        fieldTable   | Alternative field name to sample | no | this field name
        mapMethod    | Type of mapping                  | no | planar
        offset       | Offset to mapped values          | no | Zero
    \endtable

See also
    Foam::timeVaryingMappedFixedValueFvPatchField

SourceFiles
    timeVaryingMappedFixedValuePointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_timeVaryingMappedFixedValuePointPatchField_defines_typeName in .C files that
//  define the template statics of timeVaryingMappedFixedValuePointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_timeVaryingMappedFixedValuePointPatchField_defines_typeName)
#  define Foam_timeVaryingMappedFixedValuePointPatchField_typeName_API
#else
#  define Foam_timeVaryingMappedFixedValuePointPatchField_typeName_API fvMotionSolvers_API
#endif

#ifndef Foam_timeVaryingMappedFixedValuePointPatchField_H
#define Foam_timeVaryingMappedFixedValuePointPatchField_H

#include "fixedValuePointPatchField.H"
#include "Function1.H"
#include "Pair.H"
#include "instantList.H"
#include "pointToPointPlanarInterpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
        Class timeVaryingMappedFixedValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class timeVaryingMappedFixedValuePointPatchField
:
    public fixedValuePointPatchField<Type>
{
    typedef timeVaryingMappedFixedValuePointPatchField<Type> this_bctype;
    typedef fixedValuePointPatchField<Type> parent_bctype;

    // Private Data

        //- If true adjust the mapped field to maintain average value
        bool setAverage_;

        //- Fraction of perturbation (fraction of bounding box) to add
        scalar perturb_;

        //- Name of the field data table, defaults to the name of the field
        word fieldTableName_;

        //- Name of points file (default: "points")
        word pointsName_;

        //- Interpolation scheme to use (default is empty == "planar")
        word mapMethod_;

        //- 2D interpolation (for 'planar' mapMethod)
        autoPtr<pointToPointPlanarInterpolation> mapperPtr_;

        //- List of boundaryData time directories
        instantList sampleTimes_;

        //- The first/second sample indices in sampleTimes
        labelPair sampleIndex_;

        //- The average values at first/second sampleIndex_ (if setAverage)
        Pair<Type> sampleAverage_;

        //- The interpolated values at first/second sampleIndex_
        Pair<Field<Type>> sampleValues_;

        //- Time varying offset values to interpolated data
        autoPtr<Function1<Type>> offset_;


    // Private Member Functions

        //- Find boundary data between time 't' and interpolate
        void checkTable(const scalar t);

        //- Update field and average value using interpolation at
        //- given instant index
        void updateSampledValues
        (
            const label sampleIndex,
            Field<Type>& field,
            Type& avg
        ) const;


public:

    //- Runtime type information
    TypeNameApi(Foam_timeVaryingMappedFixedValuePointPatchField_typeName_API, "timeVaryingMappedFixedValue");


    // Constructors

        //- Construct from patch and internal field
        timeVaryingMappedFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        timeVaryingMappedFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        timeVaryingMappedFixedValuePointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        timeVaryingMappedFixedValuePointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        timeVaryingMappedFixedValuePointPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const pointPatchFieldMapper&
            );

            //- Reverse map the given PointPatchField onto
            //  this PointPatchField
            virtual void rmap
            (
                const pointPatchField<Type>&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "timeVaryingMappedFixedValuePointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
