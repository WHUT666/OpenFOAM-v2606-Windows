/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2020 OpenCFD Ltd.
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
    Foam::functionObjects::hydrostaticPressure

Group
    grpInitialisationFunctionObjects

Description
    Calculates and outputs the pressure fields \c p_rgh and \c ph_rgh.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    hydrostaticPressureFO
    {
        // Mandatory entries
        type        hydrostaticPressure;
        libs        (initialisationFunctionObjects);

        // Optional entries
        p_rgh       <word>;
        ph_rgh      <word>;
        pRef        <word>;
        rho         <word>;
        U           <word>;
        gh          <word>;
        ghf         <word>;
        nCorrectors <label>;
        reInitialise <bool>;

        // Conditional entries

            // when 'pRef' is 'pInf'
            pRefValue   <scalar>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                        | Type | Reqd | Deflt
      type      | Type name: hydrostaticPressure     | word | yes  | -
      libs      | Library name: initialisationFunctionObjects | word | yes | -
      p_rgh     | Name of p - rho*g*h field          | word | no   | p_rgh
      ph_rgh    | Name of p_hydrostatic - rho*g*h field | word | no | ph_rgh
      pRef      | Name of pressure ref field         | word | no   | pRef
      pRefValue | Value of pressure ref              | scalar | conditional | 0
      rho       | Name of density field              | word | no   | rho
      U         | Name of velocity field             | word | no   | U
      gh        | Name of g*h volume field           | word | no   | gh
      ghf       | Name of g*h surface field          | word | no   | ghf
      nCorrectors | Number of correctors when solving ph_rgh | label | no
      reInitialise | Re-initialise at start of simulation | bool | no | false
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

Note
    Calculates the hydrostatic pressure on construction/re-initialisation;
    the execute and write functions are not used.

SourceFiles
    hydrostaticPressure.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_hydrostaticPressure_H
#define Foam_functionObjects_hydrostaticPressure_H

#include "fvMeshFunctionObject.H"
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                     Class hydrostaticPressure Declaration
\*---------------------------------------------------------------------------*/

class hydrostaticPressure
:
    public fvMeshFunctionObject
{
protected:

    // Protected data

        //- Name of p - rho*g*h field, default is "p_rgh"
        word p_rghName_;

        //- Name of p_hydrostatic - rho*g*h field, default is "ph_rgh"
        word ph_rghName_;

        //- Name of uniform pressure reference field, default is "pRef"
        //  Set to "none" to set to zero or pInf to set according to pRefValue
        word pRefName_;

        //- Reference pressure if pRefName is set to "pInf"
        scalar pRefValue_;

        //- Name of density field, default is "rho"
        word rhoName_;

        //- Name of velocity field, default is "ph_rgh"
        word UName_;

        //- Name of g*h volume field, default is "gh"
        word ghName_;

        //- Name of g*h surface field, default is "ghf"
        word ghfName_;

        //- Number of correctors when solving for ph_rgh
        label nCorrectors_;


    // Protected Member Functions

        //- Helper function to return the reference pressure
        //  Returns zero if pRefName = none; otherwise returns the value
        //  from the mesh database
        dimensionedScalar pRef() const;

        //- Calculate the fields and write
        void calculateAndWrite();


public:

    //- Runtime type information
    TypeNameApi(initialisationFunctionObjects_API, "hydrostaticPressure");


    // Constructors

        //- Construct from name, Time and dictionary
        hydrostaticPressure
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~hydrostaticPressure() = default;


    // Member Functions

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
