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
    Foam::fixedPressureCompressibleDensityFvPatchScalarField

Group
    grpInletBoundaryConditions

Description
    This boundary condition calculates a (liquid) compressible density as a
    function of pressure and fluid properties:

        \f[
            \rho = \rho_{l,sat} + \psi_l*(p - p_{sat})
        \f]

    where

    \vartable
        \rho    | density [kg/m3]
        \rho_{l,sat} | saturation liquid density [kg/m3]
        \psi_l  | liquid compressibility
        p       | pressure [Pa]
        p_{sat} | saturation pressure [Pa]
    \endvartable

    The variables \c rholSat, \c pSat and \c psil are retrieved from the
    \c thermodynamicProperties dictionary.

Usage
    \table
        Property     | Description             | Required    | Default value
        p            | pressure field name     | no          | p
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type        fixedPressureCompressibleDensity;
        p           p;
        value       uniform 1;
    }
    \endverbatim

See also
    Foam::fixedValueFvPatchField

SourceFiles
    fixedPressureCompressibleDensityFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_fixedPressureCompressibleDensityFvPatchScalarField_H
#define Foam_fixedPressureCompressibleDensityFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
      Class fixedPressureCompressibleDensityFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class fixedPressureCompressibleDensityFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    typedef fixedPressureCompressibleDensityFvPatchScalarField this_bctype;
    typedef fixedValueFvPatchScalarField parent_bctype;

    // Private Data

        //- Name of static pressure field
        word pName_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "fixedPressureCompressibleDensity");


    // Constructors

        //- Construct from patch and internal field
        fixedPressureCompressibleDensityFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedPressureCompressibleDensityFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        fixedPressureCompressibleDensityFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        fixedPressureCompressibleDensityFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        fixedPressureCompressibleDensityFvPatchScalarField
        (
            const this_bctype&
        ) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
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

#endif

// ************************************************************************* //
