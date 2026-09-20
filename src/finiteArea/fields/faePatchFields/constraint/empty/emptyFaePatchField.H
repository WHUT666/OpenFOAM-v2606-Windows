/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2019 OpenCFD Ltd.
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
    Foam::emptyFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    emptyFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_emptyFaePatchField_defines_typeName in .C files that
//  define the template statics of emptyFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_emptyFaePatchField_defines_typeName)
#  define Foam_emptyFaePatchField_typeName_API
#else
#  define Foam_emptyFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_emptyFaePatchField_H
#define Foam_emptyFaePatchField_H

#include "faePatchField.H"
#include "emptyFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class emptyFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class emptyFaePatchField
:
    public faePatchField<Type>
{
    typedef emptyFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_emptyFaePatchField_typeName_API, emptyFaPatch::typeName_());  // fieldTypes::emptyTypeName_()


    // Constructors

        //- Construct from patch and internal field
        emptyFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        emptyFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        emptyFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        emptyFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        emptyFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~emptyFaePatchField() = default;


    // Member Functions

    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const faPatchFieldMapper&) {}

        //- Reverse map the given fvsPatchField onto this fvsPatchField
        virtual void rmap(const faePatchField<Type>&, const labelList&) {}


    // Member Functions

        //- Write without "value" entry!
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment (TBD: could also be ignore assignment)
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "emptyFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
