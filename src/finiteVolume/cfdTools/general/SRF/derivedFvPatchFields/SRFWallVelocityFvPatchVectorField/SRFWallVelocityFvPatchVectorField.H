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
    Foam::SRFWallVelocityFvPatchVectorField

Group
    grpWallBoundaryConditions

Description
    Wall-velocity condition to be used in conjunction with the single rotating
    frame (SRF) model (see: FOAM::SRFModel)

    The condition applies the appropriate rotation transformation in time and
    space to determine the local SRF velocity of the wall.

        \f[
            U_p = - U_{p,srf}
        \f]

    where
    \vartable
        U_p     = patch velocity [m/s]
        U_{p,srf} = SRF velocity
    \endvartable

    The normal component of \f$ U_p \f$ is removed to ensure 0 wall-flux even
    if the wall patch faces are irregular.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            SRFWallVelocity;
        value           uniform (0 0 0);    // Initial value
    }
    \endverbatim

See also
    Foam::SRFModel
    Foam::SRFVelocityFvPatchVectorField
    Foam::fixedValueFvPatchField

SourceFiles
    SRFWallVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_SRFWallVelocityFvPatchVectorField_H
#define Foam_SRFWallVelocityFvPatchVectorField_H

#include "fvPatchFields.H"
#include "fixedValueFvPatchFields.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
               Class SRFWallVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class SRFWallVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef SRFWallVelocityFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "SRFWallVelocity");


    // Constructors

        //- Construct from patch and internal field
        SRFWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        SRFWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        SRFWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        SRFWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        SRFWallVelocityFvPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

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
