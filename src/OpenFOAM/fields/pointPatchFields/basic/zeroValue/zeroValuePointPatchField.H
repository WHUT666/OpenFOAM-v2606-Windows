/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014 OpenFOAM Foundation
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
    Foam::zeroValuePointPatchField

Description
    Specifies a zero fixed value boundary condition

    Example of the boundary condition specification:
    \verbatim
    inlet
    {
        type    zeroValue;
    }
    \endverbatim

SourceFiles
    zeroValuePointPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_zeroValuePointPatchField_defines_typeName in .C files that
//  define the template statics of zeroValuePointPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_zeroValuePointPatchField_defines_typeName)
#  define Foam_zeroValuePointPatchField_typeName_API
#else
#  define Foam_zeroValuePointPatchField_typeName_API OpenFOAM_API
#endif

#ifndef Foam_zeroValuePointPatchField_H
#define Foam_zeroValuePointPatchField_H

#include "fixedValuePointPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class zeroValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class zeroValuePointPatchField
:
    public fixedValuePointPatchField<Type>
{
    typedef zeroValuePointPatchField<Type> this_bctype;
    typedef fixedValuePointPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_zeroValuePointPatchField_typeName_API, "zeroValue");


    // Constructors

        //- Construct from patch and internal field
        zeroValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        zeroValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Copy construct onto a new patch
        zeroValuePointPatchField
        (
            const this_bctype& pfld,
            const pointPatch& p,
            const DimensionedField<Type, pointMesh>& iF,
            const pointPatchFieldMapper&
        );

        //- Copy construct with internal field reference
        zeroValuePointPatchField
        (
            const this_bctype& pfld,
            const DimensionedField<Type, pointMesh>& iF
        );

        //- No copy without an internal field
        zeroValuePointPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<Type>> clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return pointPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- False: this patch field is not altered by assignment.
        virtual bool assignable() const { return false; }

        //- Write (without "value" entry)
        virtual void write(Ostream& os) const;


    // Member Operators

        // Disable assignment operators
        virtual void operator=(const valuePointPatchField<Type>&) {}
        virtual void operator=(const pointPatchField<Type>&) {}
        virtual void operator=(const Field<Type>&) {}
        virtual void operator=(const Type&) {}

        // Disable forced assignment operators
        virtual void operator==(const valuePointPatchField<Type>&) {}
        virtual void operator==(const pointPatchField<Type>&) {}
        virtual void operator==(const Field<Type>&) {}
        virtual void operator==(const Type&) {}

        // Prevent automatic comparison rewriting (c++20)
        bool operator!=(const valuePointPatchField<Type>&) const = delete;
        bool operator!=(const pointPatchField<Type>&) const = delete;
        bool operator!=(const Field<Type>&) const = delete;
        bool operator!=(const Type&) const = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "zeroValuePointPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
