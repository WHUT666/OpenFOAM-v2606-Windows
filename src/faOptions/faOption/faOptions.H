/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2025 OpenCFD Ltd.
------------------------------------------------------------------------------
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
    Foam::fa::options

Description
    Finite-area options, which is an IOdictionary of values and
    a fa::optionList

    Possible file locations (default area region):
    \verbatim
    .
    |-- constant/finite-area/faOptions
    |-- system/finite-area/faOptions
    |
    |-- constant/faOptions  (legacy location: OpenFOAM-v2506 and earlier)
    |-- system/faOptions    (legacy location: OpenFOAM-v2506 and earlier)
    \endverbatim

    Possible file locations (multi-region):
    \verbatim
    .
    |-- constant/finite-area/faOptions.<area-name>
    |-- system/finite-area/faOptions.<area-name>
    \endverbatim

    There are no legacy locations when using multiple area regions.

SourceFiles
    faOptions.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_fa_options_H
#define Foam_fa_options_H

#include "faOptionList.H"
#include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fa
{

/*---------------------------------------------------------------------------*\
                        Class options Declaration
\*---------------------------------------------------------------------------*/

class options
:
    public IOdictionary,
    public fa::optionList
{
    // Private Methods

        //- Construct with specified IOobject for the IOdictionary
        //- (somewhat fragile)
        options(const fvMesh& mesh, const IOobject& io, const word& areaName);


public:

    //- Runtime information
    ClassNameApi(faOptions_API, "faOptions");


    // Constructors

        //- Construct from components with list of field names
        explicit options
        (
            const fvMesh& mesh,
            //! The expected finite-area mesh name
            const word& defaultAreaName = word()
        );

        //- No copy construct
        options(const options&) = delete;

        //- No copy assignment
        void operator=(const options&) = delete;


    // Factory Methods

        //- Construct faOptions and register to database if not present
        //- otherwise lookup and return
        static options& New
        (
            const fvMesh& mesh,
            //! The expected finite-area mesh name
            const word& defaultAreaName = word()
        );


    //- Destructor
    virtual ~options() = default;


    // Member Functions

        // Note: areaName() available from fa::optionList

        //- Inherit read from fa::optionList
        using fa::optionList::read;

        //- Read dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
