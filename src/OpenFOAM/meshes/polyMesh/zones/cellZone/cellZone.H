/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2025 OpenCFD Ltd.
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
    Foam::cellZone

Description
    A subset of mesh cells.

    Currently set up as an indirect list but will be extended to use a
    primitive mesh.  For quick check whether a cell belongs to the zone use
    the lookup mechanism in cellZoneMesh, where all the zoned cells are
    registered with their zone number.

SourceFiles
    cellZone.C
    cellZoneNew.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_cellZone_H
#define Foam_cellZone_H

#include "zone.H"
#include "cellZoneMeshFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class cellZone;
Ostream& operator<<(Ostream& os, const cellZone& zn);

/*---------------------------------------------------------------------------*\
                           Class cellZone Declaration
\*---------------------------------------------------------------------------*/

class cellZone
:
    public zone
{
    // Private Data

        //- Reference to zone list
        const cellZoneMesh& zoneMesh_;


public:

    // Static Data Members

        //- The name associated with the zone-labels dictionary entry
        static constexpr const char* labelsName() { return "cellLabels"; }


    //- Runtime type information
    TypeNameApi(OpenFOAM_API, "cellZone");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTableApi
        ( OpenFOAM_API,
            autoPtr,
            cellZone,
            dictionary,
            (
                const word& name,
                const dictionary& dict,
                const label index,
                const cellZoneMesh& zm
            ),
            (name, dict, index, zm)
        );


    // Constructors

        //- No copy construct
        cellZone(const cellZone&) = delete;

        //- Construct an empty zone - name="", index=0
        explicit cellZone(const cellZoneMesh& zm);

        //- Construct an empty zone with specified name and index
        cellZone
        (
            const word& name,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from components
        cellZone
        (
            const word& name,
            const labelUList& addr,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from components, transferring addressing
        cellZone
        (
            const word& name,
            labelList&& addr,
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct from dictionary
        cellZone
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const cellZoneMesh& zm
        );

        //- Copy construct with new mesh reference.
        cellZone
        (
            const cellZone& originalZone,
            const cellZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, index, groups)
        //- and mesh reference.
        cellZone
        (
            const cellZone& originalZone,
            Foam::zero,
            const cellZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, groups),
        //- resetting the index and zone mesh reference.
        cellZone
        (
            const cellZone& originalZone,
            Foam::zero,
            //! The new index (-1 retains the original value)
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct with original zone information (name, groups),
        //- resetting the cell list, the index and zone mesh reference.
        cellZone
        (
            const cellZone& originalZone,
            const labelUList& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const cellZoneMesh& zm
        );

        //- Construct with original zone information (name, groups),
        //- resetting the cell list, the index and zone mesh reference.
        cellZone
        (
            const cellZone& originalZone,
            labelList&& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const cellZoneMesh& zm
        );


        //- Construct and return a clone, resetting the zone mesh
        virtual autoPtr<cellZone> clone
        (
            const cellZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        ) const
        {
            return autoPtr<cellZone>::New(*this, zm, newIndex);
        }

        //- Construct and return a clone,
        //- resetting the cell list and zone mesh
        virtual autoPtr<cellZone> clone
        (
            const labelUList& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const cellZoneMesh& zm
        ) const
        {
            return autoPtr<cellZone>::New(*this, addr, index, zm);
        }


    // Selectors

        //- Return a pointer to a new cell zone
        //- created on freestore from dictionary
        static autoPtr<cellZone> New
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const cellZoneMesh& zm
        );


    //- Destructor
    virtual ~cellZone() = default;


    // Member Functions

        //- The maximum index the zone may contain == mesh nCells()
        label max_index() const noexcept;

        //- Return reference to the zone mesh
        const cellZoneMesh& zoneMesh() const noexcept { return zoneMesh_; }

        //- The addressing (cell IDs) used for the zone
        using zone::addressing;

        //- The local index of the given mesh cell, -1 if not in the zone
        label whichCell(const label meshCellID) const
        {
            return zone::localID(meshCellID);
        }


    // Checks

        //- Check zone definition.
        //  \return True if any errors.
        virtual bool checkDefinition(const bool report = false) const
        {
            return zone::checkDefinition(max_index(), report);
        }

        //- Check whether zone is synchronised across coupled boundaries.
        //  \return True if any errors.
        virtual bool checkParallelSync(const bool report = false) const
        {
            return false;
        }


    // Assign addressing

        //- Move reset addressing from another zone
        virtual void resetAddressing(cellZone&& zn);

        //- Copy reset addressing from another zone
        virtual void resetAddressing(const cellZone& zn);

        //- Copy assign addressing
        virtual void resetAddressing(const labelUList& addr);

        //- Move assign addressing
        virtual void resetAddressing(labelList&& addr);

        //- Assign addressing, clearing demand-driven data
        void operator=(const cellZone& zn);

        //- Assign addressing, clearing demand-driven data
        void operator=(const labelUList& addr);

        //- Move assign addressing, clearing demand-driven data
        void operator=(labelList&& addr);


    // I-O

        //- Write (dictionary entries)
        virtual void write(Ostream& os) const;

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const cellZone& zn);


    // Housekeeping

        //- Deprecated(2025-09) Write dictionary
        //  \deprecated(2025-09) Write dictionary
        FOAM_DEPRECATED_FOR(2025-09, "write() or operator<<")
        void writeDict(Ostream& os) const
        {
            os.beginBlock(name()); write(os); os.endBlock();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
