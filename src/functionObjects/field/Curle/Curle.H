/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2023 OpenCFD Ltd.
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
    Foam::functionObjects::Curle

Group
    grpFieldFunctionObjects

Description
    Computes the acoustic pressure based on Curle's analogy.

    Curle's analogy is implemented as:

    \f[
        p' = \frac{1}{4 \pi}
             \frac{\vec{r}}{| \vec{r} | ^2} \cdot
             \left(
                \frac{\vec{F}}{| \vec{r} |}
              + \frac{1}{c_0}\frac{d(\vec{F})}{d(t)}
             \right)
    \f]

    where
    \vartable
        p'          | Curle's acoustic pressure [Pa] or [Pa (m3/kg)]
        c_0         | Reference speed of sound [m/s]
        \vec{r}     | Distance vector to observer locations [m]
        \vec{F}     | Force [N] or [N (m3/kg)]
        t           | time [s]
    \endvartable

    Operands:
    \table
      Operand           | Type           | Location
      input             | volScalarField | <time>/inputField
      output file       | -              | -
      output field      | volScalarField | <time>/outputField
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    CurleFO
    {
        type            Curle;
        libs            (fieldFunctionObjects);
        patches         (<wordRes>);      // (surface1 surface2);
        c0              <scalar>;
        input           <word>;           // point or surface
        output          <word>;           // point or surface

        // Optional entries
        p               <word>;
        observerPositions (<pointList>);  // ((x1 y1 z1)(x2 y2 z2) ... )
        surface         <fileName>;       // inputSurface.obj
        surfaceType     <word>;           // ensight, vtk, obj

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: Curle                   | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      p            | Pressure field name                | word |  no   | p
      patches      | Sound generation patch names       | wordRes | yes | -
      c0           | Reference speed of sound           | scalar | yes | -
      input        | Input type                         | word | yes   | -
      output       | Output type                        | word | yes   | -
      observerPositions | List of observer positions (x y z) | pointList | no |-
      surface      | Input surface file name            | word | no    | -
      surfaceType  | Output surface type                | word | no    | -
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink
      - \link writeFile.H \endlink

Note
    Only the normal-pressure force is included in the force calculation.

SourceFiles
    Curle.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_Curle_H
#define Foam_functionObjects_Curle_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"
#include "Enum.H"
#include "MeshedSurface.H"
#include "surfaceWriter.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                            Class Curle Declaration
\*---------------------------------------------------------------------------*/

class Curle
:
    public fvMeshFunctionObject,
    public writeFile
{
    // Private Enumerations

        //- Options for the mode types
        enum class modeType
        {
            point,
            surface
        };

        //- Names for modeType
        static fieldFunctionObjects_API const Enum<modeType> modeTypeNames_;


    // Private Data

        //- Name of pressure field; default = p
        word pName_;

        //- List of patches to process
        labelList patchIDs_;

        //- Observer positions
        List<point> observerPositions_;

        //- Reference speed of sound
        scalar c0_;

        //- Output files when sampling points
        PtrList<OFstream> rawFilePtrs_;

        //- Input surface when sampling a surface
        MeshedSurface<face> inputSurface_;

        //- Ouput surface when sampling a surface
        autoPtr<surfaceWriter> surfaceWriterPtr_;


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "Curle");


    // Constructors

        //- Construct from name, Time and dictionary
        Curle
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        Curle(const Curle&) = delete;

        //- No copy assignment
        void operator=(const Curle&) = delete;


    //- Destructor
    virtual ~Curle() = default;


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
