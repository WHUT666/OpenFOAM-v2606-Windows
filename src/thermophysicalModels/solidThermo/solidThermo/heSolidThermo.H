/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017 OpenCFD Ltd
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
    Foam::heSolidThermo

Description
    Energy for a solid mixture

SourceFiles
    heSolidThermo.C

\*---------------------------------------------------------------------------*/

//- Define Foam_heSolidThermo_defines_typeName in .C files that
//  define the template statics of heSolidThermo (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_heSolidThermo_defines_typeName)
#  define Foam_heSolidThermo_typeName_API
#else
#  define Foam_heSolidThermo_typeName_API solidThermo_API
#endif

#ifndef heSolidThermo_H
#define heSolidThermo_H

#include "heThermo.H"
#include "coordinateSystem.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class heSolidThermo Declaration
\*---------------------------------------------------------------------------*/

template<class BasicSolidThermo, class MixtureType>
class heSolidThermo
:
    public heThermo<BasicSolidThermo, MixtureType>
{
    // Private Member Functions

        //- Calculate the thermo variables
        void calculate();

        //- Construct (or reconstruct) anisotropic alpha field
        void constructAniAlpha();

        //- Construct as copy (not implemented)
        heSolidThermo(const heSolidThermo<BasicSolidThermo, MixtureType>&);


    // Private Data

        //- Coordinate system for anisotropic thermal conductivity
        autoPtr<coordinateSystem> csysPtr_;

        //- Anisotropic thermal diffusivity (kappa/Cp) in global coords
        autoPtr<volSymmTensorField> aniAlphaPtr_;


public:

    //- Runtime type information
    TypeNameApi(Foam_heSolidThermo_typeName_API, "heSolidThermo");


    // Constructors

        //- Construct from mesh and phase name
        heSolidThermo
        (
            const fvMesh&,
            const word& phaseName
        );

        //- Construct from mesh, dictionary and phase name
        heSolidThermo
        (
            const fvMesh&,
            const dictionary&,
            const word& phaseName
        );

         //- Construct from mesh, dictionary name and phase name
        heSolidThermo
        (
            const fvMesh&,
            const word& phaseName,
            const word& dictName
        );


    //- Destructor
    virtual ~heSolidThermo();


    // Member functions

        //- Update properties
        virtual void correct();


        // Derived thermal properties

            //- Anisotropic thermal conductivity [W/m/K]
            virtual tmp<volVectorField> Kappa() const;

            //- Return true if thermal conductivity is isotropic
            virtual bool isotropic() const
            {
                return MixtureType::thermoType::isotropic;
            }

            //- Anisotropic thermal diffusivity (kappa/Cp) in global
            //  coordinates. Returns nullptr for isotropic materials.
            virtual const volSymmTensorField& aniAlpha() const
            {
                if (!aniAlphaPtr_)
                {
                    FatalErrorInFunction
                        << "Requested aniAlpha, but it has not been created"
                        << abort(FatalError);
                }
                return *aniAlphaPtr_.get();
            }


        // Per patch calculation

            //- Anisotropic thermal conductivity [W/m/K]
            virtual tmp<vectorField> Kappa(const label patchi) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "heSolidThermo.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
