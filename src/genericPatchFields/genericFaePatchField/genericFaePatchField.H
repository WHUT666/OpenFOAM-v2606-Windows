/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2023-2026 OpenCFD Ltd.
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
    Foam::genericFaePatchField

Description
    This boundary condition provides a generic version of the \c calculated
    condition, useful as a fallback for handling unknown patch types.  Not
    generally applicable as a user-specified condition.

See also
    Foam::calculatedFaePatchField

SourceFiles
    genericFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_genericFaePatchField_defines_typeName in .C files that
//  define the template statics of genericFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_genericFaePatchField_defines_typeName)
#  define Foam_genericFaePatchField_typeName_API
#else
#  define Foam_genericFaePatchField_typeName_API genericPatchFields_API
#endif

#ifndef Foam_genericFaePatchField_H
#define Foam_genericFaePatchField_H

#include "calculatedFaePatchField.H"
#include "genericPatchFieldBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class genericFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class genericFaePatchField
:
    public genericPatchFieldBase,
    public calculatedFaePatchField<Type>
{
    typedef genericFaePatchField<Type> this_bctype;
    typedef calculatedFaePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_genericFaePatchField_typeName_API, "generic");


    // Constructors

        //- Construct from patch and internal field
        genericFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary
        genericFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        genericFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Copy construct setting internal field reference
        genericFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        genericFaePatchField(const this_bctype& pfld)
        :
            this_bctype(pfld, pfld.internalField())
        {}

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Write
        virtual void write(Ostream&) const;


    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const faPatchFieldMapper&);

        //- Reverse map the given faePatchField onto this faePatchField
        virtual void rmap(const faePatchField<Type>&, const labelList&);


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "genericFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
