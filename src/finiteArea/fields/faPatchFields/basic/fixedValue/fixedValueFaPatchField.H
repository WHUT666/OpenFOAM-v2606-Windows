/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
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
    Foam::fixedValueFaPatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    fixedValueFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedValueFaPatchField_defines_typeName in .C files that
//  define the template statics of fixedValueFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedValueFaPatchField_defines_typeName)
#  define Foam_fixedValueFaPatchField_typeName_API
#else
#  define Foam_fixedValueFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_fixedValueFaPatchField_H
#define Foam_fixedValueFaPatchField_H

#include "faPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class fixedValueFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedValueFaPatchField
:
    public faPatchField<Type>
{
    typedef fixedValueFaPatchField<Type> this_bctype;
    typedef faPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_fixedValueFaPatchField_typeName_API, "fixedValue");


    // Constructors

        //- Construct from patch and internal field
        fixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and value
        fixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const Type& value
        );

        //- Construct from patch, internal field and dictionary
        fixedValueFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&,
            //! The "value" entry (default: mandatory)
            IOobjectOption::readOption requireValue = IOobjectOption::MUST_READ
        );

        //- Construct by mapping onto a new patch
        fixedValueFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Copy construct onto a new patch with internal field reference
        //- and specified value
        fixedValueFaPatchField
        (
            const fixedValueFaPatchField<Type>& pfld,
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF,
            const Type& value
        );

        //- Copy construct with internal field reference
        fixedValueFaPatchField
        (
            const fixedValueFaPatchField<Type>& pfld,
            const DimensionedField<Type, areaMesh>& iF
        );

        //- No copy without an internal field reference
        fixedValueFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }


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


        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const faPatchField<Type>&) {}
        virtual void operator+=(const faPatchField<Type>&) {}
        virtual void operator-=(const faPatchField<Type>&) {}
        virtual void operator*=(const faPatchField<scalar>&) {}
        virtual void operator/=(const faPatchField<scalar>&) {}

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
    #include "fixedValueFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
