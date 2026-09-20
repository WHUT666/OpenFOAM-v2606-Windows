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
    Foam::GradientDispersionRAS

Description
    The velocity is perturbed in the direction of -grad(k), with a
    Gaussian random number distribution with variance sigma.
    where sigma is defined below

\*---------------------------------------------------------------------------*/

//- Define Foam_GradientDispersionRAS_defines_typeName in .C files that
//  define the template statics of GradientDispersionRAS (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_GradientDispersionRAS_defines_typeName)
#  define Foam_GradientDispersionRAS_typeName_API
#else
#  define Foam_GradientDispersionRAS_typeName_API lagrangianTurbulence_API
#endif

#ifndef GradientDispersionRAS_H
#define GradientDispersionRAS_H

#include "DispersionRASModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class GradientDispersionRAS Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class GradientDispersionRAS
:
    public DispersionRASModel<CloudType>
{
protected:

    // Protected data

        // Locally cached turbulence fields

            //- Gradient of k
            const volVectorField* gradkPtr_;

            //- Take ownership of the grad(k)
            mutable bool ownGradK_;


public:

    //- Runtime type information
    TypeNameApi(Foam_GradientDispersionRAS_typeName_API, "gradientDispersionRAS");


    // Constructors

        //- Construct from components
        GradientDispersionRAS(const dictionary& dict, CloudType& owner);

        //- Construct copy
        GradientDispersionRAS(const GradientDispersionRAS<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType>> clone() const
        {
            return autoPtr<DispersionModel<CloudType>>
            (
                new GradientDispersionRAS<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~GradientDispersionRAS();


    // Member Functions

        //- Cache carrier fields
        virtual void cacheFields(const bool store);

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
    #include "GradientDispersionRAS.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
