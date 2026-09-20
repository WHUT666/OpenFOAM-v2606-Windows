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
    Foam::pressureInletUniformVelocityFvPatchVectorField

Group
    grpInletBoundaryConditions

Description
    This velocity inlet boundary condition is applied to patches where the
    pressure is specified.  The uniform inflow velocity is obtained by
    averaging the flux over the patch, and then applying it in the direction
    normal to the patch faces.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            pressureInletUniformVelocity;
        value           uniform 0;
    }
    \endverbatim

SourceFiles
    pressureInletUniformVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_pressureInletUniformVelocityFvPatchVectorField_H
#define Foam_pressureInletUniformVelocityFvPatchVectorField_H

#include "pressureInletVelocityFvPatchVectorField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
       Class pressureInletUniformVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class pressureInletUniformVelocityFvPatchVectorField
:
    public pressureInletVelocityFvPatchVectorField
{
    typedef pressureInletUniformVelocityFvPatchVectorField this_bctype;
    typedef pressureInletVelocityFvPatchVectorField parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "pressureInletUniformVelocity");


    // Constructors

        //- Construct from patch and internal field
        pressureInletUniformVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        pressureInletUniformVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        pressureInletUniformVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        pressureInletUniformVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        pressureInletUniformVelocityFvPatchVectorField
        (
            const this_bctype&
        ) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member Functions

        //- True: this patch field is altered by assignment
        virtual bool assignable() const { return true; }


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();


    // Member Operators

        virtual void operator=(const fvPatchField<vector>& pvf);

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
