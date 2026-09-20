/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::functionObjects::systemCall

Group
    grpUtilitiesFunctionObjects

Description
    Executes system calls, entered in the form of string lists.

    Calls can be made at the following points in the calculation:
    - every time step
    - every output time
    - end of the calculation

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    systemCallFO
    {
        // Mandatory entries
        type        systemCall;
        libs        (utilityFunctionObjects);
        ...

        // Optional entries
        executeCalls
        (
            "echo execute"
        );
        writeCalls
        (
            "echo === writing data ==="
        );
        endCalls
        (
            "echo === echoing .bashrc ==="
            "cat ~/.bashrc"
            "echo \*\*\* done \*\*\*"
        );

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                          | Type | Reqd | Deflt
      type         | Type name: systemCall                | word | yes  | -
      libs         | Library name: utilityFunctionObjects | word | yes  | -
      executeCalls | List of calls on execute        | stringList | yes | -
      writeCalls   | List of calls on write          | stringList | yes | -
      endCalls     | List of calls on end            | stringList | yes | -
      master       | Execute on master only          | bool       | no  | false
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

Note
    Since this function object executes system calls, there is a potential
    security risk.  In order to use the \c systemCall function object, the
    \c allowSystemOperations must be set to '1'; otherwise, system calls will
    not be allowed.

    Additionally, since the system commands are normally sent via the shell,
    special shell character may require backslash escaping.

SourceFiles
    systemCall.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_systemCall_H
#define Foam_functionObjects_systemCall_H

#include "functionObject.H"
#include "stringList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class systemCall Declaration
\*---------------------------------------------------------------------------*/

class systemCall
:
    public functionObject
{
protected:

    // Protected Data

        //- List of calls to execute - every step
        stringList executeCalls_;

        //- List of calls to execute - write steps
        stringList writeCalls_;

        //- List of calls to execute when exiting the time-loop
        stringList endCalls_;

        //- Perform system calls on the master only
        bool masterOnly_;


    // Protected Member Functions

        //- Dispatch specified calls
        label dispatch(const stringList& calls);

        //- No copy construct
        systemCall(const systemCall&) = delete;

        //- No copy assignment
        void operator=(const systemCall&) = delete;


public:

    //- Runtime type information
    TypeNameApi(utilityFunctionObjects_API, "systemCall");


    // Constructors

        //- Construct from name, Time and dictionary
        systemCall
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~systemCall() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations
        //  Execute the "executeCalls" at each time-step
        virtual bool execute();

        //- Write the function-object results
        //  Write, execute the "writeCalls"
        virtual bool write();

        //- Execute the "endCalls" at the final time-loop
        virtual bool end();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
