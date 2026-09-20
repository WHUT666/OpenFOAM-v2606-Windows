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
    Foam::nonuniformTransformCyclicFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition enforces a cyclic condition between a pair of
    boundaries, incorporating a non-uniform transformation.

    The "value" entry is NO_READ.

SourceFiles
    nonuniformTransformCyclicFvPatchFields.H
    nonuniformTransformCyclicFvPatchFields.cxx
    nonuniformTransformCyclicFvPatchFieldsFwd.H

\*---------------------------------------------------------------------------*/

//- Define Foam_nonuniformTransformCyclicFvPatchField_defines_typeName in .C files that
//  define the template statics of nonuniformTransformCyclicFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_nonuniformTransformCyclicFvPatchField_defines_typeName)
#  define Foam_nonuniformTransformCyclicFvPatchField_typeName_API
#else
#  define Foam_nonuniformTransformCyclicFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_nonuniformTransformCyclicFvPatchField_H
#define Foam_nonuniformTransformCyclicFvPatchField_H

#include "cyclicFvPatchField.H"
#include "nonuniformTransformCyclicFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class nonuniformTransformCyclicFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class nonuniformTransformCyclicFvPatchField
:
    public cyclicFvPatchField<Type>
{
    typedef nonuniformTransformCyclicFvPatchField<Type> this_bctype;
    typedef cyclicFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_nonuniformTransformCyclicFvPatchField_typeName_API, nonuniformTransformCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        nonuniformTransformCyclicFvPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF
        )
        :
            parent_bctype(p, iF)
        {}

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        nonuniformTransformCyclicFvPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF,
            const dictionary& dict
        )
        :
            parent_bctype(p, iF, dict)  // needValue (evaluate) = true
        {}

        //- Construct by mapping onto a new patch
        nonuniformTransformCyclicFvPatchField
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
        nonuniformTransformCyclicFvPatchField
        (
            const this_bctype& ptf,
            const DimensionedField<Type, volMesh>& iF
        )
        :
            parent_bctype(ptf, iF)
        {}

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        nonuniformTransformCyclicFvPatchField(const this_bctype& pfld)
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


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
