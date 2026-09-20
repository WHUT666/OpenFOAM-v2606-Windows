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
    Foam::fixedMeanFvPatchField

Group
    grpInletBoundaryConditions

Description
    This boundary condition extrapolates field to the patch using the near-cell
    values and adjusts the distribution to match the specified, optionally
    time-varying, mean value.

Usage
    \table
        Property     | Description             | Required    | Default value
        meanValue    | mean value Function1    | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedMean;
        meanValue       1.0;
    }
    \endverbatim

See also
    Foam::fixedValueFvPatchField
    Foam::Function1Types

SourceFiles
    fixedMeanFvPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_fixedMeanFvPatchField_defines_typeName in .C files that
//  define the template statics of fixedMeanFvPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_fixedMeanFvPatchField_defines_typeName)
#  define Foam_fixedMeanFvPatchField_typeName_API
#else
#  define Foam_fixedMeanFvPatchField_typeName_API finiteVolume_API
#endif

#ifndef Foam_fixedMeanFvPatchField_H
#define Foam_fixedMeanFvPatchField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class fixedMeanFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedMeanFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    typedef fixedMeanFvPatchField<Type> this_bctype;
    typedef fixedValueFvPatchField<Type> parent_bctype;

    // Private Data

        //- MeanValue value the field is adjusted to maintain
        autoPtr<Function1<Type>> meanValue_;


public:

    //- Runtime type information
    TypeNameApi(Foam_fixedMeanFvPatchField_typeName_API, "fixedMean");


    // Constructors

        //- Construct from patch and internal field
        fixedMeanFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedMeanFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        fixedMeanFvPatchField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        fixedMeanFvPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, volMesh>&
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        fixedMeanFvPatchField(const this_bctype& pfld)
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
    #include "fixedMeanFvPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
