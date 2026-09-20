/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenCFD Ltd.
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
    Foam::waveAlphaFvPatchScalarField

Description

    Example of the boundary condition specification:
    \verbatim
    inlet
    {
        type            waveAlpha;
        value           uniform (0 0 0);
    }
    \endverbatim

Usage
    \table
        Property    | Description          | Required    | Default value
        type        | type: waveAlpha      | yes         |
        waveDict    | Dictionary specifying wave variables | no | waveProperties
    \endtable

Note
    - The value is positive inwards
    - May not work correctly for transonic inlets
    - Strange behaviour with potentialFoam since the U equation is not solved

SourceFiles
    waveAlphaFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef waveAlphaFvPatchScalarField_H
#define waveAlphaFvPatchScalarField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                class waveAlphaFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class waveAlphaFvPatchScalarField
:
    public fixedValueFvPatchScalarField
{
    typedef waveAlphaFvPatchScalarField this_bctype;
    typedef fixedValueFvPatchScalarField parent_bctype;

    // Private Data

        //- Dictionary name
        word waveDictName_;


public:

   //- Runtime type information
   TypeNameApi(waveModels_API, "waveAlpha");


   // Constructors

        //- Construct from patch and internal field
        waveAlphaFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        waveAlphaFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        waveAlphaFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        waveAlphaFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        waveAlphaFvPatchScalarField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
        }


    // Member Functions

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
