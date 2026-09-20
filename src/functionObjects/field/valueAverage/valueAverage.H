/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2022 OpenCFD Ltd.
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
    Foam::functionObjects::valueAverage

Group
    grpFieldFunctionObjects

Description
    Computes the ensemble- or time-based singular-value average values,
    with optional windowing, from the output of function objects
    that generate non-field type values (e.g. \c Cd of \c forceCoeffs or
    \c momentum_x in \c momentum function objects).

    Operands:
    \table
      Operand        | Type | Location
      input          | -    | -
      output file    | dat  | postProcessing/<FO>/<time>/file
      output field   | -    | -
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    valueAverageFO
    {
        // Mandatory entries
        type              valueAverage;
        libs              (fieldFunctionObjects);
        functionObject    <FO>;                       // forceCoeffs1;
        fields            (<field1> ... <fieldN>);    // (Cm Cd Cl);

        // Optional entries
        resetOnRestart    <bool>;
        window            <scalar>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                           | Type | Reqd | Deflt
      type         | Type name: valueAverage               | word |  yes  | -
      libs         | Library name: fieldFunctionObjects    | word |  yes  | -
      functionObject | Name of function object to retrieve data | word | yes | -
      fields       | Names of operand fields               | wordList | yes | -
      resetOnRestart | Reset the averaging on restart      | bool | no | false
      window       | Averaging window                    | scalar | no | VGREAT
    \endtable

    The inherited entries are elaborated in:
      - \link regionFunctionObject.H \endlink
      - \link valueAverageBase.H \endlink

SourceFiles
    valueAverage.C
    valueAverageTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_valueAverage_H
#define Foam_functionObjects_valueAverage_H

#include "regionFunctionObject.H"
#include "valueAverageBase.H"
#include "writeFile.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                        Class valueAverage Declaration
\*---------------------------------------------------------------------------*/

class valueAverage
:
    public regionFunctionObject,
    public valueAverageBase
{
public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "valueAverage");


    // Constructors

        //- Construct from name, Time and dictionary
        valueAverage
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        valueAverage(const valueAverage&) = delete;

        //- No copy assignment
        void operator=(const valueAverage&) = delete;


    //- Destructor
    virtual ~valueAverage() = default;


    // Public Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations
        virtual bool execute();

        //- Write the function-object results
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
