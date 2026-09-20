/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
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

Global
    dimensionSets

Description
    Useful dimension sets

SourceFiles
    dimensionSets.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_dimensionSets_H
#define Foam_dimensionSets_H

#include "scalarMatrices.H"
#include "dimensionedScalarFwd.H"
#include "PtrList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Dimensionless
extern OpenFOAM_API const dimensionSet dimless;

extern OpenFOAM_API const dimensionSet dimMass;
extern OpenFOAM_API const dimensionSet dimLength;
extern OpenFOAM_API const dimensionSet dimTime;
extern OpenFOAM_API const dimensionSet dimTemperature;
extern OpenFOAM_API const dimensionSet dimMoles;
extern OpenFOAM_API const dimensionSet dimCurrent;
extern OpenFOAM_API const dimensionSet dimLuminousIntensity;

extern OpenFOAM_API const dimensionSet dimArea;
extern OpenFOAM_API const dimensionSet dimVolume;

//- Older spelling for dimVolume
//  \deprecated(2019-01) use dimVolume instead
extern OpenFOAM_API const dimensionSet dimVol;

extern OpenFOAM_API const dimensionSet dimDensity;
extern OpenFOAM_API const dimensionSet dimForce;
extern OpenFOAM_API const dimensionSet dimEnergy;
extern OpenFOAM_API const dimensionSet dimPower;

extern OpenFOAM_API const dimensionSet dimVelocity;
extern OpenFOAM_API const dimensionSet dimAcceleration;
extern OpenFOAM_API const dimensionSet dimPressure;
extern OpenFOAM_API const dimensionSet dimCompressibility;
extern OpenFOAM_API const dimensionSet dimGasConstant;
extern OpenFOAM_API const dimensionSet dimSpecificHeatCapacity;
extern OpenFOAM_API const dimensionSet dimViscosity;
extern OpenFOAM_API const dimensionSet dimDynamicViscosity;


/*---------------------------------------------------------------------------*\
                        Class dimensionSets Declaration
\*---------------------------------------------------------------------------*/

//- Construction of unit sets
class dimensionSets
{
    // Private Data

        //- Set of dimensions
        PtrList<dimensionedScalar> units_;

        //- LU decomposition of dimensions
        scalarSquareMatrix conversion_;

        //- See above
        labelList conversionPivots_;

        //- Is LU decomposition valid
        bool valid_;

public:

    // Constructors

        //- Construct from all units and set of units to use
        //- for inversion (writing)
        dimensionSets
        (
            const HashTable<dimensionedScalar>&,
            const wordList& unitNames
        );


    // Member Functions

        //- Return the units
        const PtrList<dimensionedScalar>& units() const noexcept
        {
            return units_;
        }

        //- Is there a valid inverse of the selected unit
        bool good() const noexcept { return valid_; }

        //- Is there a valid inverse of the selected unit
        bool valid() const noexcept { return valid_; }

        //- (if valid) obtain set of coefficients of unitNames
        void coefficients(scalarField& exponents) const;
};


//- Top level dictionary
dictionary& dimensionSystems();

//- Set of all dimensions
const HashTable<dimensionedScalar>& unitSet();

//- Set of units
const dimensionSets& writeUnitSet();


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
