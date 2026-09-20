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
    Foam::chemistryReductionMethods::PFA

Description
    Path flux analysis

SourceFiles
    PFA.C

\*---------------------------------------------------------------------------*/

//- Define Foam_PFA_defines_typeName in .C files that
//  define the template statics of PFA (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_PFA_defines_typeName)
#  define Foam_PFA_typeName_API
#else
#  define Foam_PFA_typeName_API chemistryModel_API
#endif

#ifndef PFA_H
#define PFA_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace chemistryReductionMethods
{

/*---------------------------------------------------------------------------*\
                           Class ode Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class ThermoType>
class PFA
:
    public chemistryReductionMethod<CompType, ThermoType>
{
    // Private data

        //- List of label for the search initiating set
        labelList searchInitSet_;


public:

    //- Runtime type information
    TypeNameApi(Foam_PFA_typeName_API, "PFA");


    // Constructors

        //- Construct from components
        PFA
        (
            const IOdictionary& dict,
            TDACChemistryModel<CompType, ThermoType>& chemistry
        );


    //- Destructor
    virtual ~PFA();


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
    #include "PFA.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
