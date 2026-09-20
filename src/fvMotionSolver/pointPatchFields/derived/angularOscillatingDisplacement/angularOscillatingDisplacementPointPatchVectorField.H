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
    Foam::angularOscillatingDisplacementPointPatchVectorField

Description
    Foam::angularOscillatingDisplacementPointPatchVectorField

SourceFiles
    angularOscillatingDisplacementPointPatchVectorField.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_angularOscillatingDisplacementPointPatchVectorField_H
#define Foam_angularOscillatingDisplacementPointPatchVectorField_H

#include "fixedValuePointPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
     Class angularOscillatingDisplacementPointPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class angularOscillatingDisplacementPointPatchVectorField
:
    public fixedValuePointPatchField<vector>
{
    typedef angularOscillatingDisplacementPointPatchVectorField this_bctype;
    typedef fixedValuePointPatchField<vector> parent_bctype;

    // Private Data

        vector axis_;
        vector origin_;
        scalar angle0_;
        scalar amplitude_;
        scalar omega_;

        pointField p0_;


public:

    //- Runtime type information
    TypeNameApi(fvMotionSolvers_API, "angularOscillatingDisplacement");


    // Constructors

        //- Construct from patch and internal field
        angularOscillatingDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        angularOscillatingDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        angularOscillatingDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        angularOscillatingDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, pointMesh>&
        );

        //- No copy without an internal field
        angularOscillatingDisplacementPointPatchVectorField
        (
            const this_bctype&
        ) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<vector>> clone
        (
            const DimensionedField<vector, pointMesh>& iF
        ) const
        {
            return pointPatchField<vector>::Clone(*this, iF);
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const pointPatchFieldMapper&
            );

            //- Reverse map the given pointPatchField onto this pointPatchField
            virtual void rmap
            (
                const pointPatchField<vector>&,
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
