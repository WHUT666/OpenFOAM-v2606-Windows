/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2025 OpenCFD Ltd.
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
    Foam::faFaceZone

Description
    A subset of finite-area face elements.

SourceFiles
    faFaceZone.C

\*---------------------------------------------------------------------------*/

#ifndef Foam_faFaceZone_H
#define Foam_faFaceZone_H

#include "zone.H"
#include "faFaceZoneMeshFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class faFaceZone;
Ostream& operator<<(Ostream& os, const faFaceZone& zn);

/*---------------------------------------------------------------------------*\
                           Class faFaceZone Declaration
\*---------------------------------------------------------------------------*/

class faFaceZone
:
    public zone
{
    // Private Data

        //- Reference to zone list
        const faFaceZoneMesh& zoneMesh_;


public:

    // Static Data Members

        //- The name associated with the zone-labels dictionary entry
        static constexpr const char* labelsName() { return "faceLabels"; }


    //- Runtime type information
    TypeNameApi(finiteArea_API, "faFaceZone");


    // Constructors

        //- No copy construct
        faFaceZone(const faFaceZone&) = delete;

        //- Construct an empty zone - name="", index=0
        explicit faFaceZone(const faFaceZoneMesh& zm);

        //- Construct an empty zone with specified name and index
        faFaceZone
        (
            const word& name,
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Construct from components
        faFaceZone
        (
            const word& name,
            const labelUList& addr,
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Construct from components, transferring addressing
        faFaceZone
        (
            const word& name,
            labelList&& addr,
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Construct from dictionary
        faFaceZone
        (
            const word& name,
            const dictionary& dict,
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Copy construct with a new mesh reference
        faFaceZone
        (
            const faFaceZone& originalZone,
            const faFaceZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, index, groups)
        //- and mesh reference.
        faFaceZone
        (
            const faFaceZone& originalZone,
            Foam::zero,
            const faFaceZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        );

        //- Construct empty with original zone information (name, groups),
        //- resetting the index and zone mesh reference.
        faFaceZone
        (
            const faFaceZone& originalZone,
            Foam::zero,
            //! The new index (-1 retains the original value)
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Construct with original zone information (name, groups),
        //- resetting the face list, the index and zone mesh reference.
        faFaceZone
        (
            const faFaceZone& originalZone,
            const labelUList& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const faFaceZoneMesh& zm
        );

        //- Construct with original zone information (name, groups),
        //- resetting the face list, the index and zone mesh reference.
        faFaceZone
        (
            const faFaceZone& originalZone,
            labelList&& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const faFaceZoneMesh& zm
        );


        //- Construct and return a clone, resetting the zone mesh
        virtual autoPtr<faFaceZone> clone
        (
            const faFaceZoneMesh& zm,
            //! The new index (optional. -1 retains the original value)
            const label newIndex = -1
        ) const
        {
            return autoPtr<faFaceZone>::New(*this, zm, newIndex);
        }

        //- Construct and return a clone,
        //- resetting the face list and zone mesh
        virtual autoPtr<faFaceZone> clone
        (
            const labelUList& addr,
            //! The new index (-1 retains the original value)
            const label index,
            const faFaceZoneMesh& zm
        ) const
        {
            return autoPtr<faFaceZone>::New(*this, addr, index, zm);
        }


    //- Destructor
    virtual ~faFaceZone() = default;


    // Member Functions

        //- The maximum index the zone may contain == mesh nFaces()
        label max_index() const noexcept;

        //- Return reference to the zone mesh
        const faFaceZoneMesh& zoneMesh() const noexcept { return zoneMesh_; }

        //- The addressing (face IDs) used for the zone
        using zone::addressing;

        //- The local index of the given mesh face, -1 if not in the zone
        label whichFace(const label meshFaceID) const
        {
            return zone::localID(meshFaceID);
        }


    // Checks

        //- Check zone definition. Return true if in error.
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
        virtual void resetAddressing(faFaceZone&& zn);

        //- Copy reset addressing from another zone
        virtual void resetAddressing(const faFaceZone& zn);

        //- Copy assign addressing
        virtual void resetAddressing(const labelUList& addr);

        //- Move assign addressing
        virtual void resetAddressing(labelList&& addr);

        //- Assign addressing, clearing demand-driven data
        void operator=(const faFaceZone& zn);

        //- Assign addressing, clearing demand-driven data
        void operator=(const labelUList& addr);

        //- Move assign addressing, clearing demand-driven data
        void operator=(labelList&& addr);


    // I-O

        //- Write (dictionary entries)
        virtual void write(Ostream& os) const;

        //- Ostream Operator
        friend Ostream& operator<<(Ostream& os, const faFaceZone& zn);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
