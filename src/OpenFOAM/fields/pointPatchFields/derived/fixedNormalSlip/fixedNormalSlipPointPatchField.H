/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::fixedNormalSlipPointPatchField

Description
    slip with user-specified normal

SourceFiles
    fixedNormalSlipPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedNormalSlipPointPatchField_defines_typeName in .C files that
//  define the template statics of fixedNormalSlipPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedNormalSlipPointPatchField_defines_typeName)
#  define Foam_fixedNormalSlipPointPatchField_typeName_API
#else
#  define Foam_fixedNormalSlipPointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_fixedNormalSlipPointPatchField_H
#define Foam_fixedNormalSlipPointPatchField_H

#include "slipPointPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class fixedNormalSlipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedNormalSlipPointPatchField
:
    public slipPointPatchField<Type>
{
    typedef fixedNormalSlipPointPatchField<Type> this_bctype;
    typedef slipPointPatchField<Type> parent_bctype;

    // Private Data

        //- User specified normal
        vector n_;


public:

    //- Runtime type information
    TypeNameApi(Foam_fixedNormalSlipPointPatchField_typeName_API, "fixedNormalSlip");


    // Constructors

        //- Construct from patch and internal field
        fixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        fixedNormalSlipPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        fixedNormalSlipPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        fixedNormalSlipPointPatchField(const this_bctype& pfld)
        #ifdef Foam_pointPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Update the patch field
        virtual void evaluate
        (
            const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
        );

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
    #include "fixedNormalSlipPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
