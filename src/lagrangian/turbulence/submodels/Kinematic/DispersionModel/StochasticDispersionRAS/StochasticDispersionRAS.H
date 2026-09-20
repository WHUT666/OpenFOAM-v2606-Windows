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
    Foam::StochasticDispersionRAS

Description
    The velocity is perturbed in random direction, with a
    Gaussian random number distribution with variance sigma.
    where sigma is defined below

\*---------------------------------------------------------------------------*/

//- Define Foam_StochasticDispersionRAS_defines_typeName in .C files that
//  define the template statics of StochasticDispersionRAS (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_StochasticDispersionRAS_defines_typeName)
#  define Foam_StochasticDispersionRAS_typeName_API
#else
#  define Foam_StochasticDispersionRAS_typeName_API lagrangianTurbulence_API
#endif

#ifndef StochasticDispersionRAS_H
#define StochasticDispersionRAS_H

#include "DispersionRASModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class StochasticDispersionRAS Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class StochasticDispersionRAS
:
    public DispersionRASModel<CloudType>
{
public:

    //- Runtime type information
    TypeNameApi(Foam_StochasticDispersionRAS_typeName_API, "stochasticDispersionRAS");


    // Constructors

        //- Construct from components
        StochasticDispersionRAS(const dictionary& dict, CloudType& owner);

        //- Construct copy
        StochasticDispersionRAS(const StochasticDispersionRAS<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType>> clone() const
        {
            return autoPtr<DispersionModel<CloudType>>
            (
                new StochasticDispersionRAS<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~StochasticDispersionRAS();


    // Member Functions

        //- Update (disperse particles)
        virtual vector update
        (
            const scalar dt,
            const label celli,
            const vector& U,
            const vector& Uc,
            vector& UTurb,
            scalar& tTurb
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "StochasticDispersionRAS.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
