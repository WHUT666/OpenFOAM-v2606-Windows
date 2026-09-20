/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
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
    Foam::uniformJumpFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition provides a jump condition, using the \c cyclic
    condition as a base.  The jump is specified as a time-varying uniform
    value across the patch.

Usage
    \table
        Property    | Description                           | Required | Default
        patchType   | underlying patch type should be \c cyclic | yes |
        jumpTable   | jump value                            | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformJump;
        patchType       cyclic;
        jumpTable       constant 10;
    }
    \endverbatim

    The above example shows the use of a fixed jump of '10'.

Note
    The uniformValue entry is a Function1 type, able to describe time
    varying functions.  The example above gives the usage for supplying a
    constant value.

    The underlying \c patchType should be set to \c cyclic

See also
    Foam::fixedJumpFvPatchField
    Foam::Function1Types

SourceFiles
    uniformJumpFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformJumpFvPatchField_defines_typeName in .C files that
//  define the template statics of uniformJumpFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformJumpFvPatchField_defines_typeName)
#  define Foam_uniformJumpFvPatchField_typeName_API
#else
#  define Foam_uniformJumpFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_uniformJumpFvPatchField_H
#define Foam_uniformJumpFvPatchField_H

#include "fixedJumpFvPatchField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class uniformJumpFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformJumpFvPatchField
:
    public fixedJumpFvPatchField<Type>
{
    typedef uniformJumpFvPatchField<Type> this_bctype;
    typedef fixedJumpFvPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- The "jump" table
        autoPtr<Function1<Type>> jumpTable_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformJumpFvPatchField_typeName_API, "uniformJump");

    // Constructors

        //- Construct from patch and internal field
        uniformJumpFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformJumpFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            const bool needValue = true
        );

        //- Construct by mapping onto a new patch
        uniformJumpFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformJumpFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        uniformJumpFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member functions

        //- Update the coefficients
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
    #include "uniformJumpFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
