/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2023 OpenCFD Ltd.
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
    Foam::slicedFaePatchField

Description
    Specialization of faePatchField which creates the underlying
    faePatchField as a slice of the given complete field.

    The destructor is wrapped to avoid deallocation of the storage of the
    complete fields when this is destroyed.

    Should only used as a template argument for SlicedGeometricField.

SourceFiles
    slicedFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_slicedFaePatchField_defines_typeName in .C files that
//  define the template statics of slicedFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_slicedFaePatchField_defines_typeName)
#  define Foam_slicedFaePatchField_typeName_API
#else
#  define Foam_slicedFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_slicedFaePatchField_H
#define Foam_slicedFaePatchField_H

#include "faePatchField.H"
#include "processorFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class slicedFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slicedFaePatchField
:
    public faePatchField<Type>
{
    typedef slicedFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Type for the mesh processor patch
    typedef processorFaPatch processorPatchType;


    //- Runtime type information
    TypeNameApi(Foam_slicedFaePatchField_typeName_API, "sliced");


    // Constructors

        //- Construct from patch, internal field and field to slice
        slicedFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const Field<Type>& completeOrBoundaryField,
            const bool isBoundaryOnly = false
        );

        //- Construct from patch and internal field
        slicedFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary
        slicedFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        slicedFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        slicedFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        slicedFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    //- Destructor, resets pointers to avoid deletion of underlying field
    virtual ~slicedFaePatchField();


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- Write includes "value" entry
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
    #include "slicedFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
