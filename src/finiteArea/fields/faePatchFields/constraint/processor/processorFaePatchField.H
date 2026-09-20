/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
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
    Foam::processorFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    processorFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_processorFaePatchField_defines_typeName in .C files that
//  define the template statics of processorFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_processorFaePatchField_defines_typeName)
#  define Foam_processorFaePatchField_typeName_API
#else
#  define Foam_processorFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_processorFaePatchField_H
#define Foam_processorFaePatchField_H

#include "coupledFaePatchField.H"
#include "processorFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class processorFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorFaePatchField
:
    public coupledFaePatchField<Type>
{
    typedef processorFaePatchField<Type> this_bctype;
    typedef coupledFaePatchField<Type> parent_bctype;

    // Private Data

        //- Local reference cast into the processor patch
        const processorFaPatch& procPatch_;


public:

    //- Runtime type information
    TypeNameApi(Foam_processorFaePatchField_typeName_API, processorFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch and internal field and patch field
        processorFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        processorFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        processorFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        processorFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        processorFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~processorFaePatchField() = default;


    // Member Functions

        //- Return true if running parallel
        virtual bool coupled() const { return UPstream::parRun(); }


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "processorFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
