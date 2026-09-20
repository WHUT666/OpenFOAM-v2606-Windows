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
    Foam::cyclicFvsPatchField

Description
    This boundary condition enforces a cyclic condition between a pair of
    boundaries.

    The "value" entry is NO_READ, but is written.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            cyclic;
    }
    \endverbatim

SourceFiles
    cyclicFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicFvsPatchField_defines_typeName in .C files that
//  define the template statics of cyclicFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicFvsPatchField_defines_typeName)
#  define Foam_cyclicFvsPatchField_typeName_API
#else
#  define Foam_cyclicFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_cyclicFvsPatchField_H
#define Foam_cyclicFvsPatchField_H

#include "coupledFvsPatchField.H"
#include "cyclicFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class cyclicFvsPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicFvsPatchField
:
    public coupledFvsPatchField<Type>
{
    typedef cyclicFvsPatchField<Type> this_bctype;
    typedef coupledFvsPatchField<Type> parent_bctype;

    // Private Data

        //- Local reference cast into the cyclic patch
        const cyclicFvPatch& cyclicPatch_;


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicFvsPatchField_typeName_API, cyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is MUST_READ.
        cyclicFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        cyclicFvsPatchField(const this_bctype&) = delete;

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

#ifdef NoRepository
    #include "cyclicFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
