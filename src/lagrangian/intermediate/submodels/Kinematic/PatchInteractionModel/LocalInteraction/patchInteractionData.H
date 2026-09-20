/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
    Copyright (C) 2021 OpenCFD Ltd.
    Copyright (C) 2026 Keysight Technologies
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
    Foam::patchInteractionData

Description
    Helper class for the LocalInteraction patch interaction model

\*---------------------------------------------------------------------------*/

#ifndef patchInteractionData_H
#define patchInteractionData_H

#include "Switch.H"
#include "label.H"
#include "scalar.H"
#include "wordRe.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declaration
class patchInteractionData;
Istream& operator>>(Istream& is, patchInteractionData& pid);
Ostream& operator<<(Ostream& os, const patchInteractionData& pid);

/*---------------------------------------------------------------------------*\
                    Class patchInteractionData Declaration
\*---------------------------------------------------------------------------*/

class patchInteractionData
{
    // Private Data

        //- Interaction type name
        word interactionTypeName_;

        //- Patch name(s)
        wordRe patchName_;

        //- Elasticity coefficient
        scalar e_;

        //- Restitution coefficient
        scalar mu_;


    // Bei-Gosman Model Parameters

        //- Flag to select dry or wet wall assumption
        Switch dry_;

        //- Number of splash parcels
        label parcelsPerSplash_;

        //- Override splash parcel typeId
        label splashParcelType_;

        //- Wet film threshold [m]
        scalar deltaWet_;

        //- Splash parcel minimum diameter [m]
        scalar dMinSplash_;

        //- Splash parcel maximum diameter [m]
        scalar dMaxSplash_;

        //- Dry roughness coefficient
        scalar Adry_;

        //- Wet roughness coefficient
        scalar Awet_;

        //- Skin friction coefficient
        scalar Cf_;

        //- Reference temperature [K]
        scalar Tref_;

        //- Melt/transition temperature [K]
        scalar Tmelt_;

        //- Critical Weber number for initial regime gate
        scalar Wec_;


public:

    // Public Data

        //- BaiGosman type keyword used in patch entries
        static lagrangianIntermediate_API const word beiGosmanTypeName;


    // Constructor

        //- Default construct
        patchInteractionData();


    // Member Functions

        //- The interaction type name
        const word& interactionTypeName() const noexcept
        {
            return interactionTypeName_;
        }

        //- Is this entry a BaiGosman interaction
        bool isBaiGosman() const noexcept
        {
            return interactionTypeName_ == beiGosmanTypeName;
        }

        //- The patch name(s)
        const wordRe& patchName() const noexcept
        {
            return patchName_;
        }

        //- The elasticity coefficient
        scalar e() const noexcept
        {
            return e_;
        }

        //- The restitution coefficient
        scalar mu() const noexcept
        {
            return mu_;
        }


    // Bei-Gosman Functions

        //- Return dry wall switch
        const Switch& dry() const noexcept { return dry_; }

        //- Return number of splash parcels
        label parcelsPerSplash() const noexcept { return parcelsPerSplash_; }

        //- Return splash parcel type override
        label splashParcelType() const noexcept { return splashParcelType_; }

        //- Return wet film threshold [m]
        scalar deltaWet() const noexcept { return deltaWet_; }

        //- Return minimum splash diameter [m]
        scalar dMinSplash() const noexcept { return dMinSplash_; }

        //- Return maximum splash diameter [m]
        scalar dMaxSplash() const noexcept { return dMaxSplash_; }

        //- Return dry roughness coefficient
        scalar Adry() const noexcept { return Adry_; }

        //- Return wet roughness coefficient
        scalar Awet() const noexcept { return Awet_; }

        //- Return skin friction coefficient
        scalar Cf() const noexcept { return Cf_; }

        //- Return reference temperature [K]
        scalar Tref() const noexcept { return Tref_; }

        //- Return melt temperature [K]
        scalar Tmelt() const noexcept { return Tmelt_; }

        //- Return critical Weber number
        scalar Wec() const noexcept { return Wec_; }


    // IO Operators

        //- Istream operator
        friend Istream& operator>>(Istream& is, patchInteractionData& pid);

        //- Ostream operator
        friend Ostream& operator<<(Ostream& os, const patchInteractionData& pid);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
