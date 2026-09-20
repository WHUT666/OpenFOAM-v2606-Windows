/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2019-2025 OpenCFD Ltd.
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
    Foam::fv::options

Description
    Finite-volume options, which is an IOdictionary of values and
    a fv::optionList

    Possible file locations (default region):
    \verbatim
    .
    |-- constant/fvOptions
    |-- system/fvOptions
    \endverbatim

    Possible file locations (multi-region):
    \verbatim
    .
    |-- constant/<region>/fvOptions
    |-- system/<region>/fvOptions
    \endverbatim

SourceFiles
    fvOptions.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_fvOptions_H
#define Foam_fvOptions_H

#include "fvOptionList.H"
#include "IOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                        Class options Declaration
\*---------------------------------------------------------------------------*/

class options
:
    public IOdictionary,
    public fv::optionList
{
    // Private Member Functions

        //- Construct with specified IOobject for the IOdictionary
        //- (somewhat fragile)
        options(const fvMesh& mesh, const IOobject& io);


public:

    // Declare name of the class and its debug switch
    ClassNameApi(finiteVolume_API, "fvOptions");


    // Constructors

        //- Construct from components with list of field names
        explicit options(const fvMesh& mesh);

        //- No copy construct
        options(const options&) = delete;

        //- No copy assignment
        void operator=(const options&) = delete;


    // Factory Methods

        //- Construct fvOptions and register to database if not present
        //- otherwise lookup and return
        static options& New(const fvMesh& mesh);


    //- Destructor
    virtual ~options() = default;


    // Member Functions

        //- Inherit read from fv::optionList
        using fv::optionList::read;

        //- Read dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
