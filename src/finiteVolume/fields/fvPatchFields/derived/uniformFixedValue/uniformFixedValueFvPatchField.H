/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018-2023 OpenCFD Ltd.
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
    Foam::uniformFixedValueFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition provides a uniform fixed value condition.

Usage
    \table
        Property     | Description                  | Required | Default
        uniformValue | uniform value                | yes |
        value        | initial field value          | optional |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformFixedValue;
        uniformValue    constant 0.2;
    }
    \endverbatim

Note
    The uniformValue entry is a PatchFunction1 type,
    able to describe time and spatial varying functions.
    The example above gives the usage for supplying a constant value.

    The \c value entry (optional) is used for the initial values.
    Otherwise the \c uniformValue is evaluated.
    In some cases (eg, coded or expression entries with references to other
    fields) this can be problematic and the \c value entry will be needed.

See also
    Foam::Function1Types
    Foam::fixedValueFvPatchField

SourceFiles
    uniformFixedValueFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformFixedValueFvPatchField_defines_typeName in .C files that
//  define the template statics of uniformFixedValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformFixedValueFvPatchField_defines_typeName)
#  define Foam_uniformFixedValueFvPatchField_typeName_API
#else
#  define Foam_uniformFixedValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_uniformFixedValueFvPatchField_H
#define Foam_uniformFixedValueFvPatchField_H

#include "fixedValueFvPatchFields.H"
#include "PatchFunction1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class uniformFixedValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    typedef uniformFixedValueFvPatchField<Type> this_bctype;
    typedef fixedValueFvPatchField<Type> parent_bctype;

    // Private Data

        //- Function providing the value
        autoPtr<PatchFunction1<Type>> refValueFunc_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformFixedValueFvPatchField_typeName_API, "uniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        uniformFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch and internal field and patch field
        uniformFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>& fld
        );

        //- Construct from patch, internal field and dictionary
        uniformFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformFixedValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformFixedValueFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        uniformFixedValueFvPatchField(const this_bctype& pfld)
        #ifdef Foam_fvPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>&,
                const labelList&
            );


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream& os) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "uniformFixedValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
