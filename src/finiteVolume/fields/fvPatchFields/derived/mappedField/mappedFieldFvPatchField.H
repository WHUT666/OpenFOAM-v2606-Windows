/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018 OpenCFD Ltd.
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
    Foam::mappedFieldFvPatchField

Group
    grpGenericBoundaryConditions grpCoupledBoundaryConditions

Description
    This boundary condition provides a self-contained version of the \c mapped
    condition.  It does not use information on the patch; instead it holds
    the data locally.

Usage
    \table
        Property     | Description                      | Required | Default
        field        | Name of field to be mapped       | no  | this field name
        setAverage   | Use average value                | no  | false |
        average      | Average value to use if \c setAverage = yes | partly |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mappedField;
        field           T;              // optional field name
        setAverage      no;             // apply an average value
        average         0;              // average to apply if setAverage
        value           uniform 0;      // place holder
    }
    \endverbatim

Note
    Since this condition can be applied on a per-field and per-patch basis,
    it is possible to duplicate the mapping information.  If possible, employ
    the \c mapped condition in preference to avoid this situation, and only
    employ this condition if it is not possible to change the underlying
    geometric (poly) patch type to \c mapped.

See also
    Foam::mappedPatchBase
    Foam::mappedPolyPatch
    Foam::mappedFvPatch
    Foam::fixedValueFvPatchField

SourceFiles
    mappedFieldFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_mappedFieldFvPatchField_defines_typeName in .C files that
//  define the template statics of mappedFieldFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_mappedFieldFvPatchField_defines_typeName)
#  define Foam_mappedFieldFvPatchField_typeName_API
#else
#  define Foam_mappedFieldFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_mappedFieldFvPatchField_H
#define Foam_mappedFieldFvPatchField_H

#include "mappedPatchBase.H"
#include "mappedPatchFieldBase.H"
#include "fixedValueFvPatchFields.H"
#include "interpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class mappedFieldFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFieldFvPatchField
:
    public fixedValueFvPatchField<Type>,
    public mappedPatchBase,
    public mappedPatchFieldBase<Type>
{
    typedef mappedFieldFvPatchField<Type> this_bctype;
    typedef fixedValueFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_mappedFieldFvPatchField_typeName_API, "mappedField");


    // Constructors

        //- Construct from patch and internal field
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct from patch, internal field and distance for normal type
        //  sampling
        mappedFieldFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,

            // mappedPatchBase
            const word& sampleRegion,
            const sampleMode sampleMode,
            const word& samplePatch,
            const scalar distance,

            // My settings
            const word& fieldName,
            const bool setAverage,
            const Type average,
            const word& interpolationScheme
        );

        //- Construct by mapping onto a new patch
        mappedFieldFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        mappedFieldFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        mappedFieldFvPatchField(const this_bctype& pfld)
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


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


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
    #include "mappedFieldFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
