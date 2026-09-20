/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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
    Foam::zeroValueFaPatchField

Description
    Specifies a zero fixed value boundary condition.

    Example of the boundary condition specification:
    \verbatim
    inlet
    {
        type    zeroValue;
    }
    \endverbatim

SourceFiles
    zeroValueFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_zeroValueFaPatchField_defines_typeName in .C files that
//  define the template statics of zeroValueFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_zeroValueFaPatchField_defines_typeName)
#  define Foam_zeroValueFaPatchField_typeName_API
#else
#  define Foam_zeroValueFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_zeroValueFaPatchField_H
#define Foam_zeroValueFaPatchField_H

#include "fixedValueFaPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class zeroValueFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class zeroValueFaPatchField
:
    public fixedValueFaPatchField<Type>
{
    typedef zeroValueFaPatchField<Type> this_bctype;
    typedef fixedValueFaPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_zeroValueFaPatchField_typeName_API, "zeroValue");


    // Constructors

        //- Construct from patch and internal field
        zeroValueFaPatchField
        (
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF
        );

        //- Construct from patch, internal field and dictionary
        zeroValueFaPatchField
        (
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF,
            const dictionary& dict
        );

        //- Copy construct onto a new patch (no mapping needed)
        zeroValueFaPatchField
        (
            const zeroValueFaPatchField<Type>& pfld,
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF,
            const faPatchFieldMapper&
        );

        //- Copy construct with internal field reference
        zeroValueFaPatchField
        (
            const zeroValueFaPatchField<Type>& pfld,
            const DimensionedField<Type, areaMesh>& iF
        );

        //- No copy without an internal field
        zeroValueFaPatchField(const this_bctype&) = delete;

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

        //- False: this patch field is not altered by assignment.
        virtual bool assignable() const { return false; }

        //- Write (without "value" entry)
        virtual void write(Ostream& os) const;


    // Evaluation Functions

        //- Return the matrix diagonal coefficients corresponding to the
        //-  evaluation of the value of this patchField with given weights
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

        // Disable assignment operators
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


        // Disable forced assignment operators
        virtual void operator==(const faPatchField<Type>&) {}
        virtual void operator==(const Field<Type>&) {}
        virtual void operator==(const Type&) {}

        // Prevent automatic comparison rewriting (c++20)
        bool operator!=(const faPatchField<Type>&) const = delete;
        bool operator!=(const Field<Type>&) const = delete;
        bool operator!=(const Type&) const = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "zeroValueFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
