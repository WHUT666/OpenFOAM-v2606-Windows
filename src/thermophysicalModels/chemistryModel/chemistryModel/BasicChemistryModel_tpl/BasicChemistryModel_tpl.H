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
    Foam::BasicChemistryModel

Description
    Basic chemistry model templated on thermodynamics

SourceFiles
    BasicChemistryModelI.H
    BasicChemistryModel.C

\*---------------------------------------------------------------------------*/

//- Define Foam_BasicChemistryModel_defines_typeName in .C files that
//  define the template statics of BasicChemistryModel (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_BasicChemistryModel_defines_typeName)
#  define Foam_BasicChemistryModel_typeName_API
#else
#  define Foam_BasicChemistryModel_typeName_API chemistryModel_API
#endif

#ifndef BasicChemistryModel_H
#define BasicChemistryModel_H

#include <basicChemistryModel.H>
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                     class BasicChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class BasicChemistryModel
:
    public basicChemistryModel
{
    // Private Member Functions

        //- Construct as copy (not implemented)
        BasicChemistryModel
        (
            const BasicChemistryModel<ReactionThermo>&
        ) = delete;

        //- No copy assignment
        void operator=(const BasicChemistryModel<ReactionThermo>&) = delete;


protected:

    // Protected data

        //- Thermo
        ReactionThermo& thermo_;


public:

    //- Runtime type information
    TypeNameApi(Foam_BasicChemistryModel_typeName_API, "BasicChemistryModel");


    //- Thermo type
    typedef ReactionThermo reactionThermo;


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTableApi
    ( Foam_BasicChemistryModel_typeName_API,
        autoPtr,
        BasicChemistryModel,
        thermo,
        (ReactionThermo& thermo),
        (thermo)
    );


    // Constructors

        //- Construct from thermo
        BasicChemistryModel(ReactionThermo& thermo);


    //- Selector
    static autoPtr<BasicChemistryModel<ReactionThermo>> New
    (
        ReactionThermo& thermo
    );


    //- Destructor
    virtual ~BasicChemistryModel();


    // Member Functions

        //- Return access to the thermo package
        inline ReactionThermo& thermo();

        //- Return const access to the thermo package
        inline const ReactionThermo& thermo() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include <BasicChemistryModel_tpl.C>
#endif

#include <BasicChemistryModelI_tpl.H>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
