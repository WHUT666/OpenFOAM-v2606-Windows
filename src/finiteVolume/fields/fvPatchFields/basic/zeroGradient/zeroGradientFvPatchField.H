/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2025 OpenCFD Ltd.
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
    Foam::zeroGradientFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition applies a zero-gradient condition from the patch
    internal field onto the patch faces.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            zeroGradient;
    }
    \endverbatim

SourceFiles
    zeroGradientFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_zeroGradientFvPatchField_defines_typeName in .C files that
//  define the template statics of zeroGradientFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_zeroGradientFvPatchField_defines_typeName)
#  define Foam_zeroGradientFvPatchField_typeName_API
#else
#  define Foam_zeroGradientFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_zeroGradientFvPatchField_H
#define Foam_zeroGradientFvPatchField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class zeroGradientFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class zeroGradientFvPatchField
:
    public fvPatchField<Type>
{
    typedef zeroGradientFvPatchField<Type> this_bctype;
    typedef fvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_zeroGradientFvPatchField_typeName_API, "zeroGradient");  // fieldTypes::zeroGradientTypeName_()


    // Constructors

        //- Construct from patch and internal field
        zeroGradientFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        zeroGradientFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        zeroGradientFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        zeroGradientFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        zeroGradientFvPatchField(const this_bctype& pfld)
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

        // Evaluation Functions

            //- Return patch-normal gradient
            virtual tmp<Field<Type>> snGrad() const
            {
                // zero-gradient
                return tmp<Field<Type>>::New(this->size(), Foam::zero{});
            }

            //- Retrieve patch-normal gradient [contiguous storage]
            virtual void snGrad(UList<Type>& result) const
            {
                // zero-gradient
                result = Foam::zero{};
            }

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            );

            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "zeroGradientFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
