/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2020 OpenCFD Ltd.
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
    Foam::functionObjects::setFlow

Group
    grpFieldFunctionObjects

Description
    Provides options to set the velocity and flux fields as a function of time.

    Useful for testing advection behaviour of numerical schemes by e.g.
    imposing solid body rotation, vortex flows.  All types include a scaling
    \c Function1 type enabling the strength of the transformation to vary
    as a function of time.

    Operands:
    \table
      Operand       | Type                       | Location
      input         | {vol,surface}<Type>Field   | <time>/inputField
      output file   | -                          | -
      output field  | {vol,surface}<Type>Field   | <time>/outputField
    \endtable

    where \c Type can be one of:
        \c Scalar, \c Vector, \c SphericalTensor, \c SymmTensor, or \c Tensor.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    setFlowFO
    {
        // Mandatory entries
        type        setFlow;
        libs        (fieldFunctionObjects);
        mode        <word>;
        scale       <Function1<scalar>>;

        // Optional entries
        U           <word>;
        rho         <word>;
        phi         <word>;
        reverseTime <scalar>;

        // Conditional entries

            // when 'mode' is 'function'
            velocity    <Function1<vector>>;

            // when 'mode' is 'rotation'
            omega       <Function1<scalar>>;
            origin      <vector>;
            refDir      <vector>;
            axis        <vector>;

            // when 'mode' is 'vortex2D' or 'vortex3D'
            origin      <vector>;
            refDir      <vector>;
            axis        <vector>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                         | Type | Reqd  | Deflt
      type         | Type name: readFields               | word |  yes  | -
      libs         | Library name: fieldFunctionObjects  | word |  yes  | -
      mode         | Operating mode - see below          | word |  yes  | -
      scale        | Scaling function        | Function1<scalar> | yes  | -
      U            | Name of velocity field              | word |  no   | U
      rho          | Name of density field               | word |  no   | none
      phi          | Name of flux field                  | word |  no   | phi
      reverseTime  <!--
      --> | Specified time to reverse flow direction     | scalar  | no | VGREAT
      velocity     <!--
      --> | Velocity function           | Function1<vector> | conditional | -
      omega        <!--
      --> | Rotational speed function   | Function1<scalar> | conditional | -
      origin       <!--
      --> | Rotation vector origin      | vector            | conditional | -
      refDir       <!--
      --> | Rotation vector reference direction   | vector  | conditional | -
      axis         <!--
      --> | Rotation vector axis                  | vector  | conditional | -
    \endtable

    Options for the \c mode entry:
    \verbatim
      function | Uses a velocity function to set the flow field
      rotation | Applies rotational flow based on specified parameters
      vortex2D | Imposes a 2D vortex flow
      vortex3D | Imposes a 3D vortex flow
    \endverbatim

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    setFlow.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_setFlow_H
#define Foam_functionObjects_setFlow_H

#include "fvMeshFunctionObject.H"
#include "Function1.H"
#include "Enum.H"
#include "point.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class setFlow Declaration
\*---------------------------------------------------------------------------*/

class setFlow
:
    public fvMeshFunctionObject
{
    // Private Enumerations

        //- Options for the operating mode
        enum class modeType
        {
            FUNCTION,
            ROTATION,
            VORTEX2D,
            VORTEX3D
        };

        //- Names for modeType
        static fieldFunctionObjects_API const Enum<modeType> modeTypeNames;


   // Private Data

        //- Operating mode
        modeType mode_;

        //- Name of velocity field
        word UName_;

        //- Name of density field
        word rhoName_;

        //- Name of flux field
        word phiName_;

        //- Reverse time
        scalar reverseTime_;

        //- Scaling function
        autoPtr<Function1<scalar>> scalePtr_;


        // Rotation

            //- Origin
            point origin_;

            //- Rotation tensor for rotational mode
            tensor R_;

            //- Rotational speed function
            autoPtr<Function1<scalar>> omegaPtr_;


        // Function

            //- Velocity function
            autoPtr<Function1<vector>> velocityPtr_;


    // Private Member Functions

        //- Set the flux field
        void setPhi(const volVectorField& U);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "setFlow");


    // Constructors

        //- Construct from name, Time and dictionary
        setFlow
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        setFlow(const setFlow&) = delete;

        //- No copy assignment
        void operator=(const setFlow&) = delete;


    //- Destructor
    virtual ~setFlow() = default;


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
