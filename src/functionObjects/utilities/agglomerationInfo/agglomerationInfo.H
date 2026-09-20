/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2026 Keysight Technologies
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
    Foam::functionObjects::agglomerationInfo

Description
    Writes the agglomeration information for a GAMG solver to a file,
    controlled by the \c writeControl entry in the function-object dictionary.

    Each agglomeration level is written as a volScalarField with the cell values
    being the index of the coarser cell in the next level.

    Overall agglomeration statistics are written to a file in the
    \c postProcessing directory.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    agglomerationInfoFO
    {
        // Mandatory entries
        type            agglomerationInfo;
        libs            (utilityFunctionObjects);
        field           <fieldName>;

        // Optional entries
        writeControl   runTime;
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: agglomerationInfo       | word | yes  | -
      libs     | Library name: utilityFunctionObjects | word | yes | -
      field    | Name of the field to be agglomerated | word | yes | -
    \endtable

SourceFiles
    agglomerationInfo.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_agglomerationInfo_H
#define Foam_functionObjects_agglomerationInfo_H

#include "writeFile.H"
#include "fvMeshFunctionObject.H"
#include "labelField.H"
#include "volFieldsFwd.H"
#include "GAMGAgglomeration.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class agglomerationInfo Declaration
\*---------------------------------------------------------------------------*/

class agglomerationInfo
:
    public fvMeshFunctionObject,
    public writeFile
{
    // Private Data

        //- Name of the field to be agglomerated
        word fieldName_;

        //- Name of the GAMGAgglomeration object
        word agglomName_;


    // Private Member Functions

        //- Return the GAMGAgglomeration object for the field
        const GAMGAgglomeration& agglomeration() const;

        //- Write the level 0 agglomeration field
        void writeLevel0();

        //- Set the cellsPerLevel field for the given level
        void setCellsPerLevel
        (
            const GAMGAgglomeration& agglom,
            const label leveli,
            const label& procAgglomLeveli,
            labelField& cellsPerLevel
        ) const;

        //- Initialize the agglomeration field for the given level
        tmp<volScalarField> initAgglomField
        (
            const GAMGAgglomeration& agglom,
            const label leveli,
            const label procAgglomLeveli
        ) const;


protected:

    // Protected Member Functions

        //- No copy construct
        agglomerationInfo(const agglomerationInfo&) = delete;

        //- No copy assignment
        void operator=(const agglomerationInfo&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "agglomerationInfo");


    // Constructors

        //- Construct from name, Time and dictionary
        agglomerationInfo
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~agglomerationInfo() = default;


    // Member Functions


        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations (no-op)
        virtual bool execute() { return true; };

        //- Write the function-object results
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //