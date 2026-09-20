/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 OpenFOAM Foundation
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
    Foam::functionObjects::wallHeatFlux

Group

Description
    This function object calculates the wall heat flux based on a custom model
    defined in the dictionary. The model can be specified to compute the heat
    flux according to user-defined criteria or equations.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    wallHeatFlux1
    {
        // Mandatory entries
        type        wallHeatFlux;
        libs        (fieldFunctionObjects);

        // Optional entries
        model       <word>;

        // Conditional entries

            // Option-1: when 'model' is 'wall' - previous 'wallHeatFlux' state

            // Optional entries
            patches     (<patch1> ... <patchN>); // (wall1 "(wall2|wall3)");
            qr          <word>;


            // Option-2: when 'model' is 'gauge'
            // Mandatory entries
            patch       <word>;
            Tgauge      <scalar>;

            // Optional entries
            absorptivity  <scalar>;
            emissivity    <scalar>;
            T             <word>;
            qin           <word>;
            alphat        <word>;
            convective    <bool>;
            radiative     <bool>;
            writeFields   <bool>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                           | Type | Reqd | Deflt
      type         | Type name: wallHeatFlux               | word |  yes  | -
      libs         | Library name: fieldFunctionObjects    | word |  yes  | -
      model      | Model name                              | word |  no  | wall
      patches    | Names of operand patches   | wordList | no | all wall patches
      qr         | Name of radiative heat flux field       | word | no | qr
      patch      | Name of the patch to probe              | word | yes  | -
      Tgauge     | Gauge temperature (K)                  | scalar | yes | -
      absorptivity | Absorptivity of the gauge              | scalar | no | 1
      emissivity   | Emissivity of the gauge                | scalar | no | 1
      T           | Name of the temperature field          | word | no  | T
      qin | Name of the incident radiative heat flux field | word | no | qin
      alphat | Name of turbulent thermal diffusivity field | word | no | alphat
      convective  | Write convective heat flux into file   | bool | no | true
      radiative   | Write radiative heat flux into file    | bool | no | true
      writeFields | Write the fields 'qConv' and 'qRad'    | bool | no | true
    \endtable

    The inherited entries are elaborated in:
     - \link fvMeshFunctionObject.H \endlink
     - \link writeFile.H \endlink
     - \link probeModel.H \endlink
     - \link patchFieldProbe.H \endlink

SourceFiles
    wallHeatFlux.cxx

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_wallHeatFlux_H
#define functionObjects_wallHeatFlux_H

#include "fvMeshFunctionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class wallHeatFluxModel;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class wallHeatFlux Declaration
\*---------------------------------------------------------------------------*/

class wallHeatFlux
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Heat-flux model
        autoPtr<wallHeatFluxModel> qModelPtr_;


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "wallHeatFlux");


    // Constructors

        //- Construct from name, Time and dictionary
        wallHeatFlux
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~wallHeatFlux() = default;


    // Member Functions

        //- Read the settings
        virtual bool read(const dictionary& dict);

        //- Calculate the heat-flux data
        virtual bool execute();

        //- Write the heat-flux data
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
