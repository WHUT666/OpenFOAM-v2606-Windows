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
    Foam::calculatedFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    calculatedFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_calculatedFaePatchField_defines_typeName in .C files that
//  define the template statics of calculatedFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_calculatedFaePatchField_defines_typeName)
#  define Foam_calculatedFaePatchField_typeName_API
#else
#  define Foam_calculatedFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_calculatedFaePatchField_H
#define Foam_calculatedFaePatchField_H

#include "faePatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class calculatedFaePatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class calculatedFaePatchField
:
    public faePatchField<Type>
{
    typedef calculatedFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_calculatedFaePatchField_typeName_API, "calculated");  // fieldTypes::calculatedTypeName_()


    // Constructors

        //- Construct from patch and internal field
        calculatedFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary
        calculatedFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary& dict,
            //! The "value" entry (default: mandatory)
            IOobjectOption::readOption requireValue = IOobjectOption::MUST_READ
        );

        //- Construct by mapping onto a new patch
        calculatedFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        calculatedFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        calculatedFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~calculatedFaePatchField() = default;


    // Member Functions

        //- True: the patch field fixes a value.
        virtual bool fixesValue() const { return true; }


    // Other

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
    #include "calculatedFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
