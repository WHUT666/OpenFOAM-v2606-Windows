/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2024-2025 OpenCFD Ltd.
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
    Foam::functionObjects::surfaceCourantNumber

Description
    Computes the surface Courant number field at finite-area face centres.

    Operands:
    \table
      Operand           | Type            | Location
      input             | -               | -
      output file       | dat            <!--
                    --> | postProcessing/\<FO\>/\<time\>/\<file\>
      output field      | areaScalarField | \<time\>/\<outField\>
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    surfaceCourantNumber1
    {
        // Mandatory entries
        type        surfaceCourantNumber;
        libs        (regionFaModels);

        // Optional entries
        area        <word>;
        result      <word>;
        phis        <word>;
        rho         <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property   | Description                        | Type | Reqd | Deflt
      type       | Type name: surfaceCourantNumber    | word | yes  | -
      libs       | Library name: regionFaModels       | word | yes  | -
      region     | Name of finite-volume region       | word | no   | region0
      area       | Name of finite-area region         | word | no   |
      result     | Name of result field               | word | no   | surfaceCo
      phis       | Name of edge flux field            | word | no   | phis
      rho        | Name of density field              | word | no   | rho
    \endtable

    The inherited entries are elaborated in:
     - \link fvMeshFunctionObject.H \endlink
     - \link writeFile.H \endlink

Note
  - The \c surfaceCourantNumber calculates the Courant number at face centers,
    rather than at edge centers.
  - If the area region is not specified, the first region is taken

SourceFiles
    surfaceCourantNumber.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_surfaceCourantNumber_H
#define Foam_functionObjects_surfaceCourantNumber_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class faMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                    Class surfaceCourantNumber Declaration
\*---------------------------------------------------------------------------*/

class surfaceCourantNumber
:
    public functionObjects::fvMeshFunctionObject,
    public writeFile
{
    // Private Data

        //- Name of result field
        word resultName_;

        //- Name of edge flux field
        word phisName_;

        //- Name of density field
        word rhoName_;

        //- Reference to finite-area object registry
        const faMesh* faMeshPtr_;


    // Private Member Functions

        //- Output file header information
        virtual void writeFileHeader(Ostream& os);


public:

    //- Runtime type information
    TypeNameApi(regionFaModels_API, "surfaceCourantNumber");


    // Constructors

        //- Construct from Time and dictionary
        surfaceCourantNumber
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        surfaceCourantNumber(const surfaceCourantNumber&) = delete;

        //- No copy assignment
        void operator=(const surfaceCourantNumber&) = delete;


    //- Destructor
    virtual ~surfaceCourantNumber() = default;


    // Member Functions

        //- Read the surfaceCourantNumber data
        virtual bool read(const dictionary&);

        //- Calculate the Courant number field and return true if successful
        virtual bool execute();

        //- Write the result field
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
