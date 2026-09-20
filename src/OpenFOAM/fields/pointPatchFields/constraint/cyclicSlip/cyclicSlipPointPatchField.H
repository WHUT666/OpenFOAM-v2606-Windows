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
    Foam::cyclicSlipPointPatchField

Description
    Cyclic + slip constraints

SourceFiles
    cyclicSlipPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicSlipPointPatchField_defines_typeName in .C files that
//  define the template statics of cyclicSlipPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicSlipPointPatchField_defines_typeName)
#  define Foam_cyclicSlipPointPatchField_typeName_API
#else
#  define Foam_cyclicSlipPointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_cyclicSlipPointPatchField_H
#define Foam_cyclicSlipPointPatchField_H

#include "cyclicPointPatchField.H"
#include "cyclicSlipPointPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class cyclicSlipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicSlipPointPatchField
:
    public cyclicPointPatchField<Type>
{
    typedef cyclicSlipPointPatchField<Type> this_bctype;
    typedef cyclicPointPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicSlipPointPatchField_typeName_API, cyclicSlipPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicSlipPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicSlipPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        cyclicSlipPointPatchField(const this_bctype& pfld)
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

        //- Evaluate the patch field
        virtual void evaluate
        (
            const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
        );


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cyclicSlipPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
