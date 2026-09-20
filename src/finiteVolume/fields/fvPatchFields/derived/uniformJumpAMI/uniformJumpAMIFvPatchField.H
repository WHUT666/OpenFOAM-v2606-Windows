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
    Foam::uniformJumpAMIFvPatchField

Group
    grpCoupledBoundaryConditions

Description
    This boundary condition provides a jump condition, using the \c cyclicAMI
    condition as a base.  The jump is specified as a time-varying uniform
    value across the patch.

Usage
    \table
        Property     | Description             | Required    | Default value
        patchType    | underlying patch type should be \c cyclicAMI| yes |
        jumpTable    | jump value              | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformJumpAMI;
        patchType       cyclicAMI;
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
    Foam::jumpCyclicAMIFvPatchField
    Foam::Function1Types

SourceFiles
    uniformJumpAMIFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_uniformJumpAMIFvPatchField_defines_typeName in .C files that
//  define the template statics of uniformJumpAMIFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_uniformJumpAMIFvPatchField_defines_typeName)
#  define Foam_uniformJumpAMIFvPatchField_typeName_API
#else
#  define Foam_uniformJumpAMIFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_uniformJumpAMIFvPatchField_H
#define Foam_uniformJumpAMIFvPatchField_H

#include "fixedJumpAMIFvPatchField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class uniformJumpAMIFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformJumpAMIFvPatchField
:
    public fixedJumpAMIFvPatchField<Type>
{
    typedef uniformJumpAMIFvPatchField<Type> this_bctype;
    typedef fixedJumpAMIFvPatchField<Type> parent_bctype;

protected:

    // Protected Data

        //- "jump" table
        autoPtr<Function1<Type>> jumpTable_;


public:

    //- Runtime type information
    TypeNameApi(Foam_uniformJumpAMIFvPatchField_typeName_API, "uniformJumpAMI");

    // Constructors

        //- Construct from patch and internal field
        uniformJumpAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformJumpAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformJumpAMIFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformJumpAMIFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        uniformJumpAMIFvPatchField(const this_bctype&) = delete;

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
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "uniformJumpAMIFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
