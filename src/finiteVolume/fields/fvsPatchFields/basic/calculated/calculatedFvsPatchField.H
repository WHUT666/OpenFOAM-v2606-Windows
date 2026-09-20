/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2024 OpenCFD Ltd.
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
    Foam::calculatedFvsPatchField

Description
    This boundary condition is not designed to be evaluated; it is assumed
    that the value is assigned via field assignment.

    The "value" entry is MUST_READ.

Usage
    \table
        Property     | Description             | Required  | Default
        value        | Patch face values       | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type    calculated;
        value   uniform (0 0 0);    // Required value entry
    }
    \endverbatim

SourceFiles
    calculatedFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_calculatedFvsPatchField_defines_typeName in .C files that
//  define the template statics of calculatedFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_calculatedFvsPatchField_defines_typeName)
#  define Foam_calculatedFvsPatchField_typeName_API
#else
#  define Foam_calculatedFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_calculatedFvsPatchField_H
#define Foam_calculatedFvsPatchField_H

#include "fvsPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class calculatedFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class calculatedFvsPatchField
:
    public fvsPatchField<Type>
{
    typedef calculatedFvsPatchField<Type> this_bctype;
    typedef fvsPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_calculatedFvsPatchField_typeName_API, "calculated");  // fieldTypes::calculatedTypeName_()


    // Constructors

        //- Construct from patch and internal field
        calculatedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary
        //- The "value" entry is MUST_READ.
        calculatedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        calculatedFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        calculatedFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        calculatedFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- True: this patch field fixes a value.
        virtual bool fixesValue() const { return true; }

        //- Write includes "value" entry
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "calculatedFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
