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
    Foam::waveSurfacePressureFvPatchScalarField

Group
    grpInletBoundaryConditions

Description
    This is a pressure boundary condition, whose value is calculated as
    the hydrostatic pressure based on a given displacement:

        \f[
            p = -\rho*g*\zeta
        \f]

    \vartable
        \rho  | density [kg/m3]
        g     | acceleration due to gravity [m/s2]
        \zeta | wave amplitude [m]
    \endvartable

    The wave amplitude is updated as part of the calculation, derived from the
    local volumetric flux.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | flux field name         | no          | phi
        rho          | density field name      | no          | rho
        zeta         | wave amplitude field name | no        | zeta
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            waveSurfacePressure;
        phi             phi;
        rho             rho;
        zeta            zeta;
        value           uniform 0;  // place holder
    }
    \endverbatim

    The density field is only required if the flux is mass-based as opposed to
    volumetric-based.

See also
    Foam::fixedValueFvPatchField

SourceFiles
    waveSurfacePressureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_waveSurfacePressureFvPatchScalarField_H
#define Foam_waveSurfacePressureFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
            Class waveSurfacePressureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class waveSurfacePressureFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    typedef waveSurfacePressureFvPatchScalarField this_bctype;
    typedef fixedValueFvPatchScalarField parent_bctype;

public:

    // Public data

        //- Enumeration defining the available ddt schemes
        enum ddtSchemeType
        {
            tsEuler,
            tsCrankNicolson,
            tsBackward
        };


private:

    // Private data

        //- Flux field name
        word phiName_;

        //- Wave height field name
        word zetaName_;

        //- Density field for mass-based flux evaluations
        word rhoName_;

        //- Time scheme type names
        static finiteVolume_API const Enum<ddtSchemeType> ddtSchemeTypeNames_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "waveSurfacePressure");


    // Constructors

        //- Construct from patch and internal field
        waveSurfacePressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        waveSurfacePressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        waveSurfacePressureFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        waveSurfacePressureFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        waveSurfacePressureFvPatchScalarField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
        }


    // Member Functions

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
