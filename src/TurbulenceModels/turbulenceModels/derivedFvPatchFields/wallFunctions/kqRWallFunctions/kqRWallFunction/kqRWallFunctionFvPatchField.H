/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017, 2019 OpenFOAM Foundation
    Copyright (C) 2019-2022 OpenCFD Ltd.
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
    Foam::kqRWallFunctionFvPatchField

Group
    grpWallFunctions

Description
    This boundary condition provides a simple wrapper around the zero-gradient
    condition, which can be used for the turbulent kinetic energy (i.e. \c k),
    square-root of turbulent kinetic energy (i.e. \c q) and Reynolds stress
    symmetric-tensor fields (i.e. \c R) for the cases of high Reynolds number
    flow using wall functions. It is not a wall-function condition.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        // Mandatory entries
        type            kqRWallFunction;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                   | Type | Reqd | Deflt
      type      | Type name: kqRWallFunction    | word | yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link zeroGradientFvPatchField.H \endlink

SourceFiles
    kqRWallFunctionFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_kqRWallFunctionFvPatchField_defines_typeName in .C files that
//  define the template statics of kqRWallFunctionFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_kqRWallFunctionFvPatchField_defines_typeName)
#  define Foam_kqRWallFunctionFvPatchField_typeName_API
#else
#  define Foam_kqRWallFunctionFvPatchField_typeName_API turbulenceModels_API
#endif

#ifndef kqRWallFunctionFvPatchField_H
#define kqRWallFunctionFvPatchField_H

#include "zeroGradientFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class kqRWallFunctionFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class kqRWallFunctionFvPatchField
:
    public zeroGradientFvPatchField<Type>
{
    typedef kqRWallFunctionFvPatchField this_bctype;
    typedef zeroGradientFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_kqRWallFunctionFvPatchField_typeName_API, "kqRWallFunction");


    // Constructors

        //- Construct from patch and internal field
        kqRWallFunctionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        kqRWallFunctionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        kqRWallFunctionFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        kqRWallFunctionFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        kqRWallFunctionFvPatchField
        (
            const this_bctype&
        ) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Evaluate the patchField
        virtual void evaluate
        (
            const Pstream::commsTypes commsType = Pstream::commsTypes::buffered
        );

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
    #include "kqRWallFunctionFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
