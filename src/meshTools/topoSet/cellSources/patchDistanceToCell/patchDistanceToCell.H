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
    Foam::patchDistanceToCell

Description
    A \c topoSetCellSource to select cells whose geometric distance from
    one or more patches is within a user-specified range.

    The distance is computed using \c MeshWave propagation (same algorithm
    as turbulence wall distance), which gives the true Euclidean distance
    from each cell centre to the nearest point on the selected patch faces.

    Operands:
    \table
      Operand   | Type    | Location
      output    | cellSet | $FOAM_CASE/constant/polyMesh/sets/\<set\>
    \endtable

Usage
    Minimal example by using \c system/topoSetDict.actions:
    \verbatim
    {
        // Mandatory (inherited) entries
        name        <name>;
        type        cellSet;
        action      <action>;

        // Mandatory entries
        source      patchDistanceToCell;

        // Conditional mandatory entries
        // Select one of the below

        // Option-1
        patches
        (
            <patchName1>
            <patchName2>
            ...
        );

        // Option-2
        patch    <patchName>;

        // Maximum distance from the patch [m]
        distance    <scalar>;

        // Optional minimum distance from the patch [m] (default: 0)
        minDistance <scalar>;
    }
    \endverbatim

    where the entries mean:
    \table
      Property    | Description                         | Type   | Reqd  | Deflt
      name        | Name of cellSet                     | word   |  yes  | -
      type        | Type name: cellSet                  | word   |  yes  | -
      action      | Action applied on cells - see below | word   |  yes  | -
      source      | Source name: patchDistanceToCell    | word   |  yes  | -
      distance    | Maximum distance from patch [m]     | scalar |  yes  | -
      minDistance | Minimum distance from patch [m]     | scalar |  no   | 0
    \endtable

    Options for the \c action entry:
    \verbatim
      new      | Create a new cellSet from selected cells
      add      | Add selected cells into this cellSet
      subtract | Remove selected cells from this cellSet
    \endverbatim

    Options for the conditional mandatory entries:
    \verbatim
      Entry    | Description                   | Type     | Reqd   | Deflt
      patches  | Names of patches or wildcards | wordList | cond'l | -
      patch    | Name of patch or wildcard     | word     | cond'l | -
    \endverbatim

Note
    The order of precedence among the conditional mandatory entries from the
    highest to the lowest is \c patches, and \c patch.

SourceFiles
    patchDistanceToCell.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_patchDistanceToCell_H
#define Foam_patchDistanceToCell_H

#include "topoSetCellSource.H"
#include "wordRes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class patchDistanceToCell Declaration
\*---------------------------------------------------------------------------*/

class patchDistanceToCell
:
    public topoSetCellSource
{
    // Private Data

        //- Add usage string
        static meshTools_API addToUsageTable usage_;

        //- Matcher for patches
        wordRes selectedPatches_;

        //- Maximum distance from the patch [m]
        scalar distance_;

        //- Minimum distance from the patch [m]
        scalar minDistance_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeNameApi(meshTools_API, "patchDistanceToCell");

    // Constructors

        //- Construct from components (single patch, distance range)
        patchDistanceToCell
        (
            const polyMesh& mesh,
            const wordRe& patchName,
            const scalar distance,
            const scalar minDistance = 0
        );

        //- Construct from dictionary
        patchDistanceToCell(const polyMesh& mesh, const dictionary& dict);

        //- Construct from Istream
        patchDistanceToCell(const polyMesh& mesh, Istream& is);


    //- Destructor
    virtual ~patchDistanceToCell() = default;


    // Member Functions

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet& set
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
