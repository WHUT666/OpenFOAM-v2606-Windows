/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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
    Foam::scaledFixedValueFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This condition applies a scalar multiplier to the value of another
    boundary condition.

Usage
    \table
        Property     | Description             | Required    | Default value
        scale        | Time varying scale      | yes         |
        refValue     | patchField providing the raw patch value | yes |
    \endtable

    Example of the boundary condition specification to scale a reference
    velocity of (15 0 0)  supplied as a fixedValue by a table of values
    that ramps the scale from 0 to 1 over 1 second:
    \verbatim
    <patchName>
    {
        type            scaledFixedValue;

        scale table
        (
            (    0   0)
            (  1.0 1.0)
            (100.0 1.0)
        );

        refValue
        {
            type            fixedValue;
            value           uniform (15 0 0);
        }
    }
    \endverbatim

SourceFiles
    scaledFixedValueFvPatchField.txx

SeeAlso
    Foam::PatchFunction1

\*---------------------------------------------------------------------------*/

//- Define Foam_scaledFixedValueFvPatchField_defines_typeName in .C files that
//  define the template statics of scaledFixedValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_scaledFixedValueFvPatchField_defines_typeName)
#  define Foam_scaledFixedValueFvPatchField_typeName_API
#else
#  define Foam_scaledFixedValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_scaledFixedValueFvPatchField_H
#define Foam_scaledFixedValueFvPatchField_H

#include "fixedValueFvPatchField.H"
#include "PatchFunction1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class scaledFixedValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class scaledFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    typedef scaledFixedValueFvPatchField<Type> this_bctype;
    typedef fixedValueFvPatchField<Type> parent_bctype;

protected:

    // Protected data

        //- Scalar scale factor
        autoPtr<PatchFunction1<scalar>> scalePtr_;

        //- Condition to supply the reference value
        tmp<fvPatchField<Type>> refValuePtr_;


public:

    //- Runtime type information
    TypeNameApi(Foam_scaledFixedValueFvPatchField_typeName_API, "scaledFixedValue");


    // Constructors

        //- Construct from patch and internal field
        scaledFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        scaledFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        scaledFixedValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        scaledFixedValueFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        scaledFixedValueFvPatchField(const this_bctype& pfld)
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


    // Member functions

        // Access

            //- Return the reference value condition
            const fvPatchField<Type>& refValue() const
            {
                return refValuePtr_();
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap(const fvPatchFieldMapper& m);

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>& ptf,
                const labelList& addr
            );


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;

        // Force an assignment, applying scaling to refValue

        virtual void operator==(const fvPatchField<Type>&);
        virtual void operator==(const Field<Type>&);
        virtual void operator==(const Type&);

        // Prevent automatic comparison rewriting (c++20)
        bool operator!=(const fvPatchField<Type>&) const = delete;
        bool operator!=(const Field<Type>&) const = delete;
        bool operator!=(const Type&) const = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "scaledFixedValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
