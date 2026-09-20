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
    Foam::DevolatilisationModel

Group
    grpLagrangianIntermediateDevolatilisationSubModels

Description
    Templated devolatilisation model class

SourceFiles
    DevolatilisationModel.C
    DevolatilisationModelNew.C

\*---------------------------------------------------------------------------*/

//- Define Foam_DevolatilisationModel_defines_typeName in .C files that
//  define the template statics of DevolatilisationModel (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_DevolatilisationModel_defines_typeName)
#  define Foam_DevolatilisationModel_typeName_API
#else
#  define Foam_DevolatilisationModel_typeName_API lagrangianIntermediate_API
#endif

#ifndef DevolatilisationModel_H
#define DevolatilisationModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "CloudSubModelBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class DevolatilisationModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DevolatilisationModel
:
    public CloudSubModelBase<CloudType>
{
protected:

    // Protected data

        //- Mass of lagrangian phase converted
        scalar dMass_;


public:

    //- Runtime type information
    TypeNameApi(Foam_DevolatilisationModel_typeName_API, "devolatilisationModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTableApi
    ( Foam_DevolatilisationModel_typeName_API,
        autoPtr,
        DevolatilisationModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        DevolatilisationModel(CloudType& owner);

        //- Construct from dictionary
        DevolatilisationModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        DevolatilisationModel(const DevolatilisationModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DevolatilisationModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~DevolatilisationModel();


    //- Selector
    static autoPtr<DevolatilisationModel<CloudType>> New
    (
        const dictionary& dict,
        CloudType& owner
    );


    // Member Functions

        //- Update model
        virtual void calculate
        (
            const scalar dt,
            const scalar age,
            const scalar mass0,
            const scalar mass,
            const scalar T,
            const scalarField& YGasEff,
            const scalarField& YLiquidEff,
            const scalarField& YSolidEff,
            label& canCombust,
            scalarField& dMassDV
        ) const = 0;

        //- Add to devolatilisation mass
        void addToDevolatilisationMass(const scalar dMass);


        // I-O

            //- Write injection info
            virtual void info();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDevolatilisationModel(CloudType)                                   \
                                                                               \
    typedef Foam::CloudType::reactingMultiphaseCloudType                       \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
    (                                                                          \
        Foam::DevolatilisationModel<reactingMultiphaseCloudType>,              \
        0                                                                      \
    );                                                                         \
    namespace Foam                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            DevolatilisationModel<reactingMultiphaseCloudType>,                \
            dictionary                                                         \
        );                                                                     \
    }


#define makeDevolatilisationModelType(SS, CloudType)                           \
                                                                               \
    typedef Foam::CloudType::reactingMultiphaseCloudType                       \
        reactingMultiphaseCloudType;                                           \
    defineNamedTemplateTypeNameAndDebug                                        \
        (Foam::SS<reactingMultiphaseCloudType>, 0);                            \
                                                                               \
    Foam::DevolatilisationModel<reactingMultiphaseCloudType>::                 \
        adddictionaryConstructorToTable                                        \
        <Foam::SS<reactingMultiphaseCloudType>>                                \
        add##SS##CloudType##reactingMultiphaseCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "DevolatilisationModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
