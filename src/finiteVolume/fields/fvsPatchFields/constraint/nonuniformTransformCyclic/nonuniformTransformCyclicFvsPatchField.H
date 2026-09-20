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
    Foam::nonuniformTransformCyclicFvsPatchField

Description
    Foam::nonuniformTransformCyclicFvsPatchField

\*---------------------------------------------------------------------------*/

//- Define Foam_nonuniformTransformCyclicFvsPatchField_defines_typeName in .C files that
//  define the template statics of nonuniformTransformCyclicFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_nonuniformTransformCyclicFvsPatchField_defines_typeName)
#  define Foam_nonuniformTransformCyclicFvsPatchField_typeName_API
#else
#  define Foam_nonuniformTransformCyclicFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_nonuniformTransformCyclicFvsPatchField_H
#define Foam_nonuniformTransformCyclicFvsPatchField_H

#include "cyclicFvsPatchField.H"
#include "nonuniformTransformCyclicFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class nonuniformTransformCyclicFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class nonuniformTransformCyclicFvsPatchField
:
    public cyclicFvsPatchField<Type>
{
    typedef nonuniformTransformCyclicFvsPatchField<Type> this_bctype;
    typedef cyclicFvsPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_nonuniformTransformCyclicFvsPatchField_typeName_API, nonuniformTransformCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        nonuniformTransformCyclicFvsPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, surfaceMesh>& iF
        )
        :
            parent_bctype(p, iF)
        {}

        //- Construct from patch, internal field and dictionary
        nonuniformTransformCyclicFvsPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, surfaceMesh>& iF,
            const dictionary& dict
        )
        :
            parent_bctype(p, iF, dict)
        {}

        //- Construct by mapping onto a new patch
        nonuniformTransformCyclicFvsPatchField
        (
            const this_bctype& ptf,
            const fvPatch& p,
            const DimensionedField<Type, surfaceMesh>& iF,
            const fvPatchFieldMapper& mapper
        )
        :
            parent_bctype(ptf, p, iF, mapper)
        {}

        //- Construct as copy setting internal field reference
        nonuniformTransformCyclicFvsPatchField
        (
            const this_bctype& ptf,
            const DimensionedField<Type, surfaceMesh>& iF
        )
        :
            parent_bctype(ptf, iF)
        {}

        //- No copy without an internal field
        nonuniformTransformCyclicFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
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
