/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::regionModels::areaSurfaceFilmModels::kinematicThinFilmIBM

Description
    Thin film model with support for immersed boundary method (IBM)

Usage
    Example of the boundary condition specification:
    \verbatim
    {
        // Mandatory entries
        liquidFilmModel         kinematicThinFilmIBM;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property   | Description                           | Type  | Reqd | Deflt
      liquidFilmModel  | Type name: kinematicThinFilmIBM | dict  | yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link liquidFilmModel.H \endlink

SourceFiles
    kinematicThinFilmIBM.C

\*---------------------------------------------------------------------------*/

#ifndef areaSurfaceFilmModels_kinematicThinFilmIBM_H
#define areaSurfaceFilmModels_kinematicThinFilmIBM_H

#include "kinematicThinFilm.H"
#include "regionFaIBMList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace regionModels
{
namespace areaSurfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                    Class kinematicThinFilmIBM Declaration
\*---------------------------------------------------------------------------*/

class kinematicThinFilmIBM
:
    public kinematicThinFilm
{
protected:

    // Protected Data

        //- Immersed boundaries
        regionFaIBMList immersedBoundary_;


public:

    //- Runtime type information
    TypeNameApi(regionFaModels_API, "kinematicThinFilmIBM");


    // Constructors

        //- Construct from components and dict
        kinematicThinFilmIBM
        (
            const word& modelType,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- No copy construct
        kinematicThinFilmIBM(const kinematicThinFilmIBM&) = delete;

        //- No copy assignment
        void operator=(const kinematicThinFilmIBM&) = delete;


    //- Destructor
    virtual ~kinematicThinFilmIBM() = default;


    // Member Functions

        // Evolution

            //- Pre-evolve film
            virtual void preEvolveRegion();

            //- Evolve the film
            virtual void evolveRegion();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace areaSurfaceFilmModels
} // End namespace regionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
