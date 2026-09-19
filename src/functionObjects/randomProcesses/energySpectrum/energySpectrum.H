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
    Foam::functionObjects::energySpectrum

Group
    grpFieldFunctionObjects

Description
    Calculates the energy spectrum for a structured IJK mesh.

    Operands:
    \table
      Operand      | Type                 | Location
      input        | -                    | -
      output file  | dat                  | <time>/energySpectrum.dat
      output field | -                    | -
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    energySpectrumFO
    {
        // Mandatory entries
        type        energySpectrum;
        libs        (fieldFunctionObjects);

        // Optional entries
        U           <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: energySpectrum          | word | yes  | -
      libs     | Library name: fieldFunctionObjects | word | yes  | -
      U        | Name of the velocity field         | word | no   | U
    \endtable

    The inherited entries are elaborated in:
    - \link functionObject.H \endlink
    - \link writeFile.H \endlink
    - \link populationBalanceModel.H \endlink

SourceFiles
    energySpectrum.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_energySpectrum_H
#define Foam_functionObjects_energySpectrum_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"
#include "Vector_tpl.H"
#include "vectorField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class energySpectrum Declaration
\*---------------------------------------------------------------------------*/

class energySpectrum
:
    public fvMeshFunctionObject,
    public writeFile
{
protected:

    // Protected Data

        //- I-J-K mesh addressing
        labelList cellAddr_;

        //- Name of velocity field, default = U
        word UName_;

        //- Number of cells in I-J-K directions
        Vector<int> N_;

        //- Reference point
        vector c0_;

        //- Cell length scale
        vector deltaC_;

        //- Wave number
        scalar kappaNorm_;


    // Protected Member Functions

        //- Output file header information
        virtual void writeFileHeader(Ostream& os);

        //- Calculate and write the spectrum
        void calcAndWriteSpectrum
        (
            const vectorField& U,
            const vectorField& C,
            const vector& c0,
            const vector& deltaC,
            const Vector<int>& N,
            const scalar kappaNorm
        );

        //- No copy construct
        energySpectrum(const energySpectrum&) = delete;

        //- No copy assignment
        void operator=(const energySpectrum&) = delete;


public:

    //- Runtime type information
    TypeName("energySpectrum");


    // Constructors

        //- Construct from name, Time and dictionary
        energySpectrum
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~energySpectrum() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary&);

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
