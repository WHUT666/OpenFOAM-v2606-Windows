/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2019-2023 OpenCFD Ltd.
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
    Foam::fixedValueFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    fixedValueFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedValueFaePatchField_defines_typeName in .C files that
//  define the template statics of fixedValueFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedValueFaePatchField_defines_typeName)
#  define Foam_fixedValueFaePatchField_typeName_API
#else
#  define Foam_fixedValueFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_fixedValueFaePatchField_H
#define Foam_fixedValueFaePatchField_H

#include "faePatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class fixedValueFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedValueFaePatchField
:
    public faePatchField<Type>
{
    typedef fixedValueFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_fixedValueFaePatchField_typeName_API, "fixedValue");


    // Constructors

        //- Construct from patch and internal field
        fixedValueFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and value
        fixedValueFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const Type& value
        );

        //- Construct from patch, internal field and dictionary
        fixedValueFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary& dict,
            //! The "value" entry (default: mandatory)
            IOobjectOption::readOption requireValue = IOobjectOption::MUST_READ
        );

        //- Construct by mapping onto a new patch
        fixedValueFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Copy construct onto new patch with specified value
        fixedValueFaePatchField
        (
            const fixedValueFaePatchField<Type>& pfld,
            const faPatch& p,
            const DimensionedField<Type, edgeMesh>& iF,
            const Type& value
        );

        //- Construct as copy setting internal field reference
        fixedValueFaePatchField
        (
            const fixedValueFaePatchField<Type>& pfld,
            const DimensionedField<Type, edgeMesh>& iF
        );

        //- No copy without an internal field
        fixedValueFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~fixedValueFaePatchField() = default;


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }


    // Other

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const faePatchField<Type>&) {}
        virtual void operator+=(const faePatchField<Type>&) {}
        virtual void operator-=(const faePatchField<Type>&) {}
        virtual void operator*=(const faePatchField<scalar>&) {}
        virtual void operator/=(const faePatchField<scalar>&) {}

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
    #include "fixedValueFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
