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
    Foam::mappedFlowRateFvPatchVectorField

Group
    grpInletBoundaryConditions grpCoupledBoundaryConditions

Description
    Describes a volumetric/mass flow normal vector boundary condition by its
    magnitude as an integral over its area.

    The inlet mass flux is taken from the neighbour region.

    The basis of the patch (volumetric or mass) is determined by the
    dimensions of the flux, phi.  The current density is used to correct the
    velocity when applying the mass basis.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        rho          | density field name      | no          | rho
        neigPhi      | name of flux field on neighbour mesh | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mappedFlowRate;
        phi             phi;
        rho             rho;
        neigPhi         phi;
        value           uniform (0 0 0); // placeholder
    }
    \endverbatim

SourceFiles
    mappedFlowRateFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_mappedFlowRateFvPatchVectorField_H
#define Foam_mappedFlowRateFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
               Class flowRateInletVelocityFvPatch Declaration
\*---------------------------------------------------------------------------*/

class mappedFlowRateFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef mappedFlowRateFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

    // Private Data

        //- Name of the neighbour flux setting the inlet mass flux
        word nbrPhiName_;

        //- Name of the local mass flux
        word phiName_;

        //- Name of the density field used to normalize the mass flux
        word rhoName_;


public:

   //- Runtime type information
   TypeNameApi(finiteVolume_API, "mappedFlowRate");


   // Constructors

        //- Construct from patch and internal field
        mappedFlowRateFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFlowRateFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        mappedFlowRateFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        mappedFlowRateFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        mappedFlowRateFvPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
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
