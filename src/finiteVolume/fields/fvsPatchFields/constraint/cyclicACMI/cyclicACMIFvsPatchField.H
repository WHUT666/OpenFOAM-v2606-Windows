/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
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
    Foam::cyclicACMIFvsPatchField

Description
    This boundary condition enforces a cyclic condition between a pair of
    boundaries, whereby communication between the patches is performed using
    an arbitrarily coupled mesh interface (ACMI) interpolation.

SourceFiles
    cyclicACMIFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicACMIFvsPatchField_defines_typeName in .C files that
//  define the template statics of cyclicACMIFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicACMIFvsPatchField_defines_typeName)
#  define Foam_cyclicACMIFvsPatchField_typeName_API
#else
#  define Foam_cyclicACMIFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_cyclicACMIFvsPatchField_H
#define Foam_cyclicACMIFvsPatchField_H

#include "coupledFvsPatchField.H"
#include "cyclicACMIFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class cyclicACMIFvsPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicACMIFvsPatchField
:
    public coupledFvsPatchField<Type>
{
    typedef cyclicACMIFvsPatchField<Type> this_bctype;
    typedef coupledFvsPatchField<Type> parent_bctype;

    // Private Data

        //- Local reference cast into the cyclic patch
        const cyclicACMIFvPatch& cyclicACMIPatch_;


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicACMIFvsPatchField_typeName_API, cyclicACMIFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicACMIFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is MUST_READ.
        cyclicACMIFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        cyclicACMIFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicACMIFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        cyclicACMIFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Return true if running parallel
        virtual bool coupled() const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cyclicACMIFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
