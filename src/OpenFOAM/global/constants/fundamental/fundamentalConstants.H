/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

Description
    Fundamental dimensioned constants

\*---------------------------------------------------------------------------*/

#ifndef fundamentalConstants_H
#define fundamentalConstants_H

#include "dimensionedScalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace constant
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace universal
{
    //- Speed of light in a vacuum
    extern OpenFOAM_API const dimensionedScalar c;

    //- Newtonian constant of gravitation
    extern OpenFOAM_API const dimensionedScalar G;

    //- Planck constant
    extern OpenFOAM_API const dimensionedScalar h;
}

namespace electromagnetic
{
    //- Elementary charge
    extern OpenFOAM_API const dimensionedScalar e;
}

namespace atomic
{
    //- Electron mass
    extern OpenFOAM_API const dimensionedScalar me;

    //- Proton mass
    extern OpenFOAM_API const dimensionedScalar mp;
}

namespace physicoChemical
{
    //- Atomic mass unit
    extern OpenFOAM_API const dimensionedScalar mu;

    //- Avagadro number
    extern OpenFOAM_API const dimensionedScalar NA;

    //- Boltzmann constant
    extern OpenFOAM_API const dimensionedScalar k;
}

//- Standard values
namespace standard
{
    //- Standard pressure
    extern OpenFOAM_API const dimensionedScalar Pstd;

    //- Standard temperature
    extern OpenFOAM_API const dimensionedScalar Tstd;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace constant
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
