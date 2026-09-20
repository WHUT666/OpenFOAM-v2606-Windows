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
    Foam::DispersionModel

Group
    grpLagrangianIntermediateDispersionSubModels

Description
    Base class for dispersion modelling

\*---------------------------------------------------------------------------*/

//- Define Foam_DispersionModel_defines_typeName in .C files that
//  define the template statics of DispersionModel (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_DispersionModel_defines_typeName)
#  define Foam_DispersionModel_typeName_API
#else
#  define Foam_DispersionModel_typeName_API lagrangianIntermediate_API
#endif

#ifndef DispersionModel_H
#define DispersionModel_H

#include "IOdictionary.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "CloudSubModelBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class DispersionModel Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class DispersionModel
:
    public CloudSubModelBase<CloudType>
{
public:

    //- Runtime type information
    TypeNameApi(Foam_DispersionModel_typeName_API, "dispersionModel");


    // Declare runtime constructor selection table

    declareRunTimeSelectionTableApi
    ( Foam_DispersionModel_typeName_API,
        autoPtr,
        DispersionModel,
        dictionary,
        (
            const dictionary& dict,
            CloudType& owner
        ),
        (dict, owner)
    );


    // Constructors

        //- Construct null from owner
        DispersionModel(CloudType& owner);

        //- Construct from components
        DispersionModel
        (
            const dictionary& dict,
            CloudType& owner,
            const word& type
        );

        //- Construct copy
        DispersionModel(const DispersionModel<CloudType>& dm);

        //- Construct and return a clone
        virtual autoPtr<DispersionModel<CloudType>> clone() const = 0;


    //- Destructor
    virtual ~DispersionModel();


    //- Selector
    static autoPtr<DispersionModel<CloudType>> New
    (
        const dictionary& dict,
        CloudType& owner
    );


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
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDispersionModel(CloudType)                                         \
                                                                               \
    typedef Foam::CloudType::kinematicCloudType kinematicCloudType;            \
    defineTemplateTypeNameAndDebug                                             \
        (Foam::DispersionModel<kinematicCloudType>, 0);                        \
                                                                               \
    namespace Foam                                                             \
    {                                                                          \
        defineTemplateRunTimeSelectionTable                                    \
        (                                                                      \
            DispersionModel<kinematicCloudType>,                               \
            dictionary                                                         \
        );                                                                     \
    }

#define makeDispersionModelType(SS, CloudType)                                 \
                                                                               \
    typedef Foam::CloudType::kinematicCloudType kinematicCloudType;            \
    defineNamedTemplateTypeNameAndDebug(Foam::SS<kinematicCloudType>, 0);      \
                                                                               \
    Foam::DispersionModel<kinematicCloudType>::                                \
        adddictionaryConstructorToTable<Foam::SS<kinematicCloudType>>          \
            add##SS##CloudType##kinematicCloudType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "DispersionModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
