/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::functionObjects::icoUncoupledKinematicCloud

Group
    grpLagrangianFunctionObjects

Description
    This functionObject tracks a uncoupled kinematic particle cloud in the
    specified velocity field of an incompressible flow (laminar, RANS or LES).

    It may be used in conjunction with any transient single-phase incompressible
    flow solver such as pisoFoam or pimpleFoam and tracks the particles or
    parcels without affecting the flow-field.

    The kinematicCloud requires the density of the fluid which is looked-up
    from constant/transportProperties dictionary and the acceleration due to
    gravity which is read from the constant/g file if present or defaults to
    zero.

    The kinematicCloud properties are read from the
    constant/kinematicCloudProperties dictionary in the usual manner.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    tracksFO
    {
        // Mandatory entries
        type              icoUncoupledKinematicCloud;
        libs              (lagrangianFunctionObjects);

        // Optional entries
        U                 <word>;
        kinematicCloud    <word>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                        | Type | Reqd | Deflt
      type      | Type name: icoUncoupledKinematicCloud | word | yes  | -
      libs      | Library name: lagrangianFunctionObjects | word | yes  | -
      U         | Name of the velocity field         | word | no | U
      kinematicCloud | Name of the cloud      | word | no | kinematicCloud
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    icoUncoupledKinematicCloud.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_icoUncoupledKinematicCloud_H
#define Foam_functionObjects_icoUncoupledKinematicCloud_H

#include "fvMeshFunctionObject.H"
#include "basicKinematicCollidingCloud.H"
#include "volFields.H"
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class singlePhaseTransportModel;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class icoUncoupledKinematicCloud Declaration
\*---------------------------------------------------------------------------*/

class icoUncoupledKinematicCloud
:
    public fvMeshFunctionObject
{
    // Private Data

        //- Optional acceleration due to gravity
        uniformDimensionedVectorField g_;

        //- Reference to the laminar transport model
        const singlePhaseTransportModel& laminarTransport_;

        //- Uniform density value
        dimensionedScalar rhoValue_;

        //- Density field needed to construct kinematicCloud
        volScalarField rho_;

        //- Dynamic viscosity field needed to construct kinematicCloud
        volScalarField mu_;

        //- Reference to the velocity field
        const volVectorField& U_;

        //- Name of the cloud
        word kinematicCloudName_;

        //- The kinematic cloud
        basicKinematicCollidingCloud kinematicCloud_;


    // Private Member Functions

        //- No copy construct
        icoUncoupledKinematicCloud(const icoUncoupledKinematicCloud&) = delete;

        //- No copy assignment
        void operator=(const icoUncoupledKinematicCloud&) = delete;


public:

    //- Runtime type information
    TypeNameApi(lagrangianFunctionObjects_API, "icoUncoupledKinematicCloud");


    // Constructors

        //- Construct from name, Time and dictionary
        //  Allow the possibility to load fields from files
        icoUncoupledKinematicCloud
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~icoUncoupledKinematicCloud();


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
