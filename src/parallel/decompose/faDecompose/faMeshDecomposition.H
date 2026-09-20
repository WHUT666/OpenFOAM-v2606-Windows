/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2021-2026 OpenCFD Ltd.
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
    Foam::faMeshDecomposition

Description
    Automatic faMesh decomposition class. The decomposition of finite-area
    follows the faceProcAddressing, pointProcAddressing from an existing
    finite-volume decomposition.

Author
    Zeljko Tukovic, FSB Zagreb
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    faMeshDecomposition.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_faMeshDecomposition_H
#define Foam_faMeshDecomposition_H

#include "faMesh.H"

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class faMeshDecomposition Declaration
\*---------------------------------------------------------------------------*/

class faMeshDecomposition
:
    public faMesh
{
    // Private Data

        //- The area-region name
        const word areaName_;

        //- Number of processors in decomposition
        const label nProcs_;

        //- Is the decomposition data to be distributed for each processor
        bool distributed_;

        //- Are globalFaceZones being used (specific to foam-extend)
        bool hasGlobalFaceZones_;

        //- Are there cyclic-parallel edges (not really used...)
        bool cyclicParallel_;

        //- Emit edgeProcAddressing without turning index (2512 and earlier)
        bool noEdgeEncoding_;

        //- The target processor for each face
        labelList faceToProc_;

        //- (per-processor) The mesh faceLabels
        labelListList procFaceLabels_;

        //- (per-processor) Mapping of serial edgeId to local edgeLabel
        List<Map<label>> procMeshEdgesMap_;

        //- (per-processor) The number of internal edges
        labelList procNInternalEdges_;

        //- (per-processor) The edgeLabels for the boundary patches
        List<labelListList> procPatchEdgeLabels_;

        //- (per-processor) Lookup of edgeLabel to serial edgeId
        labelListList procPatchEdgeLookup_;

        //- Labels of points for each processor
        labelListList procPointAddressing_;

        //- Labels of edges for each processor. Includes directional
        //- encoding: (-edge-1) = flipped; (edge+1) = not flipped.
        labelListList procEdgeAddressing_;

        //- Labels of faces for each processor.
        //- No directional encoding
        labelListList procFaceAddressing_;

        //- Original patch index for every processor patch.
        //- An identity map for global boudaries, -1 otherwise.
        labelListList procBoundaryAddressing_;

        //- (per-processor) start/size for regular boundaries
        List<List<labelRange>> procPatchRange_;

        //- (per-processor) start/size for processor boundaries
        List<List<labelRange>> procProcessorPatchRange_;

        //- (per-processor) the neighbProcNo for processor boundaries
        labelListList procNeighbourProcessors_;

        //- List of globally shared point labels
        labelList globallySharedPoints_;


    // Private Static Data

        //- Disable use of edgeProcAddressing turning index
        static faDecompose_API bool disallowEdgeEncoding_;


    // Private Member Functions

        //- Calculate distribution of the finite-area faces.
        //  Uses faMesh::faceLabels() and the finite-volume faceProcAddressing.
        void distributeFaces();

        //- Write the addressing information
        static void writeProcAddressing
        (
            const faMesh& procMesh,
            const labelUList& faceProcAddr,
            const labelUList& edgeProcAddr,
            const labelUList& pointProcAddr,
            const labelUList& boundaryProcAddr,
            bool withoutEdgeEncoding
        );


public:

    // Tuning switches

        //- The enable/disable state for allowing encoding of
        //- edgeProcAddressing edge flips (default: true)
        static bool allowEdgeEncoding() noexcept
        {
            return (!disallowEdgeEncoding_);
        }

        //- Set enable/disable state for allowing edge flip encoding
        static bool allowEdgeEncoding(bool on) noexcept
        {
            bool old(!disallowEdgeEncoding_);
            disallowEdgeEncoding_ = (!on);
            return old;
        }


    // Generated Methods

        //- No copy construct
        faMeshDecomposition(const faMeshDecomposition&) = delete;

        //- No copy assignment
        void operator=(const faMeshDecomposition&) = delete;


    // Constructors

        //- Construct from components.
        //- Values will come from the volume decomposition
        faMeshDecomposition
        (
            //! The area-region name
            const word& areaName,
            //! The underlying polyMesh
            const polyMesh& mesh,
            //! The number of processors
            const label nProcessors,
            //! Additional parameters, sent to updateParameters
            const dictionary& params = dictionary::null
        );

        //- Construct from components (default area region).
        //- Values will come from the volume decomposition
        faMeshDecomposition
        (
            //! The underlying polyMesh
            const polyMesh& mesh,
            //! The number of processors
            const label nProcessors,
            //! Additional parameters, sent to updateParameters
            const dictionary& params = dictionary::null
        );


    //- Destructor
    ~faMeshDecomposition() = default;


    // Member Functions

    // Settings

        //- The area-region name
        const word& name() const noexcept { return areaName_; }

        //- Number of processor in decomposition
        label nProcs() const noexcept { return nProcs_; }

        //- Is decomposition data to be distributed for each processor
        bool distributed() const noexcept { return distributed_; }

        //- Change distributed flag
        bool distributed(bool on) noexcept
        {
            bool old(distributed_);
            distributed_ = on;
            return old;
        }

        //- Is edge encoding disabled
        bool noEdgeEncoding() const noexcept { return noEdgeEncoding_; }

        //- Change the edge encoding flag
        bool noEdgeEncoding(bool on) noexcept
        {
            bool old(noEdgeEncoding_);
            noEdgeEncoding_ = on;
            return old;
        }

        //- Are global face zones used
        bool useGlobalFaceZones() const noexcept
        {
            return hasGlobalFaceZones_;
        }

        //- Change global face zones flag
        bool useGlobalFaceZones(bool on) noexcept
        {
            bool old(hasGlobalFaceZones_);
            hasGlobalFaceZones_ = on;
            return old;
        }

        //- Update flags based on the decomposition model settings
        //  Sets "distributed", detects presence of "globalFaceZones"
        void updateParameters(const dictionary& params);


    // Mappings

        //- Face-processor decomposition labels
        const labelUList& faceToProc() const noexcept
        {
            return faceToProc_;
        }


    // Decompose

        //- Decompose mesh
        void decomposeMesh();

        //- Write decomposition
        bool writeDecomposition() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
