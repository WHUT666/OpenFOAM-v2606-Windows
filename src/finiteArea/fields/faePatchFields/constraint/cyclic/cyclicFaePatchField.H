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
    Foam::cyclicFaePatchField

Description
    Foam::cyclicFaePatchField

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    cyclicFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_cyclicFaePatchField_defines_typeName in .C files that
//  define the template statics of cyclicFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_cyclicFaePatchField_defines_typeName)
#  define Foam_cyclicFaePatchField_typeName_API
#else
#  define Foam_cyclicFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_cyclicFaePatchField_H
#define Foam_cyclicFaePatchField_H

#include "coupledFaePatchField.H"
#include "cyclicFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class cyclicFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicFaePatchField
:
    public coupledFaePatchField<Type>
{
    typedef cyclicFaePatchField<Type> this_bctype;
    typedef coupledFaePatchField<Type> parent_bctype;

    // Private Data

        //- Local reference cast into the cyclic patch
        const cyclicFaPatch& cyclicPatch_;


public:

    //- Runtime type information
    TypeNameApi(Foam_cyclicFaePatchField_typeName_API, cyclicFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary& dict
        );

        //- Construct by mapping onto a new patch
        cyclicFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        cyclicFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        cyclicFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~cyclicFaePatchField() = default;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "cyclicFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
