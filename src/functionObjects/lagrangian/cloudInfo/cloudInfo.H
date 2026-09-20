/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
    Copyright (C) 2015-2022 OpenCFD Ltd.
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
    Foam::functionObjects::cloudInfo

Group
    grpLagrangianFunctionObjects

Description
    Outputs Lagrangian cloud information to a file.

    The current outputs include:
    - total current number of parcels
    - total current mass of parcels

    Operands:
    \table
      Operand      | Type     | Location
      input        | -        | -
      output file  | dat      | postProcessing/<FO>/<time>/<cloudName>.dat
      output field | -        | -
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    cloudInfoFO
    {
        // Mandatory entries
        type        cloudInfo;
        libs        (lagrangianFunctionObjects);
        clouds      <wordList>;  // (kinematicCloud1 thermoCloud1)

        // Optional entries
        selection   <dictionary>;
        sampleOnExecute <bool>;
        verbose     <bool>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                        | Type | Reqd | Deflt
      type      | Type name: cloudInfo               | word | yes  | -
      libs      | Library name: lagrangianFunctionObjects | word | yes  | -
      clouds    | List of clouds names to process    | yes | -   | -
      selection | Parcel selection control           | no  | empty-dict
      sampleOnExecute| Sample/report (on execute) without writing | no | false
      verbose   | Additional verbosity               | bool | no   | false
    \endtable

    The inherited entries are elaborated in:
      - \link regionFunctionObject.H \endlink
      - \link logFiles.H \endlink
      - \link parcelSelectionDetail.H \endlink

SourceFiles
    cloudInfo.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_cloudInfo_H
#define Foam_functionObjects_cloudInfo_H

#include "regionFunctionObject.H"
#include "logFiles.H"
#include "parcelSelectionDetail.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class cloudInfo Declaration
\*---------------------------------------------------------------------------*/

class cloudInfo
:
    public functionObjects::regionFunctionObject,
    public functionObjects::logFiles,
    public Foam::Detail::parcelSelection
{
protected:

    // Data Types

        //- Local control for sampling actions
        enum sampleActionType : unsigned
        {
            ACTION_NONE  = 0,
            ACTION_WRITE = 0x1,
            ACTION_STORE = 0x2,
            ACTION_ALL = 0xF
        };


    // Protected Data

        //- Additional verbosity
        bool verbose_;

        //- Perform sample actions on execute as well
        bool onExecute_;

        //- List of cloud names
        wordList cloudNames_;

        //- Output file per cloud
        PtrList<OFstream> filePtrs_;


    // Protected Member Functions

        //- File header information
        virtual void writeFileHeader(Ostream& os) const;

        //- Perform operation report/write
        bool performAction(unsigned request);

        //- No copy construct
        cloudInfo(const cloudInfo&) = delete;

        //- No copy assignment
        void operator=(const cloudInfo&) = delete;


public:

    //- Runtime type information
    TypeNameApi(lagrangianFunctionObjects_API, "cloudInfo");


    // Constructors

        //- Construct from name, Time and dictionary
        cloudInfo
        (
            const word& name,
            const Time& runTime,
            const dictionary&
        );


    //- Destructor
    virtual ~cloudInfo() = default;


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
