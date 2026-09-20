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
    Foam::cyclicSlipFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition is a light wrapper around the cyclicFvPatchField
    condition, providing no new functionality.

    The "value" entry is NO_READ.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            cyclicSlip;
    }
    \endverbatim

See also
    Foam::cyclicFvPatchField

SourceFiles
    cyclicSlipFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicSlipFvPatchField_defines_typeName in .C files that
//  define the template statics of cyclicSlipFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicSlipFvPatchField_defines_typeName)
#  define Foam_cyclicSlipFvPatchField_typeName_API
#else
#  define Foam_cyclicSlipFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_cyclicSlipFvPatchField_H
#define Foam_cyclicSlipFvPatchField_H

#include "cyclicFvPatchField.H"
#include "cyclicSlipFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class cyclicSlipFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicSlipFvPatchField
:
    public cyclicFvPatchField<Type>
{
    typedef cyclicSlipFvPatchField<Type> this_bctype;
    typedef cyclicFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicSlipFvPatchField_typeName_API, cyclicSlipFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        cyclicSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicSlipFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicSlipFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        cyclicSlipFvPatchField(const this_bctype& pfld)
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

        //- Inherit assignment (TBD: could also be ignore assignment)
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cyclicSlipFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
