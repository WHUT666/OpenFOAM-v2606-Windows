/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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
    Foam::fixedFluxExtrapolatedPressureFvPatchScalarField

Group
    grpInletBoundaryConditions grpWallBoundaryConditions

Description
    This boundary condition sets the pressure gradient to the provided value
    such that the flux on the boundary is that specified by the velocity
    boundary condition.

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedFluxExtrapolatedPressure;
    }
    \endverbatim

See also
    Foam::fixedGradientFvPatchField

SourceFiles
    fixedFluxExtrapolatedPressureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_fixedFluxExtrapolatedPressureFvPatchScalarField_H
#define Foam_fixedFluxExtrapolatedPressureFvPatchScalarField_H

#include "fixedFluxPressureFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class fixedFluxExtrapolatedPressureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class fixedFluxExtrapolatedPressureFvPatchScalarField
:
    public fixedFluxPressureFvPatchScalarField
{
    typedef fixedFluxExtrapolatedPressureFvPatchScalarField this_bctype;
    typedef fixedFluxPressureFvPatchScalarField parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "fixedFluxExtrapolatedPressure");


    // Constructors

        //- Construct from patch and internal field
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        fixedFluxExtrapolatedPressureFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        fixedFluxExtrapolatedPressureFvPatchScalarField
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


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
