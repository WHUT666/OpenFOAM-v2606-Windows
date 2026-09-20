/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
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
    Foam::symmetryPlaneFvsPatchField

Description
    This boundary condition enforces a symmetryPlane constraint

    The "value" entry is MUST_READ.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type    symmetryPlane;
        value   ...;
    }
    \endverbatim

SourceFiles
    symmetryPlaneFvsPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_symmetryPlaneFvsPatchField_defines_typeName in .C files that
//  define the template statics of symmetryPlaneFvsPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_symmetryPlaneFvsPatchField_defines_typeName)
#  define Foam_symmetryPlaneFvsPatchField_typeName_API
#else
#  define Foam_symmetryPlaneFvsPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_symmetryPlaneFvsPatchField_H
#define Foam_symmetryPlaneFvsPatchField_H

#include "fvsPatchField.H"
#include "symmetryPlaneFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class symmetryPlaneFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class symmetryPlaneFvsPatchField
:
    public fvsPatchField<Type>
{
    typedef symmetryPlaneFvsPatchField<Type> this_bctype;
    typedef fvsPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_symmetryPlaneFvsPatchField_typeName_API, symmetryPlaneFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        symmetryPlaneFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is MUST_READ.
        symmetryPlaneFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        symmetryPlaneFvsPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        symmetryPlaneFvsPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- No copy without an internal field
        symmetryPlaneFvsPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvsPatchField<Type>> clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return fvsPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

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
    #include "symmetryPlaneFvsPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
