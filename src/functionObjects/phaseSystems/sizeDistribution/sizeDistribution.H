/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2019 OpenFOAM Foundation
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
    Foam::functionObjects::sizeDistribution

Description
    This function object calculates and outputs information about the size
    distribution of the dispersed phase, such as the number density function or
    its moments. It is designed to be used exclusively with the population
    balance modeling functionality of the reactingEulerFoam solvers. It can be
    applied to a specific cellZone or the entire domain.

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    sizeDistributionFO
    {
        // Mandatory entries
        type                sizeDistribution;
        libs                (reactingEulerFoamFunctionObjects);
        selectionMode       <word>;
        functionType        <word>;
        abszissaType        <word>;
        populationBalance   <word>;

        // Conditional entries

            // when 'selectionMode' is 'cellZone'
            cellZone            <word>;

        // Optional entries
        writeVolume         <bool>;
        momentOrder         <label>;
        normalize           <bool>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description                        | Type | Reqd | Deflt
      type     | Type name: sizeDistribution        | word | yes  | -
      libs     | Library name: reactingEulerFoamFunctionObjects | word | yes | -
      selectionMode | Evaluate for cellZone or entire mesh | word | yes | -
      functionType  | Name of the function type     | word | yes  | -
      abszissaType  | Name of the abszissa type     | word | yes  | -
      populationBalance | Name of the model         | word | yes  | -
      momentOrder | Write the moment up to given order | label | no | 0
      normalize | Flag to normalize the results     | bool | no | false
      cellZone | Name of the cell zone | word | conditional | -
    \endtable

    Options for the \c selectionMode entry:
    \verbatim
      cellZone | Select the specified cell zone
      all      | Select all the cells
    \endverbatim

    Options for the \c functionType entry:
    \verbatim
      numberDensity       | Calculates the number density function (NDF)
      volumeDensity       | Calculates the volume density function (VDF)
      numberConcentration | Calculates the number concentration
      moments             | Calculates the moments of the distribution
    \endverbatim

    Options for the \c abszissaType entry:
    \verbatim
      diameter | Use particle diameter as abscissa
      volume   | Use particle volume as abscissa
    \endverbatim

    The inherited entries are elaborated in:
      - \link functionObject.H \endlink
      - \link writeFile.H \endlink
      - \link populationBalanceModel.H \endlink

Note
    The choice of \c abszissaType determines whether the distribution and
    its moments are calculated with respect to particle diameter or particle
    volume.

SourceFiles
    sizeDistribution.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_sizeDistribution_H
#define Foam_functionObjects_sizeDistribution_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"
#include "populationBalanceModel.H"
#include "Enum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class fvMesh;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                      Class sizeDistribution Declaration
\*---------------------------------------------------------------------------*/

class sizeDistribution
:
    public fvMeshFunctionObject,
    public writeFile
{
public:

    // Public Enumerations

        //- Selection mode type enumeration
        enum selectionModeTypes
        {
            rtCellZone,
            rtAll
        };

        //- Selection mode type names
        static phaseFunctionObjects_API const Enum<selectionModeTypes> selectionModeTypeNames_;


        //- Function type enumeration
        enum functionTypes
        {
            ftNdf,
            ftVdf,
            ftNc,
            ftMom
        };

        //- Function type names
        static phaseFunctionObjects_API const Enum<functionTypes> functionTypeNames_;


        //- Abszissa type enumeration
        enum abszissaTypes
        {
            atDiameter,
            atVolume,
        };

        //- Abszissa type names
        static phaseFunctionObjects_API const Enum<abszissaTypes> abszissaTypeNames_;


protected:

    // Protected Data

        //- Construction dictionary
        dictionary dict_;

        //- Selection mode type
        selectionModeTypes selectionModeType_;

        //- Name of selection
        word selectionModeTypeName_;

        //- Function type
        functionTypes functionType_;

        //- Abszissa type
        abszissaTypes abszissaType_;

        //- Global number of cells
        label nCells_;

        //- Local list of cell IDs
        labelList cellId_;

        //- Total volume of the evaluated selection
        scalar volume_;

        //- Optionally write the volume of the sizeDistribution
        bool writeVolume_;

        //- PopulationBalance
        const Foam::diameterModels::populationBalanceModel& popBal_;

        //- Number concentrations
        List<scalar> N_;

        //- Write moments up to specified order with respect to abszissaType
        label momentOrder_;

        //- Normalization switch
        const Switch normalize_;

        //- Sum of number concentrations
        scalar sumN_;

        //- Volumertic sum
        scalar sumV_;


    // Protected Member Functions

        //- Initialise, e.g. cell addressing
        void initialise(const dictionary& dict);

        //- Set cells to evaluate based on a cell zone
        void setCellZoneCells();

        //- Calculate and return volume of the evaluated cell zone
        scalar volume() const;

        //- Combine fields from all processor domains into single field
        void combineFields(scalarField& field);

        //- Filter field according to cellIds
        tmp<scalarField> filterField(const scalarField& field) const;

        //- Output file header information
        void writeFileHeader(const label i = 0);


public:

    //- Runtime type information
    TypeNameApi(phaseFunctionObjects_API, "sizeDistribution");


    // Constructors

        //- Construct from name, Time and dictionary
        sizeDistribution
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sizeDistribution();


    // Member Functions

        //- Return the reference to the construction dictionary
        const dictionary& dict() const
        {
            return dict_;
        }

        //- Return the local list of cell IDs
        const labelList& cellId() const
        {
            return cellId_;
        }

        //- Helper function to return the reference to the mesh
        const fvMesh& mesh() const
        {
            return refCast<const fvMesh>(obr_);
        }

    // I-O

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
