/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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
    Foam::functionObjects::fieldsExpression

Group
    grpFieldFunctionObjects

Description
    Intermediate class for handling field expression function objects
    (e.g. \c add, \c subtract etc.) whereinto more than one fields are input.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    FOname
    {
        // Mandatory and optional entries
        ...

        // Mandatory entries
        fields            <wordList>;  // (<field1> <field2> ... <fieldN>);

        // Optional entries
        result            <word>;
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                 | Type     | Reqd  | Deflt
      fields    | Names of operand fields     | wordList |  yes  | -
      result    | Names of output fields | wordList | no | \<FO\>(\<f1\>,...)
    \endtable

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink

SourceFiles
    fieldsExpression.C
    fieldsExpressionTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_fieldsExpression_H
#define Foam_functionObjects_fieldsExpression_H

#include "fvMeshFunctionObject.H"
#include "volFieldsFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class fieldsExpression Declaration
\*---------------------------------------------------------------------------*/

class fieldsExpression
:
    public fvMeshFunctionObject
{
protected:

    // Protected Member Data

        //- Names of fields to process
        wordList fieldNames_;

        //- Name of result fields
        word resultName_;


    // Protected Member Functions

        //- Set the result name
        void setResultName
        (
            const word& typeName,
            const wordList& defaultArg = wordList::null()
        );

        //- Call 'calcFieldType' for the given functionObject
        //  for 'volField' and 'surfaceField' field types
        template<class Type, class FOType>
        bool calcFieldTypes(FOType& fo);

        //- Call 'calcFieldTypes' for the given 'Type' and functionObject
        template<class Type, class FOType>
        bool calcType(FOType& fo);

        //- Call 'calcType' for the given functionObject
        //  for each primitive type
        template<class FOType>
        bool calcAllTypes(FOType& fo);

        //- Calculate expression
        virtual bool calc() = 0;


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "fieldsExpression");


    // Constructors

        //- Construct from name, Time and dictionary
        fieldsExpression
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict,
            const wordList& fieldNames = wordList::null(),
            const word& resultName = word::null
        );

        //- No copy construct
        fieldsExpression(const fieldsExpression&) = delete;

        //- No copy assignment
        void operator=(const fieldsExpression&) = delete;


    //- Destructor
    virtual ~fieldsExpression() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);

        //- Execute the function-object operations
        virtual bool execute();

        //- Write the function-object results
        virtual bool write();

        //- Clear the result fields from the objectRegistry
        virtual bool clear();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fieldsExpressionTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
