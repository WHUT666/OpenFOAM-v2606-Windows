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
    Foam::coupledFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    coupledFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_coupledFaePatchField_defines_typeName in .C files that
//  define the template statics of coupledFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_coupledFaePatchField_defines_typeName)
#  define Foam_coupledFaePatchField_typeName_API
#else
#  define Foam_coupledFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_coupledFaePatchField_H
#define Foam_coupledFaePatchField_H

#include "faePatchField.H"
#include "coupledFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class coupledFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class coupledFaePatchField
:
    public faePatchField<Type>
{
    typedef coupledFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_coupledFaePatchField_typeName_API, coupledFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        coupledFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and patch field
        coupledFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        coupledFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary& dict,
            //! The "value" entry (default: optional)
            IOobjectOption::readOption requireValue = IOobjectOption::LAZY_READ
        );

        //- Construct by mapping onto a new patch
        coupledFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        coupledFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        coupledFaePatchField(const this_bctype&) = delete;

        //- Construct and return a clone
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>&
        ) const = 0;


    //- Destructor
    virtual ~coupledFaePatchField() = default;


    // Member Functions

        //- True if derived from coupledFaePatchField
        virtual bool coupled() const { return true; }

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "coupledFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
