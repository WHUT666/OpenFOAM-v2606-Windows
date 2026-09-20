/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2020 OpenCFD Ltd.
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
    Foam::mappedMixedFvPatchField

Group
    grpGenericBoundaryConditions grpCoupledBoundaryConditions

Description
    This boundary condition maps the value at a set of cells or patch faces
    back to *this.

    The sample mode is set by the underlying mapping engine, provided by the
    mappedPatchBase class.

Usage
    \table
        Property     | Description                      | Required | Default
        field        | Name of field to be mapped       | no  | this field name
        weightField  | Name of field to use as weight   | no |
        setAverage   | Use average value                | no  | false |
        average      | Average value to use if \c setAverage = yes | partly |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mappedMixed;
        field           T;              // optional field name

        value           uniform 273.0;

        refValue        $value;
        refGradient     uniform 0.0;
        valueFraction   uniform 1.0;
    }
    \endverbatim

Note
    Supports multi-world operation:

    +----+                      +----+
    |    |                      |    |
    |    |                      |    |
    +----+                      +----+
    worldA                      worldB
    regionA                     regionB
    patchA                      patchB


See also
    Foam::mappedPatchBase
    Foam::mappedPolyPatch
    Foam::mappedFvPatch
    Foam::mappedFieldFvPatchField
    Foam::mixedFvPatchField

SourceFiles
    mappedMixedFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_mappedMixedFvPatchField_defines_typeName in .C files that
//  define the template statics of mappedMixedFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_mappedMixedFvPatchField_defines_typeName)
#  define Foam_mappedMixedFvPatchField_typeName_API
#else
#  define Foam_mappedMixedFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_mappedMixedFvPatchField_H
#define Foam_mappedMixedFvPatchField_H

#include "mappedPatchFieldBase.H"
#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class mappedMixedFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedMixedFvPatchField
:
    public mixedFvPatchField<Type>,
    public mappedPatchFieldBase<Type>
{
    typedef mappedMixedFvPatchField<Type> this_bctype;
    typedef mixedFvPatchField<Type> parent_bctype;

    // Private Data

        //- Name of weight field to sample
        word weightFieldName_;


public:

    //- Runtime type information
    TypeNameApi(Foam_mappedMixedFvPatchField_typeName_API, "mappedMixed");


    // Constructors

        //- Construct from patch and internal field
        mappedMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        mappedMixedFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        mappedMixedFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        mappedMixedFvPatchField(const this_bctype&) = delete;

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
    #include "mappedMixedFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
