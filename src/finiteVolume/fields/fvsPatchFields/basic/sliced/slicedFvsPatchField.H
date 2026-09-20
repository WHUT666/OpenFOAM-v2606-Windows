/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2023-2024 OpenCFD Ltd.
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
    Foam::slicedFvsPatchField

Description
    Specialization of fvsPatchField which creates the underlying
    fvsPatchField as a slice of the given complete field.

    The destructor is wrapped to avoid deallocation of the storage of the
    complete fields when this is destroyed.

    Should only used as a template argument for SlicedGeometricField.

SourceFiles
    slicedFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_slicedFvsPatchField_defines_typeName in .C files that
//  define the template statics of slicedFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_slicedFvsPatchField_defines_typeName)
#  define Foam_slicedFvsPatchField_typeName_API
#else
#  define Foam_slicedFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_slicedFvsPatchField_H
#define Foam_slicedFvsPatchField_H

#include "fvsPatchField.H"
#include "processorFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class slicedFvsPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slicedFvsPatchField
:
    public fvsPatchField<Type>
{
    typedef slicedFvsPatchField<Type> this_bctype;
    typedef fvsPatchField<Type> parent_bctype;

public:

    //- The mesh processor patch type
    typedef processorFvPatch processorPatchType;


    //- Runtime type information
    TypeNameApi(Foam_slicedFvsPatchField_typeName_API, "sliced");


    // Constructors

        //- Construct from patch, internal field and field to slice
        slicedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const Field<Type>& completeOrBoundaryField,
            const bool isBoundaryOnly = false
        );

        //- Construct from patch and internal field
        slicedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //  Not implemented.
        slicedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        //  Not implemented.
        slicedFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        slicedFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        slicedFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor, resets pointers to avoid deletion of underlying field
    virtual ~slicedFvsPatchField();


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- False: this patch field is not altered by assignment.
        virtual bool assignable() const { return false; }

        //- Write includes "value" entry
        virtual void write(Ostream&) const;


    // Member Operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const fvsPatchField<Type>&) {}
        virtual void operator+=(const fvsPatchField<Type>&) {}
        virtual void operator-=(const fvsPatchField<Type>&) {}
        virtual void operator*=(const fvsPatchField<scalar>&) {}
        virtual void operator/=(const fvsPatchField<scalar>&) {}

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
    #include "slicedFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
