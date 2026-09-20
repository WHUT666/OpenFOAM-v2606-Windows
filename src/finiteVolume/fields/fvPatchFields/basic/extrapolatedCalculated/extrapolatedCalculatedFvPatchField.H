/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 OpenFOAM Foundation
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
    Foam::extrapolatedCalculatedFvPatchField

Group
    grpGenericBoundaryConditions

Description
    This boundary condition applies a zero-gradient condition from the patch
    internal field onto the patch faces when \c evaluated but may also be
    assigned.  \c snGrad returns the patch gradient evaluated from the current
    internal and patch field values rather than returning zero.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            extrapolatedCalculated;
    }
    \endverbatim

SourceFiles
    extrapolatedCalculatedFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_extrapolatedCalculatedFvPatchField_defines_typeName in .C files that
//  define the template statics of extrapolatedCalculatedFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_extrapolatedCalculatedFvPatchField_defines_typeName)
#  define Foam_extrapolatedCalculatedFvPatchField_typeName_API
#else
#  define Foam_extrapolatedCalculatedFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_extrapolatedCalculatedFvPatchField_H
#define Foam_extrapolatedCalculatedFvPatchField_H

#include "calculatedFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class extrapolatedCalculatedFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class extrapolatedCalculatedFvPatchField
:
    public calculatedFvPatchField<Type>
{
    typedef extrapolatedCalculatedFvPatchField<Type> this_bctype;
    typedef calculatedFvPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_extrapolatedCalculatedFvPatchField_typeName_API, "extrapolatedCalculated");
    // fieldTypes::extrapolatedCalculatedTypeName_()


    // Constructors

        //- Construct from patch and internal field
        extrapolatedCalculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        extrapolatedCalculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        extrapolatedCalculatedFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        extrapolatedCalculatedFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        extrapolatedCalculatedFvPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return fvPatchField<Type>::Clone(*this, iF);
        }


    // Member functions

        //- Evaluate the patch field
        virtual void evaluate
        (
            const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
        );


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "extrapolatedCalculatedFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
