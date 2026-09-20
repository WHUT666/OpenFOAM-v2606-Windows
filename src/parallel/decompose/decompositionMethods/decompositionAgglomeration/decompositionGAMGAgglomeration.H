/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2023 M. Janssens
    Copyright (C) 2025 OpenCFD Ltd.
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
    Foam::decompositionGAMGAgglomeration

Description
    Agglomerate using the decomposition algorithm.

    Can be used to produce quicker coarsening.

    Method coefficients:
    \table
        Property  | Description                             | Required | Default
        method     | name of decompositionMethod            | yes |
        numberOfSubdomains | n                              | yes |
        forceConnected | (bool) split disconnected region   | no  | true (GIVE WARNING)
        renumber   | (bool) renumber individual levels      | no  | false
    \endtable


SourceFiles
    decompositionGAMGAgglomeration.C

\*---------------------------------------------------------------------------*/

#ifndef decompositionGAMGAgglomeration_H
#define decompositionGAMGAgglomeration_H

#include "GAMGAgglomeration.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class decompositionMethod;

/*---------------------------------------------------------------------------*\
                 Class decompositionGAMGAgglomeration Declaration
\*---------------------------------------------------------------------------*/

class decompositionGAMGAgglomeration
:
    public GAMGAgglomeration
{
    // Private Data

        const bool doRenumber_;

        const bool forceConnected_;

        mutable autoPtr<decompositionMethod> decomposerPtr_;

        const label clusterSize_;

        mutable bool hasWarned_;


    // Private Member Functions

        template<class Type>
        static tmp<Field<Type>> avg
        (
            const UList<Type>& fld,
            const label nCoarse,
            const labelUList& oldToNew
        );

        //- Agglomerate single level
        tmp<labelField> agglomerate
        (
            const bool partByPart,
            const label nCoarseCells,

            // Current mesh
            const lduMesh& mesh,
            const pointField& cellCentres,
            const scalarField& cellWeights,
            const scalarField& faceWeights,

            // Agglomeration of current mesh
            const labelUList& region
        ) const;

        static bitSet blockedFaces
        (
            const lduAddressing& addr,
            const labelUList& region        // region per cell
        );

        static bool checkRestriction
        (
            labelList& newRestrict,
            label& nNewCoarse,
            const lduAddressing& fineAddressing,
            const labelUList& restriction,
            const label nCoarse
        );

        static void coarseCellCells
        (
            const lduAddressing& addr,
            const labelList& regions,           // coarse cell
            CompactListList<label>& cellCells   // coarse-to-coarse cells
        );


public:

    //- Runtime type information
    TypeNameApi(decompositionMethods_API, "decomposition");


    // Constructors

        //- Construct given mesh and controls
        decompositionGAMGAgglomeration
        (
            const lduMesh& mesh,
            const dictionary& controlDict
        );

        //- Construct given mesh and controls
        decompositionGAMGAgglomeration
        (
            const lduMesh& mesh,
            const scalarField& cellVolumes,
            const vectorField& faceAreas,
            const dictionary& controlDict
        );


    // Member Functions

        //- Agglomerate from a starting level. Starting level is usually 0
        //- (initial mesh) but sometimes >0 (restarting after processor
        //- agglomeration)
        virtual void agglomerate
        (
            const label nCellsInCoarsestLevel,
            const label startLevel,
            const scalarField& startFaceWeights,
            const bool doProcessorAgglomerate = true
        );


    // Helpers

        static void localCellCells
        (
            const lduAddressing& addr,
            const bitSet& isBlockedFace,
            CompactListList<label>& cellCells
        );
        static labelList localCellCells
        (
            const lduAddressing& addr,
            const labelList& regions,   // marker
            const label regioni,        // which marker to keep
            CompactListList<label>& cellCells
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "decompositionGAMGAgglomerationTemplates.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
