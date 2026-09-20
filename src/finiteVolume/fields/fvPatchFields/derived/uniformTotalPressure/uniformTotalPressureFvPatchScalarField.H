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
    Foam::uniformTotalPressureFvPatchScalarField

Group
    grpInletBoundaryConditions grpOutletBoundaryConditions

Description
    This boundary condition provides a time-varying form of the uniform total
    pressure boundary condition Foam::totalPressureFvPatchField.

Usage
    \table
        Property     | Description                | Required    | Default value
        U            | Velocity field name        | no          | U
        phi          | Flux field name            | no          | phi
        rho          | Density field name         | no          | rho
        psi          | Compressibility field name | no          | none
        gamma        | (Cp/Cv)                    | no          | 1
        p0           | Total pressure as a function of time | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformTotalPressure;
        p0              uniform 1e5;
    }
    \endverbatim

    The \c p0 entry is specified as a Function1 type, able to describe
    time varying functions.

See also
    Foam::Function1Types
    Foam::uniformFixedValueFvPatchField
    Foam::totalPressureFvPatchField

SourceFiles
    uniformTotalPressureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_uniformTotalPressureFvPatchScalarField_H
#define Foam_uniformTotalPressureFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class uniformTotalPressureFvPatchField Declaration
\*---------------------------------------------------------------------------*/

class uniformTotalPressureFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    typedef uniformTotalPressureFvPatchScalarField this_bctype;
    typedef fixedValueFvPatchScalarField parent_bctype;

    // Private Data

        //- Name of the velocity field
        word UName_;

        //- Name of the flux transporting the field
        word phiName_;

        //- Name of the density field used to normalise the mass flux
        //- if necessary
        word rhoName_;

        //- Name of the compressibility field used to calculate the wave speed
        word psiName_;

        //- Heat capacity ratio
        scalar gamma_;

        //- Table of time vs total pressure, including the bounding treatment
        autoPtr<Function1<scalar>> p0_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "uniformTotalPressure");


    // Constructors

        //- Construct from patch and internal field
        uniformTotalPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformTotalPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformTotalPressureFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformTotalPressureFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        uniformTotalPressureFvPatchScalarField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
        }


    // Member functions

        // Access

            //- Return the name of the velocity field
            const word& UName() const
            {
                return UName_;
            }

            //- Return reference to the name of the velocity field
            //  to allow adjustment
            word& UName()
            {
                return UName_;
            }

            //- Return the heat capacity ratio
            scalar gamma() const
            {
                return gamma_;
            }

            //- Return reference to the heat capacity ratio to allow adjustment
            scalar& gamma()
            {
                return gamma_;
            }


        // Evaluation functions

            //- Inherit updateCoeffs from parent_bctype
            using parent_bctype::updateCoeffs;

            //- Update the coefficients associated with the patch field
            //- using the given patch velocity field
            virtual void updateCoeffs(const vectorField& Up);

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
