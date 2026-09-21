/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2012 OpenFOAM Foundation
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
    Foam::smoluchowskiJumpTFvPatchScalarField

Description
    Smoluchowski temperature jump boundary condition

SourceFiles
    smoluchowskiJumpTFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef smoluchowskiJumpTFvPatchScalarField_H
#define smoluchowskiJumpTFvPatchScalarField_H

#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
            Class smoluchowskiJumpTFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class smoluchowskiJumpTFvPatchScalarField
:
    public mixedFvPatchScalarField
{
    typedef smoluchowskiJumpTFvPatchScalarField this_bctype;
    typedef mixedFvPatchScalarField parent_bctype;

    // Private Data

        //- Velocity field name, default = "U"
        word UName_;

        //- Density field name, default = "rho"
        word rhoName_;

        //- Compressibility field name, default = "thermo:psi"
        word psiName_;

        //- Dynamic viscosity field name, default = "thermo:mu"
        word muName_;

        //- Accommodation coefficient
        scalar accommodationCoeff_;

        //- Wall surface temperature
        scalarField Twall_;

        //- Heat capacity ratio (default 1.4)
        scalar gamma_;


public:

    //- Runtime type information
    TypeNameApi(rhoCentralFoam_API, "smoluchowskiJumpT");


    // Constructors

        //- Construct from patch and internal field
        smoluchowskiJumpTFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        smoluchowskiJumpTFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        smoluchowskiJumpTFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        smoluchowskiJumpTFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        smoluchowskiJumpTFvPatchScalarField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
        }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<scalar>&,
                const labelList&
            );


        // Evaluation functions

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
