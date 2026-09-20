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
    Foam::ReitzKHRT

Group
    grpLagrangianIntermediateBreakupSubModels

Description
    Secondary breakup model which uses the Kelvin-Helmholtz
    instability theory to predict the 'stripped' droplets... and
    the Raleigh-Taylor instability as well.

\*---------------------------------------------------------------------------*/

//- Define Foam_ReitzKHRT_defines_typeName in .C files that
//  define the template statics of ReitzKHRT (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_ReitzKHRT_defines_typeName)
#  define Foam_ReitzKHRT_typeName_API
#else
#  define Foam_ReitzKHRT_typeName_API lagrangianSpray_API
#endif

#ifndef ReitzKHRT_H
#define ReitzKHRT_H

#include "BreakupModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                         Class ReitzKHRT Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ReitzKHRT
:
    public BreakupModel<CloudType>
{
private:

    // Private data

        // model constants
        scalar b0_;
        scalar b1_;
        scalar cTau_;
        scalar cRT_;
        scalar msLimit_;
        scalar weberLimit_;


public:

    //- Runtime type information
    TypeNameApi(Foam_ReitzKHRT_typeName_API, "ReitzKHRT");


    // Constructors

        //- Construct from dictionary
        ReitzKHRT(const dictionary&, CloudType&);

        //- Construct copy
        ReitzKHRT(const ReitzKHRT<CloudType>& bum);

        //- Construct and return a clone
        virtual autoPtr<BreakupModel<CloudType>> clone() const
        {
            return autoPtr<BreakupModel<CloudType>>
            (
                new ReitzKHRT<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ReitzKHRT();


    // Member Functions

        //- Update the parcel diameter
        virtual bool update
        (
            const scalar dt,
            const vector& g,
            scalar& d,
            scalar& tc,
            scalar& ms,
            scalar& nParticle,
            scalar& KHindex,
            scalar& y,
            scalar& yDot,
            const scalar d0,
            const scalar rho,
            const scalar mu,
            const scalar sigma,
            const vector& U,
            const scalar rhoc,
            const scalar muc,
            const vector& Urel,
            const scalar Urmag,
            const scalar tMom,
            scalar& dChild,
            scalar& massChild
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "ReitzKHRT.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
