/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016-2026 OpenCFD Ltd.
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
    Foam::genericPointPatchField

Description
    A generic version of calculatedPointPatchField, useful as a fallback for
    handling unknown patch types.

SourceFiles
    genericPointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_genericPointPatchField_defines_typeName in .C files that
//  define the template statics of genericPointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_genericPointPatchField_defines_typeName)
#  define Foam_genericPointPatchField_typeName_API
#else
#  define Foam_genericPointPatchField_typeName_API genericPatchFields_API
#endif

#ifndef Foam_genericPointPatchField_H
#define Foam_genericPointPatchField_H

#include "calculatedPointPatchField.H"
#include "genericPatchFieldBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class genericPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class genericPointPatchField
:
    public genericPatchFieldBase,
    public calculatedPointPatchField<Type>
{
    typedef genericPointPatchField<Type> this_bctype;
    typedef calculatedPointPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_genericPointPatchField_typeName_API, "generic");


    // Constructors

        //- Construct from patch and internal field
        genericPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        genericPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        genericPointPatchField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Copy construct setting internal field reference
        genericPointPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, pointMesh>&
        );

        //- No copy without an internal field
        genericPointPatchField(const this_bctype& pfld)
        :
            this_bctype(pfld, pfld.internalField())
        {}

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Write
        virtual void write(Ostream&) const;


    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const pointPatchFieldMapper&);

        //- Reverse map the given pointPatchField onto this pointPatchField
        virtual void rmap(const pointPatchField<Type>&, const labelList&);


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "genericPointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
