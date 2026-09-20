/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2014-2022 PCOpt/NTUA
    Copyright (C) 2014-2022 FOSS GP
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
    Foam::kaqRWallFunctionFvPatchScalarField

Description

SourceFiles
    kaqRWallFunctionFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef kaqRWallFunctionFvPatchScalarField_H
#define kaqRWallFunctionFvPatchScalarField_H

#include "kqRWallFunctionFvPatchField.H"
#include "adjointBoundaryConditions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class kaqRWallFunctionFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class kaqRWallFunctionFvPatchScalarField
:
    public kqRWallFunctionFvPatchField<scalar>,
    public adjointScalarBoundaryCondition
{
    typedef kaqRWallFunctionFvPatchScalarField this_bctype;
    typedef kqRWallFunctionFvPatchField<scalar> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(adjointOptimisation_API, "kaqRWallFunction");


    // Constructors

        //- Construct from patch and internal field
        kaqRWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        kaqRWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        kaqRWallFunctionFvPatchScalarField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        kaqRWallFunctionFvPatchScalarField
        (
            const this_bctype&,
            const DimensionedField<scalar, volMesh>&
        );

        //- No copy without an internal field
        kaqRWallFunctionFvPatchScalarField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<scalar>> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return fvPatchField<scalar>::Clone(*this, iF);
        }


    // Member functions

        //- Add source terms to the rhs of the first cell centre
        virtual void manipulateMatrix(fvMatrix<scalar>& matrix);

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
