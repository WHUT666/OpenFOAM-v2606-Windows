/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2019 OpenCFD Ltd.
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
    Foam::LocalInteraction

Group
    grpLagrangianIntermediatePatchInteractionSubModels

Description
    Patch interaction specified on a patch-by-patch basis

\*---------------------------------------------------------------------------*/

//- Define Foam_LocalInteraction_defines_typeName in .C files that
//  define the template statics of LocalInteraction (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_LocalInteraction_defines_typeName)
#  define Foam_LocalInteraction_typeName_API
#else
#  define Foam_LocalInteraction_typeName_API lagrangianIntermediate_API
#endif

#ifndef LocalInteraction_H
#define LocalInteraction_H

#include "BaiGosman.H"
#include "PatchInteractionModel.H"
#include "patchInteractionDataList.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class LocalInteraction Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LocalInteraction
:
    public PatchInteractionModel<CloudType>
{
    // Friends

        friend class localInteractionModels::BaiGosman<CloudType>;


    // Private data

        //- List of participating patches
        const patchInteractionDataList patchData_;


        // Bookkeeping for particle fates

            //- Number of parcels escaped
            List<List<label>> nEscape_;

            //- Mass of parcels escaped
            List<List<scalar>> massEscape_;

            //- Number of parcels stuck to patches
            List<List<label>> nStick_;

            //- Mass of parcels stuck to patches
            List<List<scalar>> massStick_;

            //- Number of parcels rebounded from patches
            List<List<label>> nRebound_;

            //- Mass of parcels rebounded from patches
            List<List<scalar>> massRebound_;

            //- Number of parcels splashed from patches
            List<List<label>> nSplash_;

            //- Mass of parcels splashed from patches
            List<List<scalar>> massSplash_;


        //- Flag to output data as fields
        bool writeFields_;

        //- True if any patch entry uses the BaiGosman interaction type
        bool hasBaiGosman_;

        //- Internal BaiGosman thermo patch interaction
        localInteractionModels::BaiGosman<CloudType> beiGosman_;

        //- InjectorId to index map, when outputting escaped/stick/...
        //  particles sorted by injectorID
        Map<label> injIdToIndex_;

        //- Mass escape field
        autoPtr<volScalarField> massEscapePtr_;

        //- Mass stick field
        autoPtr<volScalarField> massStickPtr_;


    // Private Member Functions

        //- True if any patch data entry is a BaiGosman interaction
        bool hasBaiGosmanPatch() const;

        //- Add escape stats and optional field output
        void addEscapeCounters
        (
            const label patchi,
            const label idx,
            const scalar mass,
            const polyPatch& pp,
            const label facei
        );

        //- Add stick stats and optional field output
        void addStickCounters
        (
            const label patchi,
            const label idx,
            const scalar mass,
            const polyPatch& pp,
            const label facei,
            const bool incrementCount = true
        );

        //- Add rebound stats
        void addReboundCounters
        (
            const label patchi,
            const label idx,
            const scalar mass
        );

        //- Add splash stats
        void addSplashCounters
        (
            const label patchi,
            const label idx,
            const scalar mass
        );


protected:

    // Protected Member Functions

        //- Output file header information
        virtual void writeFileHeader(Ostream& os);


public:

    //- Runtime type information
    TypeNameApi(Foam_LocalInteraction_typeName_API, "localInteraction");


    // Constructors

        //- Construct from dictionary
        LocalInteraction(const dictionary& dict, CloudType& owner);

        //- Construct copy from owner cloud and patch interaction model
        LocalInteraction(const LocalInteraction<CloudType>& pim);

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<PatchInteractionModel<CloudType>> clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType>>
            (
                new LocalInteraction<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LocalInteraction() = default;


    // Member Functions

        //- Return access to the massEscape field
        volScalarField& massEscape();

        //- Return access to the massStick field
        volScalarField& massStick();

        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        );


        //- Write patch interaction info
        virtual void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "LocalInteraction.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
