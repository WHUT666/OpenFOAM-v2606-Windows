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
    Foam::noSlipFvPatchVectorField

Group
    grpWallBoundaryConditions

Description
    This boundary condition fixes the velocity to zero (eg, at walls).

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type    noSlip;
    }
    \endverbatim

See also
    Foam::fixedValueFvPatchVectorField
    Foam::zeroValueFvPatchVectorField

SourceFiles
    noSlipFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_noSlipFvPatchVectorField_H
#define Foam_noSlipFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                  Class noSlipFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class noSlipFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef noSlipFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "noSlip");


    // Constructors

        //- Construct from patch and internal field
        noSlipFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        noSlipFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        noSlipFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Copy construct with internal field reference
        noSlipFvPatchVectorField
        (
            const this_bctype& pfld,
            const DimensionedField<vector, volMesh>& iF
        );

        //- No copy without an internal field
        [[deprecated("2026-03: soon = delete")]]
        noSlipFvPatchVectorField(const this_bctype& pfld)
        #ifdef Foam_fvPatchField_copyConstruct
        : this_bctype(pfld, pfld.internalField()) {}
        #else
        = delete;
        #endif

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member Functions

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
