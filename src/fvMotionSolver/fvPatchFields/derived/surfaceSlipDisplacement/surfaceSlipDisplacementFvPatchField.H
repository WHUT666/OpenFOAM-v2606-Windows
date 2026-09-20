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
    Foam::surfaceSlipDisplacementFvPatchField

Description
    fvPatchField corresponding to surfaceSlipDisplacementPointPatchField.
    Is just a slip type since all hard work (projection) is done in the
    pointPatch field.

\*---------------------------------------------------------------------------*/

//- Define Foam_surfaceSlipDisplacementFvPatchField_defines_typeName in .C files that
//  define the template statics of surfaceSlipDisplacementFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_surfaceSlipDisplacementFvPatchField_defines_typeName)
#  define Foam_surfaceSlipDisplacementFvPatchField_typeName_API
#else
#  define Foam_surfaceSlipDisplacementFvPatchField_typeName_API fvMotionSolvers_API
#endif

#ifndef surfaceSlipDisplacementFvPatchField_H
#define surfaceSlipDisplacementFvPatchField_H

#include "slipFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class surfaceSlipDisplacementFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class surfaceSlipDisplacementFvPatchField
:
    public slipFvPatchField<Type>
{
    typedef surfaceSlipDisplacementFvPatchField<Type> this_bctype;
    typedef slipFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_surfaceSlipDisplacementFvPatchField_typeName_API, "surfaceSlipDisplacement");


    // Constructors

        //- Construct from patch and internal field
        surfaceSlipDisplacementFvPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF
        )
        :
            parent_bctype(p, iF)
        {}

        //- Construct from patch, internal field and dictionary
        surfaceSlipDisplacementFvPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF,
            const dictionary& dict
        )
        :
            parent_bctype(p, iF, dict)
        {}

        //- Construct by mapping onto a new patch
        surfaceSlipDisplacementFvPatchField
        (
            const this_bctype& ptf,
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF,
            const fvPatchFieldMapper& mapper
        )
        :
            parent_bctype(ptf, p, iF, mapper)
        {}

        //- Construct as copy setting internal field reference
        surfaceSlipDisplacementFvPatchField
        (
            const this_bctype& ptf,
            const DimensionedField<Type, volMesh>& iF
        )
        :
            parent_bctype(ptf, iF)
        {}

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        surfaceSlipDisplacementFvPatchField(const this_bctype& pfld)
        #ifdef Foam_fvPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        virtual void write(Ostream& os) const
        {
            slipFvPatchField<Type>::write(os);
            // Only so we can easily postprocess.
            fvPatchField<Type>::writeValueEntry(os);
        }


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
