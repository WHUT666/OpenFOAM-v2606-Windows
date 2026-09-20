/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2022-2025 OpenCFD Ltd.
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
    Foam::regionModels::areaSurfaceFilmModels::regionFaIBM

Description
    Immersed Boundary Method (IBM) for finite area (FA). Multiple IBM surfaces
    can be defined, each represented by a triangulated surface mesh.
    Boundary motion is supported via a solidBodyMotionFunction.

Usage
    Example usage in constant/regionFaIBMPropeties

    \verbatim
    regionFaIBM1
    {
        type            regionFaIBM;
        surface         <surfaceFileName>;

        solidBodyMotionFunction <motionType>
        // Additional motion type information...

        // Optional

        // Solve type for stencil weights; default = direct
        solveType       direct; // or invDistance

        // Stencil radius factor; default = 1.5
        rFactor         2.0;
    }
    regionFaIBM2
    {
        ...
    }
    \endverbatim


SeeAlso
    Foam::solidBodyMotionFunction

SourceFiles
    regionFaIBM.cxx
    regionFaIBM.txx

\*---------------------------------------------------------------------------*/

#ifndef Foam_FA_regionFaIBM_H
#define Foam_FA_regionFaIBM_H

#include "areaFields.H"
#include "triSurfaceMesh.H"
#include "indexedOctree.H"
#include "solidBodyMotionFunction.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class Time;

namespace regionModels
{
namespace areaSurfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                         Class regionFaIBM Declaration
\*---------------------------------------------------------------------------*/

class regionFaIBM
{
public:

    // Data Types

        //- Solve type for stencil weights
        enum class solveType
        {
            invDistance, //<! Inverse distance weighting
            direct       //<! Direct solve
        };


    // Member Functions

        //- Dump the stencil to file
        static void dumpStencil
        (
            const fileName& fname,
            const mapDistribute& mapDist,
            const labelListList& stencil,
            const pointField& centres
        );


protected:

    // Protected Data

        typedef treeDataPrimitivePatch<uindirectPrimitivePatch> treeType;

        //- Selection names for solveType
        static regionFaModels_API const Enum<solveType> solveTypeNames;


        //- Reference to the finite area mesh
        const faMesh& aMesh_;

        //- regionFaIBM surface described by a triSurfaceMesh
        //  Note: this should be a closed surface since we perform point
        //  inside/outside tests
        triSurfaceMesh surface_;

        //- Reference points for surface motion
        const pointField points0_;

        //- Parallel map to apply the regionFaIBM stencil
        autoPtr<mapDistribute> mapPtr_;

        //- Surface motion as a function of time
        autoPtr<solidBodyMotionFunction> SBMFPtr_;

        //- Octree for face search
        autoPtr<indexedOctree<treeType>> treePtr_;

        //- Integration time step
        scalar deltaT_;

        //- Time at last update
        scalar updateTimeOld_;

        //- Last transformation
        septernion transformOld_;

        //- Transformation to go to state at last step
        septernion invTransformDelta_;

        //- Face types indicator field
        labelField faceTypes_;

        //- Forcing faces
        labelField forcingFaceIDs_;

        //- Inside faces
        labelField insideFaceIDs_;

        //- Solve type
        solveType solveType_;

        //- Stencil radius factor
        scalar rFactor_;

        //- Interpolation stencil
        labelListList stencil_;

        //- Interpolation weights
        scalarListList weights_;

        //- Distance weights used when calculating uStar
        scalarList h_;


    // Protected Member Functions

        //- Move the immersed boundary
        bool moveBody();

        //- Set the face types indicator field and derived info
        void setFaceTypes();

        //- Retrieve the velocity at a point on the moving surface
        vector setVelocityFromSurface(const pointIndexHit& hit) const;

        //- Set the regionFaIBM stencil addressing
        void setStencilAddressing
        (
            const pointField& forcingFaceCentres,
            const scalarField& forcingFaceAreas,
            pointField& mirrorPoints
        );

        //- Set the regionFaIBM stencil weights
        void setStencilWeights(const pointField& mirrorPoints);


public:

    //- Runtime type information
    TypeNameApi(regionFaModels_API, "IBM");

    // Constructors

        //- Construct from Time and dictionary
        regionFaIBM
        (
            const Time& runTime,
            const faMesh& aMesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~regionFaIBM() = default;


    // Member Functions

        //- Update mesh
        void updateMesh();

        //- Correct the regionFaIBM model
        bool correct(const areaVectorField& Us, areaVectorField& Ustar);

        //- Apply the zero filter to an area field
        template<class Type>
        void zeroFilter(Type& fld) const;

        //- Add to mask; Sets a value of 1 for participating faces
        void addToMask(areaScalarField& mask) const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "regionFaIBM.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace areaSurfaceFilmModels
} // End namespace regionModels
} // End namespace Foam

// ************************************************************************* //

#endif

// ************************************************************************* //
