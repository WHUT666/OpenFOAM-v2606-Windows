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
    Foam::VariableHardSphere

Description
    Variable Hard Sphere BinaryCollision Model

\*---------------------------------------------------------------------------*/

//- Define Foam_VariableHardSphere_defines_typeName in .C files that
//  define the template statics of VariableHardSphere (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_VariableHardSphere_defines_typeName)
#  define Foam_VariableHardSphere_typeName_API
#else
#  define Foam_VariableHardSphere_typeName_API DSMC_API
#endif

#ifndef VariableHardSphere_H
#define VariableHardSphere_H

#include "BinaryCollisionModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                      Class VariableHardSphere Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class VariableHardSphere
:
    public BinaryCollisionModel<CloudType>
{
    // Private data

        //- Reference temperature
        const scalar Tref_;


public:

    //- Runtime type information
    TypeNameApi(Foam_VariableHardSphere_typeName_API, "VariableHardSphere");


    // Constructors

        //- Construct from dictionary
        VariableHardSphere
        (
            const dictionary& dict,
            CloudType& cloud
        );


    //- Destructor
    virtual ~VariableHardSphere();


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
    #include "VariableHardSphere.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
