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
    Foam::translatingWallVelocityFvPatchVectorField

Group
    grpWallBoundaryConditions grpGenericBoundaryConditions

Description
    This boundary condition provides a velocity condition for translational
    motion on walls.

Usage
    \table
        Property     | Description             | Required    | Default value
        U            | translational velocity  | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            translatingWallVelocity;
        U               (100 0 0);
    }
    \endverbatim

    The \c U entry is a Function1 of time, see Foam::Function1Types.


See also
    Foam::fixedValueFvPatchField
    Foam::Function1Types

SourceFiles
    translatingWallVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_translatingWallVelocityFvPatchVectorField_H
#define Foam_translatingWallVelocityFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
            Class translatingWallVelocityFvPatchField Declaration
\*---------------------------------------------------------------------------*/

class translatingWallVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef translatingWallVelocityFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

    // Private Data

        //- Translational velocity
        autoPtr<Function1<vector>> U_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "translatingWallVelocity");


    // Constructors

        //- Construct from patch and internal field
        translatingWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        translatingWallVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        translatingWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        translatingWallVelocityFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        translatingWallVelocityFvPatchVectorField(const this_bctype&) = delete;

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
