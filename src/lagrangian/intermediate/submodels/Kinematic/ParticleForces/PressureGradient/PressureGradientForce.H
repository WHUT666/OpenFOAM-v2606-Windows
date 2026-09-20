/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
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
    Foam::PressureGradientForce

Group
    grpLagrangianIntermediateForceSubModels

Description
    Calculates particle pressure gradient force

SourceFiles
    PressureGradientForceI.H
    PressureGradientForce.C

\*---------------------------------------------------------------------------*/

//- Define Foam_PressureGradientForce_defines_typeName in .C files that
//  define the template statics of PressureGradientForce (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_PressureGradientForce_defines_typeName)
#  define Foam_PressureGradientForce_typeName_API
#else
#  define Foam_PressureGradientForce_typeName_API lagrangianIntermediate_API
#endif

#ifndef PressureGradientForce_H
#define PressureGradientForce_H

#include "ParticleForce.H"
#include "volFields.H"
#include "interpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class PressureGradientForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PressureGradientForce
:
    public ParticleForce<CloudType>
{
protected:

    // Protected data

        //- Name of velocity field
        const word UName_;

        //- Rate of change of carrier phase velocity interpolator
        autoPtr<interpolation<vector>> DUcDtInterpPtr_;


public:

    //- Runtime type information
    TypeNameApi(Foam_PressureGradientForce_typeName_API, "pressureGradient");


    // Constructors

        //- Construct from mesh
        PressureGradientForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict,
            const word& forceType = typeName
        );

        //- Construct copy
        PressureGradientForce(const PressureGradientForce& pgf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new PressureGradientForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PressureGradientForce();


    // Member Functions

        // Access

            //- Return the rate of change of carrier phase velocity interpolator
            inline const interpolation<vector>& DUcDtInterp() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;

            //- Return the added mass
            virtual scalar massAdd
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar mass
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "PressureGradientForceI.H"

#ifdef NoRepository
    #include "PressureGradientForce.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
