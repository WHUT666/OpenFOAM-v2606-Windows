/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
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
    Foam::chemistryReductionMethods::EFA

Description

SourceFiles
    EFA.C

\*---------------------------------------------------------------------------*/

//- Define Foam_EFA_defines_typeName in .C files that
//  define the template statics of EFA (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_EFA_defines_typeName)
#  define Foam_EFA_typeName_API
#else
#  define Foam_EFA_typeName_API chemistryModel_API
#endif

#ifndef EFA_H
#define EFA_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace chemistryReductionMethods
{

/*---------------------------------------------------------------------------*\
                           Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class EFA
:
    public chemistryReductionMethod<CompType, ThermoType>
{
    // Private data

        List<label> sC_,sH_,sO_,sN_;
        scalar  sortPart_;

public:

    //- Runtime type information
    TypeNameApi(Foam_EFA_typeName_API, "EFA");


    // Constructors

        //- Construct from components
        EFA
        (
            const IOdictionary& dict,
            TDACChemistryModel<CompType, ThermoType>& chemistry
        );


    //- Destructor
    virtual ~EFA();


    // Member Functions

        //- Reduce the mechanism
        virtual void reduceMechanism
        (
            const scalarField &c,
            const scalar T,
            const scalar p
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace chemistryReductionMethods
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "EFA.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
