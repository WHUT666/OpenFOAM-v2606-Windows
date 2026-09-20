/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2025 Mark Olesen
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
    Foam::UPstream::File

Description
    An opaque wrapper for MPI_File methods
    without any \c <mpi.h> header dependency.

Note
    Not included as part of UPstream.H - only include locally as required

SourceFiles
    UPstreamFile.txx

\*---------------------------------------------------------------------------*/

#ifndef Foam_UPstreamFile_H
#define Foam_UPstreamFile_H

#include "fileName.H"
#include "UPstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class UPstream::File Declaration
\*---------------------------------------------------------------------------*/

class OpenFOAM_API UPstream::File
{
    // Forward Declaration
    class Impl;


    // Private Data

        //- Implementation wrapper of MPI_File etc
        std::unique_ptr<Impl> file_;


protected:

    // Protected Method Functions

        //- MPI_File_write [non-collective] : write data
        bool write_data
        (
            //! Source buffer
            const void* buffer,
            //! The data count - number of elements
            std::streamsize count,
            const UPstream::dataTypes dataTypeId
        );

        //- MPI_File_write_at [non-collective] : write data at specified offset
        bool write_data_at
        (
            //! The offset - number of 'view' elements (default: byte)
            std::streamsize offset,
            //! Source buffer
            const void* buffer,
            //! The data count - number of elements
            std::streamsize count,
            const UPstream::dataTypes dataTypeId
        );

        //- MPI_File_write_all [collective] : write data
        bool write_data_all
        (
            //! Source buffer
            const void* buffer,
            //! The data count - number of elements
            std::streamsize count,
            const UPstream::dataTypes dataTypeId
        );

        //- MPI_File_write_at_all [collective] :
        //- write data at specified offset
        bool write_data_at_all
        (
            //! The offset - number of 'view' elements (default: byte)
            std::streamsize offset,
            //! Source buffer
            const void* buffer,
            //! The data count - number of elements
            std::streamsize count,
            const UPstream::dataTypes dataTypeId
        );


public:

    // Generated Methods

        //- No copy construct
        File(const File&&) = delete;

        //- Move construct
        File(File&&) noexcept;

        //- No copy assignment
        File& operator=(const File&) = delete;

        //- Move assignment
        File& operator=(File&&) noexcept;


    // Constructors

        //- Default construct
        File();

        //- Destructor. Non-default in header (incomplete types)
        ~File();


    // Static Member Functions

        //- True if MPI/IO appears to be supported
        static bool supported();


    // Member Functions

    // Access

        //- The name of the open stream
        const fileName& name() const;

        //- True if allocated and open has been called
        bool is_open() const;


    // Basics

        //- MPI_File_open [collective] :
        //- open file in write-only mode, no-append
        bool open_write
        (
            //! The OpenFOAM communicator index
            const int communicator,
            //! Full file path (parent directory must exist before calling)
            const fileName& pathname,
            //! Simulated atomic file handling
            IOstreamOption::atomicType = IOstreamOption::NON_ATOMIC
        );

        //- MPI_File_close [collective]
        bool close();

        //- Set the (output) file size [collective]
        bool set_size(std::streamsize num_bytes);


    // Writing

        //- MPI_File_write [non-collective] : write data.
        //  A no-op and return true if content is empty
        inline bool write(std::string_view sv);

        //- MPI_File_write [non-collective] : write data.
        //  A no-op and return true if buffer is nullptr or count is zero
        template<class Type>
        inline bool write
        (
            //! The content
            const Type* buffer,
            //! The data count - number of elements
            std::streamsize count
        );

        //- MPI_File_write_at [non-collective] : write data at specified offset.
        //  A no-op and return true if content is empty
        inline bool write_at(std::streamsize offset, std::string_view sv);

        //- MPI_File_write_at [non-collective] : write data at specified offset.
        //  A no-op and return true if buffer is nullptr or count is zero
        template<class Type>
        inline bool write_at
        (
            //! The offset within the file - number of 'view' elements
            std::streamsize offset,
            //! The content
            const Type* buffer,
            //! The data count - number of elements
            std::streamsize count
        );

        //- MPI_File_write_all [collective] : write data
        inline bool write_all(std::string_view sv);

        //- MPI_File_write_all [collective] : write data
        template<class Type>
        inline bool write_all
        (
            //! The content
            const Type* buffer,
            //! The data count - number of elements
            std::streamsize count
        );

        //- MPI_File_write_at_all [collective] :
        //- write data at specified offset
        inline bool write_at_all
        (
            //! The offset within the file - number of 'view' elements
            std::streamsize offset,
            //! The content
            std::string_view sv
        );

        //- MPI_File_write_at_all [collective] :
        //- write data at specified offset
        template<class Type>
        inline bool write_at_all
        (
            //! The offset within the file - number of 'view' elements
            std::streamsize offset,
            //! The content
            const Type* buffer,
            //! The data count - number of elements
            std::streamsize count
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UPstreamFile.txx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
