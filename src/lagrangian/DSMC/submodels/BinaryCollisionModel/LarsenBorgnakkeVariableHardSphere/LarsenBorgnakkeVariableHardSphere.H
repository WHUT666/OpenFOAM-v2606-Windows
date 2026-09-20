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
    Foam::LarsenBorgnakkeVariableHardSphere

Description
    Variable Hard Sphere BinaryCollision Model with Larsen Borgnakke internal
    energy redistribution.  Based on the INELRS subroutine in Bird's DSMC0R.FOR

\*---------------------------------------------------------------------------*/

//- Define Foam_LarsenBorgnakkeVariableHardSphere_defines_typeName in .C files that
//  define the template statics of LarsenBorgnakkeVariableHardSphere (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_LarsenBorgnakkeVariableHardSphere_defines_typeName)
#  define Foam_LarsenBorgnakkeVariableHardSphere_typeName_API
#else
#  define Foam_LarsenBorgnakkeVariableHardSphere_typeName_API DSMC_API
#endif

#ifndef LarsenBorgnakkeVariableHardSphere_H
#define LarsenBorgnakkeVariableHardSphere_H

#include "BinaryCollisionModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                      Class LarsenBorgnakkeVariableHardSphere Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LarsenBorgnakkeVariableHardSphere
:
    public BinaryCollisionModel<CloudType>
{
    // Private data

        //- Reference temperature
        const scalar Tref_;

        //- Relaxation collision number
        const scalar relaxationCollisionNumber_;


    // Private Member Functions

        //- Calculate the energy ratio for distribution to internal degrees of
        // freedom
        scalar energyRatio
        (
            scalar ChiA,
            scalar ChiB
        );


public:

    //- Runtime type information
    TypeNameApi(Foam_LarsenBorgnakkeVariableHardSphere_typeName_API, "LarsenBorgnakkeVariableHardSphere");


    // Constructors

        //- Construct from dictionary
        LarsenBorgnakkeVariableHardSphere
        (
            const dictionary& dict,
            CloudType& cloud
        );


    //- Destructor
    virtual ~LarsenBorgnakkeVariableHardSphere();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;

        //- Return the collision cross section * relative velocity product
        virtual scalar sigmaTcR
        (
            const typename CloudType::parcelType& pP,
            const typename CloudType::parcelType& pQ
        ) const;

        //- Apply collision
        virtual void collide
        (
            typename CloudType::parcelType& pP,
            typename CloudType::parcelType& pQ
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "LarsenBorgnakkeVariableHardSphere.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
