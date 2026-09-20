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
    Foam::noChemistrySolver

Description
    Dummy chemistry solver for 'none' option

SourceFiles
    noChemistrySolver.H
    noChemistrySolver.C

\*---------------------------------------------------------------------------*/

//- Define Foam_noChemistrySolver_defines_typeName in .C files that
//  define the template statics of noChemistrySolver (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_noChemistrySolver_defines_typeName)
#  define Foam_noChemistrySolver_typeName_API
#else
#  define Foam_noChemistrySolver_typeName_API chemistryModel_API
#endif

#ifndef noChemistrySolver_H
#define noChemistrySolver_H

#include "chemistrySolver.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class noChemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class ChemistryModel>
class noChemistrySolver
:
    public chemistrySolver<ChemistryModel>
{

public:

    //- Runtime type information
    TypeNameApi(Foam_noChemistrySolver_typeName_API, "none");


    // Constructors

        //- Construct from thermo
        noChemistrySolver(typename ChemistryModel::reactionThermo& thermo);


    //- Destructor
    virtual ~noChemistrySolver();


    // Member Functions

        //- Update the concentrations and return the chemical time
        virtual void solve
        (
            scalarField& c,
            scalar& T,
            scalar& p,
            scalar& deltaT,
            scalar& subDeltaT
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "noChemistrySolver.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
