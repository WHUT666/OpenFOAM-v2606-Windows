/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::timeVaryingUniformFixedValuePointPatchField

Description
    A time-varying form of a uniform fixed value boundary condition.

See also
    Foam::timeVaryingUniformFixedValueFvField

SourceFiles
    timeVaryingUniformFixedValuePointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_timeVaryingUniformFixedValuePointPatchField_defines_typeName in .C files that
//  define the template statics of timeVaryingUniformFixedValuePointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_timeVaryingUniformFixedValuePointPatchField_defines_typeName)
#  define Foam_timeVaryingUniformFixedValuePointPatchField_typeName_API
#else
#  define Foam_timeVaryingUniformFixedValuePointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_timeVaryingUniformFixedValuePointPatchField_H
#define Foam_timeVaryingUniformFixedValuePointPatchField_H

#include "fixedValuePointPatchField.H"
#include "interpolationTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
        Class timeVaryingUniformFixedValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class timeVaryingUniformFixedValuePointPatchField
:
    public fixedValuePointPatchField<Type>
{
    typedef timeVaryingUniformFixedValuePointPatchField<Type> this_bctype;
    typedef fixedValuePointPatchField<Type> parent_bctype;

    // Private Data

        //- The time series being used, including the bounding treatment
        interpolationTable<Type> timeSeries_;

public:

    //- Runtime type information
    TypeNameApi(Foam_timeVaryingUniformFixedValuePointPatchField_typeName_API, "timeVaryingUniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        timeVaryingUniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        timeVaryingUniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        timeVaryingUniformFixedValuePointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        timeVaryingUniformFixedValuePointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        timeVaryingUniformFixedValuePointPatchField
        (
            const this_bctype&
        ) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        // Access

            //- Return the time series used
            const interpolationTable<Type>& timeSeries() const
            {
                return timeSeries_;
            }


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
    #include "timeVaryingUniformFixedValuePointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
