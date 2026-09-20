/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2023-2025 OpenCFD Ltd.
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
    Foam::fixedValueFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition supplies a fixed value constraint, and is the base
    class for a number of other boundary conditions.

Usage
    \table
        Property     | Description             | Required    | Default value
        value        | Patch face values       | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedValue;
        value           uniform 0;  // Example for scalar field usage
    }
    \endverbatim

SourceFiles
    fixedValueFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedValueFvPatchField_defines_typeName in .C files that
//  define the template statics of fixedValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedValueFvPatchField_defines_typeName)
#  define Foam_fixedValueFvPatchField_typeName_API
#else
#  define Foam_fixedValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_fixedValueFvPatchField_H
#define Foam_fixedValueFvPatchField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class fixedValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedValueFvPatchField
:
    public fvPatchField<Type>
{
    typedef fixedValueFvPatchField<Type> this_bctype;
    typedef fvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_fixedValueFvPatchField_typeName_API, "fixedValue");


    // Constructors

        //- Construct from patch and internal field
        fixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and value
        fixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Type& value
        );

        //- Construct from patch, internal field and dictionary
        fixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            IOobjectOption::readOption requireValue = IOobjectOption::MUST_READ
        );

        //- Compatibility (prefer with readOption)
        fixedValueFvPatchField
        (
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF,
            const dictionary& dict,
            const bool needValue
        )
        :
            fixedValueFvPatchField
            (
                p, iF, dict,
                (needValue? IOobjectOption::MUST_READ : IOobjectOption::NO_READ)
            )
        {}

        //- Construct by mapping onto a new patch
        fixedValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct onto a new patch with internal field reference
        //- and specified value
        fixedValueFvPatchField
        (
            const this_bctype& pfld,
            const fvPatch& p,
            const DimensionedField<Type, volMesh>& iF,
            const Type& value
        );

        //- Copy construct with internal field reference
        fixedValueFvPatchField
        (
            const fixedValueFvPatchField<Type>& pfld,
            const DimensionedField<Type, volMesh>& iF
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        fixedValueFvPatchField(const this_bctype& pfld)
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

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- False: this patch field is not altered by assignment.
        virtual bool assignable() const { return false; }

        //- Write includes "value" entry
        virtual void write(Ostream&) const;


        // Evaluation functions

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;


    // Member Operators

        // Disable assignment operators
        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const fvPatchField<Type>&) {}
        virtual void operator+=(const fvPatchField<Type>&) {}
        virtual void operator-=(const fvPatchField<Type>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<Type>&) {}
        virtual void operator-=(const Field<Type>&) {}

        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const Type&) {}
        virtual void operator+=(const Type&) {}
        virtual void operator-=(const Type&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fixedValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
