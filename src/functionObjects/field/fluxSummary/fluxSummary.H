/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015 OpenFOAM Foundation
    Copyright (C) 2015-2020 OpenCFD Ltd.
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
    Foam::functionObjects::fluxSummary

Group
    grpFieldFunctionObjects

Description
    Computes the volumetric- or mass-flux information across selections of face
    zones.

    Operands:
    \table
      Operand       | Type | Location
      input         | -    | -
      output file   | dat  | postProcessing/<FO>/<time>/faceName
      output field  | -    | -
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    fluxSummaryFO
    {
        // Mandatory entries
        type            fluxSummary;
        libs            (fieldFunctionObjects);
        mode            <word>;

        // Optional entries
        phi             <word>;
        scaleFactor     <scalar>;
        tolerance       <scalar>;

        // Inherited entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Reqd  | Deflt
      type         | Type name: fluxSummary             | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      mode         | Mode to generate faces to test     | word |  yes  | -
      phi          | Name of surface flux field         | word |  no   | phi
      scaleFactor | Factor to scale results             | scalar | no  | 1.0
      tolerance   | Tolerance for reference direction   | scalar | no  | 0.8
    \endtable

    The inherited entries are elaborated in:
     - \link functionObject.H \endlink
     - \link writeFile.H \endlink

    Options for the \c mode entry:
    \verbatim
      faceZone
      faceZoneAndDirection
      cellZoneAndDirection
      surface
      surfaceAndDirection
    \endverbatim

Note
    For surface and direction, phi='U' can be used for determining the fluxes.

SourceFiles
    fluxSummary.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_functionObjects_fluxSummary_H
#define Foam_functionObjects_fluxSummary_H

#include "fvMeshFunctionObject.H"
#include "writeFile.H"
#include "vector.H"
#include "DynamicList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class dimensionSet;

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class fluxSummary Declaration
\*---------------------------------------------------------------------------*/

class fluxSummary
:
    public fvMeshFunctionObject,
    public writeFile
{
public:

    // Public Enumerations

        //- Face mode type
        enum modeType
        {
            mdFaceZone,             //!< Face zone
            mdFaceZoneAndDirection, //!< Face zone with prescribed direction
            mdCellZoneAndDirection, //!< Cell zone with prescribed direction
            mdSurface,              //!< A functionObject surface
            mdSurfaceAndDirection   //!< A surface with prescribed direction
        };

        //- Face mode names
        static fieldFunctionObjects_API const Enum<modeType> modeTypeNames_;


protected:

    // Protected Data

        //- Track if the surface needs an update
        bool needsUpdate_;

        //- Mode for face determination/to generate faces to test
        modeType mode_;

        //- Factor to scale results
        scalar scaleFactor_;

        //- Name of flux field
        word phiName_;


        // Per-faceZone/surface information

            //- Region (zone/surface) names
            List<word> zoneNames_;

            //- Region (zone/surface) directions
            List<vector> zoneDirections_;

            //- Face IDs
            List<labelList> faceID_;

            //- Face patch IDs
            List<labelList> facePatchID_;

            //- Face flip map signs
            List<boolList> faceFlip_;

            //- Output file per face zone
            PtrList<OFstream> filePtrs_;


        //- Tolerance applied when matching face normals
        scalar tolerance_;


    // Protected Member Functions

        //- Check if surface mode instead of zone mode
        bool isSurfaceMode() const;

        //- Check flowType (mass or volume)
        //  Return name on success, fatal error on failure.
        word checkFlowType
        (
            const dimensionSet& fieldDims,
            const word& fieldName
        ) const;

        //- Initialise for given surface name
        void initialiseSurface
        (
            const word& surfName,
            DynamicList<word>& names,
            DynamicList<vector>& dir,
            DynamicList<boolList>& faceFlip
        ) const;

        //- Initialise for given surface name and direction
        void initialiseSurfaceAndDirection
        (
            const word& surfName,
            const vector& refDir,
            DynamicList<word>& names,
            DynamicList<vector>& dir,
            DynamicList<boolList>& faceFlip
        ) const;

        //- Initialise face set from face zone
        void initialiseFaceZone
        (
            const word& faceZoneName,
            DynamicList<word>& names,
            DynamicList<vector>& dir,
            DynamicList<labelList>& faceID,
            DynamicList<labelList>& facePatchID,
            DynamicList<boolList>& faceFlip
        ) const;

        //- Initialise face set from face zone and direction
        void initialiseFaceZoneAndDirection
        (
            const word& faceZoneName,
            const vector& refDir,
            DynamicList<word>& names,
            DynamicList<vector>& dir,
            DynamicList<labelList>& faceID,
            DynamicList<labelList>& facePatchID,
            DynamicList<boolList>& faceFlip
        ) const;

        //- Initialise face set from cell zone and direction
        void initialiseCellZoneAndDirection
        (
            const word& cellZoneName,
            const vector& refDir,
            DynamicList<word>& names,
            DynamicList<vector>& dir,
            DynamicList<labelList>& faceID,
            DynamicList<labelList>& facePatchID,
            DynamicList<boolList>& faceFlip
        ) const;

        //- Calculate the total area for the surface or derived faceZone
        scalar totalArea(const label idx) const;

        //- Initialise - after read(), before write()
        bool update();

        //- Output file header information
        virtual void writeFileHeader
        (
            const word& zoneName,
            const scalar area,
            const vector& refDir,
            Ostream& os
        ) const;

        //- Specialized write for surfaces
        bool surfaceModeWrite();


public:

    //- Runtime type information
    TypeNameApi(fieldFunctionObjects_API, "fluxSummary");


    // Constructors

        //- Construct from name, Time and dictionary
        fluxSummary
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        fluxSummary(const fluxSummary&) = delete;

        //- No copy assignment
        void operator=(const fluxSummary&) = delete;


    //- Destructor
    virtual ~fluxSummary() = default;


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
