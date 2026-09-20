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
    Foam::rotatingWallVelocityFvPatchVectorField

Group
    grpWallBoundaryConditions grpGenericBoundaryConditions

Description
    This boundary condition provides a rotational velocity condition.

Usage
    \table
        Property     | Description             | Required    | Default value
        origin       | origin of rotation in Cartesian coordinates | yes|
        axis         | axis of rotation        | yes         |
        omega        | angular velocty of the frame [rad/s] | yes    |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            rotatingWallVelocity;
        origin          (0 0 0);
        axis            (0 0 1);
        omega           100;
    }
    \endverbatim

    The \c omega entry is a Function1 of time, see Foam::Function1Types.

See also
    Foam::fixedValueFvPatchField
    Foam::Function1Types

SourceFiles
    rotatingWallVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_rotatingWallVelocityFvPatchVectorField_H
#define Foam_rotatingWallVelocityFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
           Class rotatingWallVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class rotatingWallVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef rotatingWallVelocityFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

    // Private Data

        //- Origin of the rotation
        vector origin_;

        //- Axis of the rotation
        vector axis_;

        //- Rotational speed
        autoPtr<Function1<scalar>> omega_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "rotatingWallVelocity");


    // Constructors

        //- Construct from patch and internal field
        rotatingWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        rotatingWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        rotatingWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        rotatingWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        rotatingWallVelocityFvPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member functions

        // Access functions

            //- Return the origin of the rotation
            const vector& origin() const
            {
                return origin_;
            }

            //- Return the axis of the rotation
            const vector& axis() const
            {
                return axis_;
            }

            //- Return non-const access to the origin of the rotation
            vector& origin()
            {
                return origin_;
            }

            //- Return non-const access to the axis of the rotation
            vector& axis()
            {
                return axis_;
            }


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
