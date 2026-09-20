/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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
    Foam::pointZone

Description
    A subset of mesh points.

    The labels of points in the zone can be obtained from the addressing()
    list.

    For quick check whether a point belongs to the zone use the lookup
    mechanism in pointZoneMesh, where all the zoned points are registered
    with their zone number.

SourceFiles
    pointZone.C
    pointZoneNew.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_pointZone_H
#define Foam_pointZone_H

#include "zone.H"
#include "pointZoneMeshFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class pointZone;
Ostream& operator<<(Ostream& os, const pointZone& zn);


/*---------------------------------------------------------------------------*\
                          Class pointZone Declaration
\*---------------------------------------------------------------------------*/

class pointZone
:
    public zone
{
    // Private Data

        //- Reference to zone list
        const pointZoneMesh& zoneMesh_;

public:

    // Static Data Members

        //- The name associated with the zone-labels dictionary entry
        static constexpr const char* labelsName() { return "pointLabels"; }


    //- Runtime type information
    TypeNameApi(OpenFOAM_API, "pointZone");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTableApi
        ( OpenFOAM_API,
            autoPtr,
            pointZone,
            dictionary,
            (
                const word& name,
                const dictionary& dict,
                const label index,
                const pointZoneMesh& zm
            ),
            (name, dict, index, zm)
        );


    // Constructors

        //- No copy construct
        pointZone(const pointZone&) = delete;

        //- Construct an empty zone - name="", index=0
        explicit pointZone(const pointZoneMesh& zm);

        //- Construct an empty zone with specified name and index
        pointZone
        (
            const word& name,
            const label index,
            const pointZoneMesh& zm
        );

        //- Construct from components
        pointZone
        (
            const word& name,
            const labelUList& addr,
            const label index,
            const pointZoneMesh& zm
        );

        //- Construct from components, transferring addressing
        pointZone
        (
            const word& name,
            labelList&& addr,
            const label index,
            const pointZoneMesh& zm
        );

        //- Construct from dictionary
        pointZone
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const pointZoneMesh& zm
        );

        //- Copy construct with new mesh reference.
        pointZone
        (
            const pointZone& originalZone,
            const pointZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, index, groups)
        //- and mesh reference.
        pointZone
        (
            const pointZone& originalZone,
            Foam::zero,
            const pointZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, groups),
        //- resetting the index and zone mesh reference.
        pointZone
        (
            const pointZone& originalZone,
            Foam::zero,
            //! The new index (-1 retains the original value)
            const label index,
            const pointZoneMesh& zm
        );

        //- Construct with original zone information (name, groups),
        //- resetting the point addressing, the index and zone mesh reference.
        pointZone
        (
            const pointZone& originalZone,
            const labelUList& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const pointZoneMesh& zm
        );

        //- Construct with a new index and zone mesh information, the name
        //- of the original zone, (move) resetting the point addressing.
        pointZone
        (
            const pointZone& originalZone,
            labelList&& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const pointZoneMesh& zm
        );


        //- Construct and return a clone, resetting the zone mesh
        virtual autoPtr<pointZone> clone
        (
            const pointZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        ) const
        {
            return autoPtr<pointZone>::New(*this, zm, newIndex);
        }

        //- Construct and return a clone,
        //- resetting the point list and zone mesh
        virtual autoPtr<pointZone> clone
        (
            const pointZoneMesh& zm,
            //! The new index (-1 retains the original value)
            const label index,
            const labelUList& addr
        ) const
        {
            return autoPtr<pointZone>::New(*this, addr, index, zm);
        }


    // Selectors

        //- Return a pointer to a new point zone
        //  created on freestore from dictionary
        static autoPtr<pointZone> New
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const pointZoneMesh& zm
        );


    //- Destructor
    virtual ~pointZone() = default;


    // Member Functions

        //- The maximum index the zone may contain == mesh nPoints()
        label max_index() const noexcept;

        //- Return reference to the zone mesh
        const pointZoneMesh& zoneMesh() const noexcept { return zoneMesh_; }

        //- The addressing (point IDs) used for the zone
        using zone::addressing;

        //- The local index of the given mesh point, -1 if not in the zone
        label whichPoint(const label meshPointID) const
        {
            return zone::localID(meshPointID);
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
        virtual bool checkParallelSync(const bool report = false) const;


    // Assign addressing

        //- Move reset addressing from another zone.
        virtual void resetAddressing(pointZone&& zn);

        //- Copy reset addressing from another zone
        virtual void resetAddressing(const pointZone& zn);

        //- Copy assign addressing
        virtual void resetAddressing(const labelUList& addr);

        //- Move assign addressing
        virtual void resetAddressing(labelList&& addr);

        //- Assign addressing, clearing demand-driven data
        void operator=(const pointZone& zn);

        //- Assign addressing, clearing demand-driven data
        void operator=(const labelUList& addr);

        //- Assign addressing, clearing demand-driven data
        void operator=(labelList&& addr);


    // Mesh Changes

        //- Nothing to correct after moving points
        virtual void movePoints(const pointField&)
        {}


    // I-O

        //- Write (dictionary entries)
        virtual void write(Ostream& os) const;

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const pointZone& zn);


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
