/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenCFD Ltd
-------------------------------------------------------------------------------
License

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
    Foam::combustionModels::eddyDissipationDiffusionModel

Group
    grpCombustionModels

Description
    Eddy dissipation model based on the principle of mixed is burnt.

    The time scale is determined by the minimum of the turbulence
    and diffusion time scales.


SourceFiles
    eddyDissipationDiffusionModel.C

\*---------------------------------------------------------------------------*/

//- Define Foam_eddyDissipationDiffusionModel_defines_typeName in .C files that
//  define the template statics of eddyDissipationDiffusionModel (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_eddyDissipationDiffusionModel_defines_typeName)
#  define Foam_eddyDissipationDiffusionModel_typeName_API
#else
#  define Foam_eddyDissipationDiffusionModel_typeName_API combustionModels_API
#endif

#ifndef eddyDissipationDiffusionModel_H
#define eddyDissipationDiffusionModel_H

#include "eddyDissipationModelBase.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                Class eddyDissipationDiffusionModel Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class eddyDissipationDiffusionModel
:
    public eddyDissipationModelBase<ReactionThermo, ThermoType>
{
    // Private data

        //- Diffusivity constant
        scalar Cd_;


    // Private member functions

        //- No copy construct
        eddyDissipationDiffusionModel
        (
            const eddyDissipationDiffusionModel&
        ) = delete;

        //- No copy assignment
        void operator=(const eddyDissipationDiffusionModel&) = delete;


public:

    //- Runtime type information
    TypeNameApi(Foam_eddyDissipationDiffusionModel_typeName_API, "EDM");


    // Constructors

        //- Construct from components
        eddyDissipationDiffusionModel
        (
            const word& modelType,
            ReactionThermo& thermo,
            const compressibleTurbulenceModel& turb,
            const word& combustionProperties
        );


    //- Destructor
    virtual ~eddyDissipationDiffusionModel();


    // Member Functions

        //- Calculate time scale
        virtual tmp<volScalarField> timeScale();

         //- Return the reciprocal of the diffusion time scale
        tmp<volScalarField> rtDiff() const;


        // I-O

            //- Update properties
            virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "eddyDissipationDiffusionModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
