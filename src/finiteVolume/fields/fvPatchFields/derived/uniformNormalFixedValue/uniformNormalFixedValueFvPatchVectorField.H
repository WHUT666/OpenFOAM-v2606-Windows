/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019 OpenCFD Ltd.
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
    Foam::uniformNormalFixedValueFvPatchVectorField

Group
    grpGenericBoundaryConditions grpInletBoundaryConditions

Description
    This boundary condition provides a uniform surface-normal
    vector boundary condition by its magnitude.

Usage
    \table
        Property     | Description                          | Required | Default
        uniformValue | uniform value                        | yes |
        ramp         | time-based ramping                   | no  |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            uniformNormalFixedValue;
        uniformValue    constant -10;          // 10 INTO the domain
    }
    \endverbatim

Note
    Sign conventions:
    - the value is positive for outward-pointing vectors

See also
    Foam::Function1Types
    Foam::fixedValueFvPatchField
    Foam::surfaceNormalFixedValueFvPatchVectorField
    Foam::uniformFixedValueFvPatchVectorField

SourceFiles
    uniformNormalFixedValueFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_uniformNormalFixedValueFvPatchVectorField_H
#define Foam_uniformNormalFixedValueFvPatchVectorField_H

#include "fvPatchFields.H"
#include "fixedValueFvPatchFields.H"
#include "PatchFunction1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
          Class uniformNormalFixedValueFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class uniformNormalFixedValueFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    typedef uniformNormalFixedValueFvPatchVectorField this_bctype;
    typedef fixedValueFvPatchVectorField parent_bctype;

    // Private Data

        //- Function providing the value
        autoPtr<PatchFunction1<scalar>> refValueFunc_;

        //- Optional ramping
        autoPtr<Function1<scalar>> ramp_;


public:

    //- Runtime type information
    TypeNameApi(finiteVolume_API, "uniformNormalFixedValue");


    // Constructors

        //- Construct from patch and internal field
        uniformNormalFixedValueFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformNormalFixedValueFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        uniformNormalFixedValueFvPatchVectorField
        (
            const this_bctype&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        uniformNormalFixedValueFvPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, volMesh>&
        );

        //- No copy without an internal field
        uniformNormalFixedValueFvPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<fvPatchField<vector>> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return fvPatchField<vector>::Clone(*this, iF);
        }


    // Member Functions

        // Mapping Functions

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


        // Evaluation Functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream& os) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
