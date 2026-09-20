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
    Foam::rotatingPressureInletOutletVelocityFvPatchVectorField

Group
    grpInletBoundaryConditions grpOutletBoundaryConditions

Description
    This velocity inlet/outlet boundary condition is applied to patches in a
    rotating frame where the pressure is specified.  A zero-gradient is applied
    for outflow (as defined by the flux); for inflow, the velocity is obtained
    from the flux with a direction normal to the patch faces.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        tangentialVelocity | tangential velocity field | no  |
        omega        | angular velocity of the frame [rad/s] | yes    |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            rotatingPressureInletOutletVelocity;
        phi             phi;
        tangentialVelocity uniform (0 0 0);
        omega           100;
    }
    \endverbatim

    The \c omega entry is a Function1 type, able to describe time varying
    functions.

Note
    Sign conventions:
    - positive flux (out of domain): apply zero-gradient condition
    - negative flux (into of domain): derive from the flux in the patch-normal
      direction

See also
    Foam::pressureInletOutletVelocityFvPatchVectorField
    Foam::Function1Types

SourceFiles
    rotatingPressureInletOutletVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_rotatingPressureInletOutletVelocityFvPatchVectorField_H
#define Foam_rotatingPressureInletOutletVelocityFvPatchVectorField_H

#include "fvPatchFields.H"
#include "pressureInletOutletVelocityFvPatchVectorField.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
   Class rotatingPressureInletOutletVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class rotatingPressureInletOutletVelocityFvPatchVectorField
:
    public pressureInletOutletVelocityFvPatchVectorField
{
    typedef rotatingPressureInletOutletVelocityFvPatchVectorField this_bctype;
    typedef pressureInletOutletVelocityFvPatchVectorField parent_bctype;

    // Private Data

        //- Angular velocity of the frame
        autoPtr<Function1<vector>> omega_;


    // Private Member Functions

        //- Calculate the tangentialVelocity from omega
        void calcTangentialVelocity();


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "rotatingPressureInletOutletVelocity");


    // Constructors

        //- Construct from patch and internal field
        rotatingPressureInletOutletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        rotatingPressureInletOutletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        rotatingPressureInletOutletVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        rotatingPressureInletOutletVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        rotatingPressureInletOutletVelocityFvPatchVectorField
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


    // Member functions

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
