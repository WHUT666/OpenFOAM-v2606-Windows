/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2025 OpenCFD Ltd.
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
    Foam::functionObjects::thermoCoupleProbes

Group
    grpUtilitiesFunctionObjects

Description
    Sample probe for temperature using a thermocouple.

    Uses the correlation:

    \f[
        Nu = 2.0 + \left(0.4 Re^{0.5} + 0.06 Re^{2/3}\right)*Pr^{0.4}
    \f]

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    thermoCoupleProbesFO
    {
        // Mandatory entries
        type            thermoCoupleProbes;
        libs            (utilityFunctionObjects);
        radiationField  <word>;  // G;

        rho             <scalar>;
        Cp              <scalar>;
        d               <scalar>;
        epsilon         <scalar>;

        // Optional entries
        U               <word>;

        // Inherited entries
        probeLocations  (<vectorList>);
        fields          (<wordList>);
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                          | Type | Reqd | Deflt
      type         | Type name: systemCall                | word | yes  | -
      libs         | Library name: utilityFunctionObjects | word | yes  | -
      radiationField | Name of the radiation field        | word | yes  | -
      rho          | Thermocouple density                 | scalar | yes | -
      Cp           | Thermocouple heat capacity           | scalar | yes | -
      d            | Thermocouple diameter                | scalar | yes | -
      epsilon      | Thermocouple emissivity              | scalar | yes | -
      U            | Name of the velocity field           | word | no   | U
    \endtable

    The inherited entries are elaborated in:
      - \link probes.H \endlink
      - \link ODESystem.H \endlink

SourceFiles
    thermoCoupleProbes.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_thermoCoupleProbes_H
#define Foam_functionObjects_thermoCoupleProbes_H

#include "probes.H"
#include "ODESystem.H"
#include "ODESolver.H"
#include "basicThermo.H"
#include "fluidThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                     Class thermoCoupleProbes Declaration
\*---------------------------------------------------------------------------*/

class thermoCoupleProbes
:
    public probes,
    public ODESystem
{
protected:

    // Protected Data

        //- Thermocouple density
        scalar rho_;

        //- Thermocouple heat capacity
        scalar Cp_;

        //- Thermocouple diameter
        scalar d_;

        //- Thermocouple emissivity
        scalar epsilon_;

        //- Name of the velocity field
        word UName_;

        //- Name of the incident radiation field
        word radiationFieldName_;

        //- Fluid thermo reference
        const fluidThermo& thermo_;

        //- ODESolver
        autoPtr<ODESolver> odeSolver_;

        //- Cached thermocouple temperature
        scalarField Ttc_;


private:

    // Private Member Functions

        //- Sample/write
        template<class Type>
        void writeValues
        (
            const word& fieldName,
            const Field<Type>& values,
            const scalar timeValue
        );

        //- No copy construct
        thermoCoupleProbes(const thermoCoupleProbes&) = delete;

        //- No copy assignment
        void operator=(const thermoCoupleProbes&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "thermoCoupleProbes");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        thermoCoupleProbes
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict,
            const bool loadFromFiles = false,
            const bool readFields = true
        );


    //- Destructor
    virtual ~thermoCoupleProbes() = default;


    // ODE functions (overriding abstract functions in ODE.H)

        //- Number of ODE's to solve
        virtual label nEqns() const;

        virtual void derivatives
        (
            const scalar x,
            const scalarField& y,
            scalarField& dydx
        ) const;

        virtual void jacobian
        (
            const scalar t,
            const scalarField& y,
            scalarField& dfdt,
            scalarSquareMatrix& dfdy
        ) const;


    // Public Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary&);

        //- Execute the function-object operations
        //  Evaluates the ODESolver
        virtual bool execute();

        //- Write the function-object results
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "thermoCoupleProbes.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
