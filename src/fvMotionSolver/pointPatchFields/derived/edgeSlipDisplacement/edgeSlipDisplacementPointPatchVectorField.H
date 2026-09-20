/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2024 OpenCFD Ltd.
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
    Foam::edgeSlipDisplacementPointPatchVectorField

Description
    Displacement follows an edgeMesh. Use in a displacementMotionSolver
    as a bc on the pointDisplacement field.

    Needs:
    - file : name of edgeMesh. Either:
        - .extendedFeatureEdgeMesh (read from  constant/extendedFeatureEdgeMesh)
        - or .eMesh in or a known format (read from constant/triSurface)
    - frozenPointsZone : empty or name of pointZone containing points
                         that do not move
    - velocity : maximum displacement velocity


SourceFiles
    edgeSlipDisplacementPointPatchVectorField.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_edgeSlipDisplacementPointPatchVectorField_H
#define Foam_edgeSlipDisplacementPointPatchVectorField_H

#include "pointPatchFields.H"
#include "extendedFeatureEdgeMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
          Class edgeSlipDisplacementPointPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class edgeSlipDisplacementPointPatchVectorField
:
    public pointPatchVectorField
{
    typedef edgeSlipDisplacementPointPatchVectorField this_bctype;
    typedef pointPatchVectorField parent_bctype;

    // Private Data

        //- Maximum velocity
        const vector velocity_;

        //- file
        const fileName featFileName_;

        //- pointZone with frozen points
        const word frozenPointsZone_;

        //- Edge searching
        mutable autoPtr<indexedOctree<treeDataEdge>> edgeTreePtr_;


    // Private Member Functions

        //- Calculate displacement (w.r.t. points0()) to project onto surface
        void calcProjection(vectorField& displacement) const;


protected:

        const indexedOctree<treeDataEdge>& edgeTree() const;


public:

    //- Runtime type information
    TypeNameApi(fvMotionSolvers_API, "edgeSlipDisplacement");


    // Constructors

        //- Construct from patch and internal field
        edgeSlipDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        edgeSlipDisplacementPointPatchVectorField
        (
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        edgeSlipDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const pointPatch&,
            const DimensionedField<vector, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        edgeSlipDisplacementPointPatchVectorField
        (
            const this_bctype&,
            const DimensionedField<vector, pointMesh>&
        );

        //- No copy without an internal field
        edgeSlipDisplacementPointPatchVectorField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual autoPtr<pointPatchField<vector>> clone
        (
            const DimensionedField<vector, pointMesh>& iF
        ) const
        {
            return pointPatchField<vector>::Clone(*this, iF);
        }


    // Member Functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Read (& store) geometry. Exposed so point attraction can reuse it.
        static void read
        (
            const objectRegistry& obr,
            const dictionary& dict
        );

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- No copy assignment
        void operator=
        (
            const edgeSlipDisplacementPointPatchVectorField&
        ) = delete;

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
