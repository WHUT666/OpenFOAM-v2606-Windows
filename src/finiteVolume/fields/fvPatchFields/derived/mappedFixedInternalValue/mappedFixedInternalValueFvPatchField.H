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
    Foam::mappedFixedInternalValueFvPatchField

Group
    grpGenericBoundaryConditions grpCoupledBoundaryConditions

Description
    This boundary condition maps the boundary and internal values of a
    neighbour patch field to the boundary and internal values of *this.

Usage
    \table
        Property     | Description                      | Required    | Default
        field        | name of field to be mapped       | no  | this field name
        setAverage   | Use average value                | no  | false
        average      | Average value to use if \c setAverage = yes | partly |
    \endtable

    \verbatim
    <patchName>
    {
        type            mappedFixedInternalValue;
        field           T;
        setAverage      false;
        average         0;
        value           uniform 0;
    }
    \endverbatim

Note
    This boundary condition can only be applied to patches that are of
    the \c mappedPolyPatch type.

See also
    Foam::mappedPatchBase
    Foam::mappedPolyPatch
    Foam::mappedFvPatch
    Foam::mappedFixedValueFvPatchField

SourceFiles
    mappedFixedInternalValueFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_mappedFixedInternalValueFvPatchField_defines_typeName in .C files that
//  define the template statics of mappedFixedInternalValueFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_mappedFixedInternalValueFvPatchField_defines_typeName)
#  define Foam_mappedFixedInternalValueFvPatchField_typeName_API
#else
#  define Foam_mappedFixedInternalValueFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_mappedFixedInternalValueFvPatchField_H
#define Foam_mappedFixedInternalValueFvPatchField_H

#include "mappedFixedValueFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
           Class mappedFixedInternalValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedInternalValueFvPatchField
:
    public mappedFixedValueFvPatchField<Type>
{
    typedef mappedFixedInternalValueFvPatchField<Type> this_bctype;
    typedef mappedFixedValueFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_mappedFixedInternalValueFvPatchField_typeName_API, "mappedFixedInternalValue");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        mappedFixedInternalValueFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        mappedFixedInternalValueFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        mappedFixedInternalValueFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member functions

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
    #include "mappedFixedInternalValueFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
