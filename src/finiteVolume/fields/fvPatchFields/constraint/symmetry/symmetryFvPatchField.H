/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::symmetryFvPatchField

Group
    grpConstraintBoundaryConditions

Description
    This boundary condition enforces a symmetry constraint

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            symmetry;
    }
    \endverbatim

SourceFiles
    symmetryFvPatchField.txx
    symmetryFvPatchFields.cxx
    symmetryFvPatchFields.H
    symmetryFvPatchFieldsFwd.H

\*---------------------------------------------------------------------------*/

//- Define Foam_symmetryFvPatchField_defines_typeName in .C files that
//  define the template statics of symmetryFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_symmetryFvPatchField_defines_typeName)
#  define Foam_symmetryFvPatchField_typeName_API
#else
#  define Foam_symmetryFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_symmetryFvPatchField_H
#define Foam_symmetryFvPatchField_H

#include "basicSymmetryFvPatchField.H"
#include "symmetryFvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class symmetryFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class symmetryFvPatchField
:
    public basicSymmetryFvPatchField<Type>
{
    typedef symmetryFvPatchField<Type> this_bctype;
    typedef basicSymmetryFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_symmetryFvPatchField_typeName_API, symmetryFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        symmetryFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        symmetryFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        symmetryFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        symmetryFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        symmetryFvPatchField(const this_bctype& pfld)
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


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "symmetryFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
