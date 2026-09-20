/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2025 OpenCFD Ltd.
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
    Foam::solidProperties

Description
    The thermophysical, mechanical properties of a solid

SourceFiles
    solidProperties.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_solidProperties_H
#define Foam_solidProperties_H

#include "typeInfo.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class solidProperties Declaration
\*---------------------------------------------------------------------------*/

class solidProperties
{
    // Private Data

        //- Density [kg/m3]
        scalar rho_;

        //- Specific heat capacity [J/(kg.K)]
        scalar Cp_;

        //- Thermal conductivity [W/(m.K)]
        scalar kappa_;

        //- Heat of formation [J/kg]
        scalar Hf_;

        //- Emissivity
        scalar emissivity_;

        //- Molar weight [Kg/Kmol]
        scalar W_;

        //- Poisson ratio
        scalar nu_;

        //- Young Modulus [N/m2]
        scalar E_;


public:

    // Public Data Types

        //- Simple categories of solid properties.
        //  Can be leveraged eg, for mechanical-only vs thermal-only etc
        enum categories
        {
            //! Needs mechanical only [E, nu]
            MECHANICAL = (1),
            //! Needs thermal only [Cp, kappa, emissivity?]
            THERMAL = (2),
            //! thermal: mandatory, mechanical: optional
            REGULAR = 0xFF
        };


    //- Runtime type information
    TypeNameApi(thermophysicalProperties_API, "solid");


    // Declare run-time constructor selection tables

    declareRunTimeSelectionTableApi
    ( thermophysicalProperties_API,
        autoPtr,
        solidProperties,
        ,
        (),
        ()
    );

    declareRunTimeSelectionTableApi
    ( thermophysicalProperties_API,
        autoPtr,
        solidProperties,
        dictionary,
        (const dictionary& dict),
        (dict)
    );


    // Constructors

        //- Default construct
        solidProperties() noexcept;

        //- Construct from components
        solidProperties
        (
            scalar rho,
            scalar Cp,
            scalar kappa,
            scalar Hf,
            scalar emissivity,
            scalar W,
            scalar nu,
            scalar E
        ) noexcept;

        //- Construct from dictionary
        explicit solidProperties(const dictionary& dict);

        //- Construct from dictionary with category-specific handling
        solidProperties(const dictionary& dict, categories category);


        //- Construct and return clone
        virtual autoPtr<solidProperties> clone() const
        {
            return autoPtr<solidProperties>::New(*this);
        }


    // Factory Methods

        //- Clone solidProperties
        template<class Derived>
        static autoPtr<solidProperties> Clone(const Derived& prop)
        {
            return autoPtr<solidProperties>(new Derived(prop));
        }

        //- Return a pointer to a new solidProperties created from name
        static autoPtr<solidProperties> New(const word& name);

        //- Return a pointer to a new solidProperties created from dictionary
        static autoPtr<solidProperties> New(const dictionary& dict);


    //- Destructor
    virtual ~solidProperties() = default;


    // Member Functions

        // Physical constants which define the solidProperties

        //- Density [kg/m3]
        scalar rho() const noexcept { return rho_; }

        //- Specific heat capacity [J/(kg.K)]
        scalar Cp() const { return Cp_; }

        //- Thermal conductivity [W/(m.K)]
        scalar kappa() const noexcept { return kappa_; }

        //- Heat of formation [J/kg]
        scalar Hf() const noexcept { return Hf_; }

        //- Sensible enthalpy - reference to Tstd [J/kg]
        inline scalar Hs(const scalar T) const;

        //- Emissivity []
        scalar emissivity() const noexcept { return emissivity_;}

        //- Molar weight [Kg/Kmol]
        scalar W() const noexcept { return W_; }

        //- Poisson ratio
        scalar nu() const noexcept { return nu_; }

        //- Young modulus [N/m2]
        scalar E() const noexcept { return E_; }


    // I-O

        //- Read and set the properties if present in the given dictionary
        void readIfPresent(const dictionary& dict);

        //- Write the solidProperties properties
        virtual void writeData(Ostream& os) const;

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const solidProperties& s);
};


Ostream& operator<<(Ostream&, const solidProperties&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "solidPropertiesI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
