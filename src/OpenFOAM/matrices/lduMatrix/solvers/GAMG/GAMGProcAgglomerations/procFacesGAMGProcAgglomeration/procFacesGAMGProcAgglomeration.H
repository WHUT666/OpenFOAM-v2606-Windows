/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013 OpenFOAM Foundation
    Copyright (C) 2023 OpenCFD Ltd.
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
    Foam::procFacesGAMGProcAgglomeration

Description
    Processor agglomeration of GAMGAgglomerations.

    Combines processors into master processors by agglomerating across
    processor boundaries. Works in one of two modes:

    - if nMasters (or nProcessorsPerMaster) is given, agglomerate the coarsest
    level until that number of processors (or cores per master) is reached.
    This is very much like
    masterCoarsestGAMGProcAgglomeration but agglomerating only connected
    procesors.
    - if nAgglomeratingCells is given, agglomerate all levels once
    until the number of cells
    on any processor is below that number.

    E.g.

    p
    {
        solver          GAMG;
        ...

        processorAgglomerator   procFaces;
        // Agglomerate until 4 master processors are reached
        nMasters                4;
    }

    Implementation: processors get agglomerated
    by constructing an equivalent lduMesh where each processor is a cell
    and each processorInterface is a face. This mesh then gets agglomerated
    using the standard pairGAMGAgglomeration algorithm with the number of faces
    on the original processor interface as face weight. The nMasters controls
    the number of pair-wise agglomeration ('mergeLevels').

SourceFiles
    procFacesGAMGProcAgglomeration.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_procFacesGAMGProcAgglomeration_H
#define Foam_procFacesGAMGProcAgglomeration_H

#include "GAMGProcAgglomeration.H"
#include "labelField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class procFacesGAMGProcAgglomeration Declaration
\*---------------------------------------------------------------------------*/

class procFacesGAMGProcAgglomeration
:
    public GAMGProcAgglomeration
{
    // Private Data

        //- When to processor agglomerate
        label nAgglomeratingCells_;

        //- Number of master processors
        label nMasters_;

        //- Optional : restart local agglomeration
        const label nCellsInMasterLevel_;


    // Private Member Functions

        //- Helper: construct coarse level lduMesh. (copied from
        //  GAMGAgglomerateLduAddressing but adapted for lduPrimitiveMesh)
        static autoPtr<lduPrimitiveMesh> agglomerateLduAddressing
        (
            const lduPrimitiveMesh& fineMesh,
            const labelUList& restrictMap,
            label& nCoarseFaces,
            labelList& faceRestrictAddr,
            boolList& faceFlipMap
        );

        //- Return (on master) an lduMesh with each processor a cell, each
        //- processor interface a face.
        autoPtr<lduPrimitiveMesh> singleCellMesh
        (
            const label singleCellMeshComm,
            const lduMesh& mesh,
            scalarField& faceWeights
        ) const;

        //- Construct processor agglomeration: for every processor the
        //  coarse processor-cluster it agglomerates onto. nCoarsest
        //  is the number of coarsest processors (labelMax if only one level)
        tmp<labelField> processorAgglomeration
        (
            const lduMesh& mesh,
            const label nCoarsest
        ) const;

        //- Do we need to agglomerate across processors?
        bool doProcessorAgglomeration(const lduMesh&) const;

        //- Agglomerate the given level. If nCoarsest is given, agglomerate
        //  until that number of processors is reached. Otherwise, do a single
        //  agglomeration step.
        bool agglomerate
        (
            const label fineLevelIndex,
            const label nCoarsest
        );


public:

    //- Runtime type information
    TypeNameApi(OpenFOAM_API, "procFaces");

    //- No copy construct
    procFacesGAMGProcAgglomeration(const procFacesGAMGProcAgglomeration&)
        = delete;

    //- No copy assignment
    void operator=(const procFacesGAMGProcAgglomeration&) = delete;


    // Constructors

        //- Construct given agglomerator and controls
        procFacesGAMGProcAgglomeration
        (
            GAMGAgglomeration& agglom,
            const dictionary& controlDict
        );


    //- Destructor
    virtual ~procFacesGAMGProcAgglomeration() = default;


    // Member Functions

        //- Modify agglomeration. Return true if modified
        virtual bool agglomerate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
