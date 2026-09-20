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
    Foam::fixedInternalValueFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides a mechanism to set boundary (cell) values
    directly into a matrix, i.e. to set a constraint condition.  Default
    behaviour is to act as a zero gradient condition.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedInternalValue;
        value           uniform 0;              // place holder
    }
    \endverbatim

Note
    This is used as a base for conditions such as the turbulence \c epsilon
    wall function, which applies a near-wall constraint for high Reynolds
    number flows.

See also
    Foam::epsilonWallFunctionFvPatchScalarField

SourceFiles
    fixedInternalValueFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedInternalValueFvPatchField_defines_typeName in .C files that
//  define the template statics of fixedInternalValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedInternalValueFvPatchField_defines_typeName)
#  define Foam_fixedInternalValueFvPatchField_typeName_API
#else
#  define Foam_fixedInternalValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_fixedInternalValueFvPatchField_H
#define Foam_fixedInternalValueFvPatchField_H

#include "zeroGradientFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class fixedInternalValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedInternalValueFvPatchField
:
    public zeroGradientFvPatchField<Type>
{
    typedef fixedInternalValueFvPatchField<Type> this_bctype;
    typedef zeroGradientFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_fixedInternalValueFvPatchField_typeName_API, "fixedInternalValue");


    // Constructors

        //- Construct from patch and internal field
        fixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        fixedInternalValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        fixedInternalValueFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        fixedInternalValueFvPatchField(const this_bctype& pfld)
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

        //- Manipulate a matrix
        virtual void manipulateMatrix(fvMatrix<Type>& matrix);


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fixedInternalValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
