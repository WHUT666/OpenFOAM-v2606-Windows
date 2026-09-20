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
    Foam::waveDisplacementPointPatchVectorField

Description
    Foam::waveDisplacementPointPatchVectorField

SourceFiles
    waveDisplacementPointPatchVectorField.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_waveDisplacementPointPatchVectorField_H
#define Foam_waveDisplacementPointPatchVectorField_H

#include "fixedValuePointPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
        Class waveDisplacementPointPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class waveDisplacementPointPatchVectorField
:
    public fixedValuePointPatchField<vector>
{
    typedef waveDisplacementPointPatchVectorField this_bctype;
    typedef fixedValuePointPatchField<vector> parent_bctype;

    // Private Data

        vector amplitude_;
        scalar omega_;
        vector waveNumber_;


public:

    //- Runtime type information
    TypeNameApi(fvMotionSolvers_API, "waveDisplacement");


    // Constructors

        //- Construct from patch and internal field
        waveDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        waveDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        waveDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        waveDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, pointMesh>&
        );

        //- No copy without an internal field
        waveDisplacementPointPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<vector>> clone
        (
            const DimensionedField<vector, pointMesh>& iF
        ) const
        {
            return pointPatchField<vector>::Clone(*this, iF);
        }


    // Member functions

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
