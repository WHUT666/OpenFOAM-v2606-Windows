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
    Foam::functionObjects::relativeVelocityFunctionObject

Group
    grpFieldFunctionObjects

Description
    Creates a relative velocity field for rotating cases.
    The rotation can be 'specified', reference a 'MRF' or even
    solid body motion with a rotatingMotion (cellZones only).

Usage
    Minimal example:
    \verbatim
    relative
    {
        // Mandatory entries
        type        relativeVelocity;
        libs        (fieldFunctionObjects);

        // Optional entries
        U           UMean;
        result      U_rel;

        // mandatory
        rotationMode    specified;  // specified | MRF | solidBodyRotation

        specified   // single zone
        {
            origin  (0 -0.1 0);
            axis    (0 1 0);
            rpm     25;
            zone    ...;  // optional
        }
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                           | Type | Reqd | Deflt
      type         | Type name: relativeVel                | word | yes  | -
      libs         | Library name: fieldFunctionObjects    | word | yes  | -
      U            | The input velocity field name         | word | no   | U
      result       | The output field name           | word | no   | U_relative
      rotationMode | Rotation mode (see below)             | word | yes |
      backgroundMode | Background velocity type (see below) | word | no | zero
      refVelocity  | Velocity for 'specified' background    | vector | cndtnl |
      specified    | Coefficient dictionary for 'specified' | dict | cndtnl |
    \endtable

    The \c rotationMode can be (specified | MRF | solidBodyRotation).
    For (MRF | solidBodyRotation) modes,
    the corresponding models provide sufficient information.

    For the \em specified mode, the user can specify a single, flat
    \c "specified" dictionary with the following contents:
    \table
      Property     | Description                    | Type | Reqd | Deflt
      origin       | The rotation origin            | vector | yes* |
      axis         | The rotation axis              | vector | yes* |
      omega        | The rotation speed (rad/s)     | scalar | yes* |
      rpm          | The rotation speed (rev/min)   | scalar | yes* |
      n            | The rotation speed (rev/s)     | scalar | yes* |
      zone         | The cellZone selector          | wordRe | no |
    \endtable

    Alternatively, in \em specified mode, the user can specify multiple
    regions using a dictionary of dictionaries. For example,

    \verbatim
    specified   // multi-zone
    {
        inside
        {
            origin  (0 -0.1 0);
            axis    (0 1 0);
            rpm     25;
            zone    inner;   //< The zone name
        }
        ".*Outer"   //< The zone name
        {
            origin  (0 -0.1 0);
            axis    (0 1 0);
            rpm     -50;
        }
    }
    \endverbatim

    The background velocity is initialized based on the
    \c backgroundMode (zero | copy | specified):
    - zero : zero velocity for the background
    - specified : refVelocity for the background
    - copy : copy from input field
    .

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    relativeVelocityFunctionObject.cxx

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_relativeVelocityFunctionObject_H
#define Foam_functionObjects_relativeVelocityFunctionObject_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"
#include "wordRe.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class MRFZoneList;
class solidBodyMotionSolver;

namespace solidBodyMotionFunctions
{
class rotatingMotion;
}

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                        Class zeroGradient Declaration
\*---------------------------------------------------------------------------*/

class relativeVelocityFunctionObject
:
    public fvMeshFunctionObject
{
public:

    // Public Data Types

        //- Enumeration for the initial background velocity field
        enum class backgroundMode
        {
            ZERO,       //!< zero velocity for the background
            SPECIFIED,  //!< refVelocity for the background
            COPY        //!< copy from input field
        };

        //- Enumeration for the tyoe of rotation specification
        enum class rotationMode
        {
            SPECIFIED,  //!< user-specified rotations
            MRF,        //!< rotations from MRF zones
            SOLID_BODY_ROTATION,  //!< rotations from solidBodyMotion
        };

private:

    static fieldFunctionObjects_API const Enum<backgroundMode> backgroundModeNames_;
    static fieldFunctionObjects_API const Enum<rotationMode> rotationModeNames_;

    // Private Data

        //- Selection (specified | zero | copy)
        backgroundMode backgroundMode_;

        //- Selection (specified | mrf | sbr)
        rotationMode rotationMode_;

        //- Specified background velocity
        vector backgroundVelocity_;

        //- The input velocity field name (default: U)
        word UName_;

        //- The output field name (default: U_relative)
        word resultName_;

        // Entries for specified
        List<vector> origin_;
        List<vector> omega_;
        List<wordRe> zoneName_;

        // Entries for MRF zones
        const MRFZoneList* MRFlist_;

        // Entries for solid body rotationMotion
        const solidBodyMotionSolver* sbmSolver_;
        const solidBodyMotionFunctions::rotatingMotion* rotatingPtr_;


    // Private Member Functions

        void init(const dictionary& dict, const bool verbose = false);


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "relativeVelocity");


    // Constructors

        //- Construct from name, Time and dictionary
        relativeVelocityFunctionObject
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        relativeVelocityFunctionObject
        (const relativeVelocityFunctionObject&) = delete;

        //- No copy assignment
        void operator=(const relativeVelocityFunctionObject&) = delete;


    //- Destructor
    virtual ~relativeVelocityFunctionObject() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Calculate/update the relative velocity field
        virtual bool execute();

        //- Write the relative velocity field (if it exists)
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
