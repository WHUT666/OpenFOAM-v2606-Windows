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
    Foam::combustionModels::PaSR

Group
    grpCombustionModels

Description
    Partially stirred reactor turbulent combustion model.

    This model calculates a finite rate, based on both turbulence and chemistry
    time scales.  Depending on mesh resolution, the Cmix parameter can be used
    to scale the turbulence mixing time scale.

SourceFiles
    PaSR.C

\*---------------------------------------------------------------------------*/

//- Define Foam_PaSR_defines_typeName in .C files that
//  define the template statics of PaSR (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_PaSR_defines_typeName)
#  define Foam_PaSR_typeName_API
#else
#  define Foam_PaSR_typeName_API combustionModels_API
#endif

#ifndef PaSR_H
#define PaSR_H

#include "../laminar/laminar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                            Class PaSR Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class PaSR
:
    public laminar<ReactionThermo>
{
    // Private data

        //- Mixing constant
        scalar Cmix_;

        //- Mixing parameter
        volScalarField kappa_;


    // Private Member Functions

        //- No copy construct
        PaSR(const PaSR&) = delete;

        //- No copy assignment
        void operator=(const PaSR&) = delete;


public:

    //- Runtime type information
    TypeNameApi(Foam_PaSR_typeName_API, "PaSR");


    // Constructors

        //- Construct from components
        PaSR
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressibleTurbulenceModel& turb,
            const word& combustionProperties
        );


    //- Destructor
    virtual ~PaSR();


    // Member Functions

        //- Correct combustion rate
        virtual void correct();

        //- Fuel consumption rate matrix.
        virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

        //- Heat release rate [kg/m/s3]
        virtual tmp<volScalarField> Qdot() const;

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "PaSR.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
