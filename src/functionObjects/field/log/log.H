/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2019 OpenFOAM Foundation
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
    Foam::functionObjects::log

Group
    grpFieldFunctionObjects

Description
    Computes the natural logarithm of an input \c volScalarField.

    \f[
        f = s \ln(max(f_0, a)) + t
    \f]

    where
    \vartable
      f     | Output volScalarField
      f_0   | Input volScalarField
      \ln   | Natural logarithm operator
      a     | Clip scalar
      s     | Scaling factor
      t     | Offset factor
    \endvartable

    \table
      Operand       | Type           | Location
      input         | volScalarField | <time>/inputField
      output file   | -              | -
      output field  | volScalarField | <time>/outputField
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    logFO
    {
        // Mandatory entries
        type            log;
        libs            (fieldFunctionObjects);
        field           <word>;

        // Optional entries
        clip            <scalar>;
        checkDimensions <bool>;
        scale           <scalar>;
        offset          <scalar>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: log                     | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      field        | Name of the operand field          | word |  yes  | -
      clip         | Value to clip the operand field values <!--
                 --> to prevent zero or negative input  | scalar | no  | SMALL
      checkDimensions | Flag to check dimensions of the operand field <!--
                                                    --> | bool   | no  | true
      scale        | Scaling factor - \c s above        | scalar | no  | 1.0
      offset       | Offset factor - \c t above         | scalar | no  | 0.0
    \endtable

    The inherited entries are elaborated in:
      - \link fieldExpression.H \endlink

Note
    - Performs \f$\ln(max(x, a))\f$ where \f$x\f$ is a \c volScalarField, and
    \f$a\f$ a clip scalar, equals to \c SMALL by default. This prevents zero or
    negative input \f$x\f$, hence the domain error in the natural logarithm.
    - Dimension checking can optionally be suspended if \f$x\f$ is dimensioned.

SourceFiles
    log.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_log_H
#define Foam_functionObjects_log_H

#include "fieldExpression.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class log Declaration
\*---------------------------------------------------------------------------*/

class log
:
    public fieldExpression
{
    // Private Data

        //- Flag to check dimensions of the operand
        Switch checkDimensions_;

        //- Value to clip the operand field values
        //- to prevent zero or negative input
        scalar clipValue_;

        //- Scaling factor
        scalar scale_;

        //- Offset factor
        scalar offset_;


    // Private Member Functions

        //- Calculate the log field and return true if successful
        virtual bool calc();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "log");


    // Constructors

        //- Construct from name, Time and dictionary
        log
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        log(const log&) = delete;

        //- No copy assignment
        void operator=(const log&) = delete;


    //- Destructor
    virtual ~log() = default;


    // Member Functions

        //- Read the function-object dictionary
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
