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
    Foam::slipPointPatchField

Description
    Foam::slipPointPatchField

SourceFiles
    slipPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_slipPointPatchField_defines_typeName in .C files that
//  define the template statics of slipPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_slipPointPatchField_defines_typeName)
#  define Foam_slipPointPatchField_typeName_API
#else
#  define Foam_slipPointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_slipPointPatchField_H
#define Foam_slipPointPatchField_H

#include "basicSymmetryPointPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class slipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slipPointPatchField
:
    public basicSymmetryPointPatchField<Type>
{
    typedef slipPointPatchField<Type> this_bctype;
    typedef basicSymmetryPointPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_slipPointPatchField_typeName_API, "slip");


    // Constructors

        //- Construct from patch and internal field
        slipPointPatchField
        (
            const pointPatch& p,
            const DimensionedField<Type, pointMesh>& iF
        )
        :
            parent_bctype(p, iF)
        {}

        //- Construct from patch, internal field and dictionary
        slipPointPatchField
        (
            const pointPatch& p,
            const DimensionedField<Type, pointMesh>& iF,
            const dictionary& dict
        )
        :
            parent_bctype(p, iF, dict)
        {}

        //- Construct by mapping onto a new patch
        slipPointPatchField
        (
            const this_bctype& pfld,
            const pointPatch& p,
            const DimensionedField<Type, pointMesh>& iF,
            const pointPatchFieldMapper& mapper
        )
        :
            parent_bctype(pfld, p, iF, mapper)
        {}

        //- Construct as copy setting internal field reference
        slipPointPatchField
        (
            const this_bctype& pfld,
            const DimensionedField<Type, pointMesh>& iF
        )
        :
            parent_bctype(pfld, iF)
        {}

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        slipPointPatchField(const this_bctype& pfld)
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


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
