/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015-2016 OpenFOAM Foundation
    Copyright (C) 2016-2022 OpenCFD Ltd.
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
    Foam::functionObjects::runTimeControl

Group
    grpUtilitiesFunctionObjects

Description
    Controls when the calculation is terminated based on satisfying
    user-specified conditions.

    Optionally specify a number of write steps before the calculation is
    terminated.  Here, a write is performed each time that all conditions are
    satisfied.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    runTimeControlFO
    {
        // Mandatory entries
        type            runTimeControl;
        libs            (utilityFunctionObjects);
        conditions      <dictionary>;

        // Optional entries
        nWriteStep      <label>;
        satisfiedAction <satisfiedAction>;

        // Conditional entries

            // when 'satisfiedAction' is 'setTrigger'
            trigger     <label>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: runTimeControl          | word | yes  | -
      libs     | Library name: utilityFunctionObjects | word | yes | -
      conditions | List of conditions to satisfy    | dictionary | yes | -
      nWriteStep | Number of write steps before exiting | label | no | 0
      satisfiedAction | Operand action              | word | no | end
    \endtable

    Options for the \c satisfiedAction entry:
    \verbatim
      abort      | Write and emit a FatalError, terminating the run abnormally
      end        | Write and exit cleanly, terminating the run normally
      setTrigger | Trigger another condition, allowing chained control logic
    \endverbatim

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    runTimeControl.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_runTimeControl_H
#define Foam_functionObjects_runTimeControl_H

#include "fvMeshFunctionObject.H"
#include "Map.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
namespace runTimeControls
{

// Forward Declarations
class runTimeCondition;

/*---------------------------------------------------------------------------*\
                       Class runTimeControl Declaration
\*---------------------------------------------------------------------------*/

class runTimeControl
:
    public fvMeshFunctionObject
{
public:

    // Public Enumerations

        enum class satisfiedAction
        {
            ABORT,      //!< "abort" - write and emit a FatalError
            END,        //!< "end" - write and exit cleanly
            SET_TRIGGER //!< "setTrigger" - trigger another condition
        };

        static utilityFunctionObjects_API Enum<satisfiedAction> satisfiedActionNames;


private:

    // Private data

        //- List of conditions to satisfy
        PtrList<runTimeCondition> conditions_;

        //- Map to define group IDs
        Map<label> groupMap_;

        //- Number of write steps before exiting
        label nWriteStep_;

        //- Current number of steps written
        label writeStepI_;

        //- Action to take when conditions are satisfied
        satisfiedAction satisfiedAction_;

        //- Trigger index if satisfiedAction is setTrigger
        label triggerIndex_;

        //- Active flag
        //  Used in the trigger case to bypass any evaluations after the
        //  trigger has been set
        bool active_;

        //- Can be restarted flag
        //  Used in the trigger case after the trigger has been set to allow
        //  this object to be restarted/reset the active flag
        bool canRestart_;


    // Private Member Functions

        //- No copy construct
        runTimeControl(const runTimeControl&) = delete;

        //- No copy assignment
        void operator=(const runTimeControl&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "runTimeControl");


    // Constructors

        //- Construct for given objectRegistry and dictionary
        runTimeControl
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~runTimeControl() = default;


    // Member Functions

        //- Read the runTimeControl data
        virtual bool read(const dictionary&);

        //- Execute, currently does nothing
        virtual bool execute();

        //- Calculate the runTimeControl and write
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace runTimeControls
} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
