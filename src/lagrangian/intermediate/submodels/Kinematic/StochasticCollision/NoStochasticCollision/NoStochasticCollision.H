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
    Foam::NoStochasticCollision

Group
    grpLagrangianIntermediateCollisionSubModels

Description
    Dummy collision model for 'none'

\*---------------------------------------------------------------------------*/

//- Define Foam_NoStochasticCollision_defines_typeName in .C files that
//  define the template statics of NoStochasticCollision (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_NoStochasticCollision_defines_typeName)
#  define Foam_NoStochasticCollision_typeName_API
#else
#  define Foam_NoStochasticCollision_typeName_API lagrangianIntermediate_API
#endif

#ifndef NoStochasticCollision_H
#define NoStochasticCollision_H

#include "StochasticCollisionModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                    Class NoStochasticCollision Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class NoStochasticCollision
:
    public StochasticCollisionModel<CloudType>
{
protected:

    // Protected Member Functions

        //- Update the model
        virtual void collide
        (
            typename CloudType::parcelType::trackingData& td,
            const scalar dt
        );


public:

    //- Runtime type information
    TypeNameApi(Foam_NoStochasticCollision_typeName_API, "none");


    // Constructors

        //- Construct from dictionary
        NoStochasticCollision(const dictionary& dict, CloudType& owner);

        //- Construct copy
        NoStochasticCollision(const NoStochasticCollision<CloudType>& cm);

        //- Construct and return a clone
        virtual autoPtr<StochasticCollisionModel<CloudType>> clone() const
        {
            return autoPtr<StochasticCollisionModel<CloudType>>
            (
                new NoStochasticCollision<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~NoStochasticCollision();


    // Member Functions

        //- Flag to indicate whether model activates collision model
        virtual bool active() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "NoStochasticCollision.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
