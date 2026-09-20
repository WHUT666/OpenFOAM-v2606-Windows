/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2014,2024 OpenFOAM Foundation
    Copyright (C) 2017-2025 OpenCFD Ltd.
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
    Foam::UOPstreamBase

Description
    Base class for output inter-processor communications stream
    (ie, parallel streams).
    Not to be used directly, thus contructors are protected.

SourceFiles
    UOPstream.txx
    UOPstreamBase.C

\*---------------------------------------------------------------------------*/

#include "Pstream.H"

#ifndef Foam_UOPstream_H
#define Foam_UOPstream_H

#include "UPstream.H"
#include "Ostream.H"
#include "DynamicList.H"
#include "PstreamBuffers.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class UOPstreamBase Declaration
\*---------------------------------------------------------------------------*/

class OpenFOAM_API UOPstreamBase
:
    public UPstream,
    public Ostream
{
    // Private Member Functions

        //- Prepare send buffer for count bytes of output,
        //- with specified alignment. Alignment padding is filled with zero.
        inline void prepareBuffer(const size_t count, const size_t align);

        //- Generic write data to the send buffer, aligned by sizeof(T)
        template<class T>
        inline void writeToBuffer(const T& val);

        //- Write count bytes of data to the send buffer,
        //- using \p align byte alignment
        //- Writing with a null \p data pointer fills the output
        //- with the corresponding \p count values of zero.
        inline void writeToBuffer
        (
            const void* data,
            const size_t count,
            const size_t align
        );

        //- Write count bytes of data at absolute position in buffer.
        //- Position will be aligned to \p align boundary.
        //- Buffer is extended with nul-char fill if needed.
        //- Writing with a null \p data pointer fills with zeros.
        //- [ADVANCED USAGE]
        void writeToBuffer_at
        (
            //! The target output position (negative == use end position)
            int64_t pos,
            const void* data,
            const size_t count,
            const size_t align
        );

        //- Add a single char to the send buffer. No alignment needed
        inline void putChar(const char c);

        //- Write string length and content, including embedded nul chars.
        inline void putString(const char* str, const size_t len);

        //- Write string length and content, including embedded nul chars.
        inline void putString(const std::string& str);


protected:

    // Protected Data

        //- Destination rank for the data
        const int toProcNo_;

        //- Message tag for communication
        const int tag_;

        //- The communicator index
        const int comm_;

        //- Call bufferIPCsend on termination (in the destructor)
        bool sendAtDestruct_;

        //- Reference to the send buffer data
        DynamicList<char>& sendBuf_;


    // Protected Constructors

        //- Construct given process index to write to using the given
        //- attached send buffer, optional communication characteristics
        //- and IO format
        UOPstreamBase
        (
            const UPstream::commsTypes commsType,
            const int toProcNo,
            DynamicList<char>& sendBuf,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            const bool sendAtDestruct = true,
            IOstreamOption::streamFormat fmt = IOstreamOption::BINARY
        );

        //- Construct given buffers
        UOPstreamBase(const int toProcNo, PstreamBuffers& buffers);

        //- Construct for externally obtained buffers
        UOPstreamBase
        (
            DynamicList<char>& sendBuf,
            IOstreamOption::streamFormat fmt
        );

public:

    //- Destructor.
    virtual ~UOPstreamBase();


    // Member Functions

        //- The target rank
        int target() const noexcept { return toProcNo_; }


    // Stream State Functions

        //- Return current stream flags.
        //- Dummy for parallel stream, returns 0.
        virtual std::ios_base::fmtflags flags() const override
        {
            return std::ios_base::fmtflags(0);
        }

        //- Set stream flags, return old stream flags.
        //- Dummy for parallel stream, returns 0.
        virtual std::ios_base::fmtflags flags(std::ios_base::fmtflags) override
        {
            return std::ios_base::fmtflags(0);
        }


    // Write Functions

        //- Inherit write methods from Ostream
        using Ostream::writeQuoted;

        //- Write token to stream or otherwise handle it.
        //  \return false if the token type was not handled by this method
        virtual bool write(const token& tok) override;

        //- Write single character. Whitespace is suppressed.
        virtual Ostream& write(const char c) override;

        //- Write character/string content, with/without surrounding quotes
        virtual Ostream& writeQuoted
        (
            const char* str,
            std::streamsize len,
            const bool quoted=true
        ) override;

        //- Write the word-characters of a character string.
        //  Sends as a single char, or as word.
        virtual Ostream& write(const char* str) override;

        //- Write word
        virtual Ostream& write(const word& str) override;

        //- Write string
        virtual Ostream& write(const std::string& str) override;

        //- Write int32_t
        virtual Ostream& write(const int32_t val) override;

        //- Write int64_t
        virtual Ostream& write(const int64_t val) override;

        //- Write uint32_t
        virtual Ostream& write(const uint32_t val) override;

        //- Write uint64_t
        virtual Ostream& write(const uint64_t val) override;

        //- Write float
        virtual Ostream& write(const float val) override;

        //- Write double
        virtual Ostream& write(const double val) override;

        //- Write binary block with 8-byte alignment,
        //- a no-op (also no alignment) if the \p count == 0.
        //- If \p data is a null pointer:
        //- an aligned fill with \p count values of \c nul.
        virtual Ostream& write
        (
            const char* data,
            std::streamsize count
        ) override;

        //- Low-level raw binary output (without alignment),
        //- a no-op if the \p count == 0.
        //- Writing with a null \p data pointer fills the output
        //- with the corresponding \p count values of zero.
        virtual Ostream& writeRaw
        (
            const char* data,
            std::streamsize count
        ) override;

        //- Begin marker for low-level raw binary output.
        //  The count indicates the number of bytes for subsequent
        //  writeRaw calls.
        virtual bool beginRawWrite(std::streamsize count) override;

        //- End marker for low-level raw binary output.
        virtual bool endRawWrite() override
        {
            return true;
        }

        //- Add indentation characters
        virtual void indent() override
        {}


        //- Write contiguous content as binary block with 8-byte alignment.
        template<class T>
        Ostream& write_binary(const T* buffer, std::streamsize count);

        //- Write single contiguous value as binary block with 8-byte alignment.
        template<class T>
        Ostream& write_binary(const T& value);

        //- Write contiguous content at absolute position in buffer.
        //- Position will be aligned on 8-byte boundary. If pos is beyond
        //- current output position, buffer is extended with nul-char fill.
        //- After writing, position is either restored to original if write
        //- did not extend, or moved to end of write if it did extend.
        //- [ADVANCED USAGE]
        template<class T>
        Ostream& write_at(int64_t pos, const T* buffer, std::streamsize count);

        //- Write single contiguous value as binary block at specified
        //- position in buffer (with 8-byte alignment).
        //- [ADVANCED USAGE]
        template<class T>
        Ostream& write_at(int64_t pos, const T& value);


    // Stream state functions

        //- Flush stream
        virtual void flush() override
        {}

        //- Add newline and flush stream
        virtual void endl() override
        {}

        //- Get the current padding character
        //  \return previous padding character
        virtual char fill() const override
        {
            return 0;
        }

        //- Set padding character for formatted field up to field width
        virtual char fill(const char) override
        {
            return 0;
        }

        //- Get width of output field
        virtual int width() const override
        {
            return 0;
        }

        //- Set width of output field
        //  \return previous width
        virtual int width(const int) override
        {
             return 0;
        }

        //- Get precision of output field
        virtual int precision() const override
        {
             return 0;
        }

        //- Set precision of output field
        //  \return old precision
        virtual int precision(const int) override
        {
             return 0;
        }


    // Positioning

        //- Align the output position (byte boundary) to specified amount.
        //- Fills padded area with nul char. [ADVANCED USAGE]
        void alignp(size_t alignment);

        //- Align the output position (byte boundary) for the data type.
        //- Fills padded area with nul char. [ADVANCED USAGE]
        template<class T>
        void alignp() { alignp(sizeof(T)); }

        //- The current output position in the output buffer.
        int64_t tellp() const noexcept { return sendBuf_.size(); }

        //- The current number of characters in the output buffer
        int64_t size() const noexcept { return sendBuf_.size(); }

        //- Rewind the send buffer for overwriting
        virtual void rewind();


    // Print

        //- Print stream description to Ostream
        void print(Ostream& os) const override;
};


/*---------------------------------------------------------------------------*\
                          Class UOPstream Declaration
\*---------------------------------------------------------------------------*/

//- Output inter-processor communications stream
//- using MPI send/recv etc. - operating on external buffer.
class OpenFOAM_API UOPstream
:
    public UOPstreamBase
{
    // Private Member Functions

        //- Buffer send, usually called by destructor
        bool bufferIPCsend();


public:

    // Constructors

        //- Construct given process index to write to using the given
        //- attached send buffer, optional communication characteristics
        //- and IO format
        UOPstream
        (
            const UPstream::commsTypes commsType,
            const int toProcNo,
            DynamicList<char>& sendBuf,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            const bool sendAtDestruct = true,
            IOstreamOption::streamFormat fmt = IOstreamOption::BINARY
        );

        //- Construct given buffers
        UOPstream(const int toProcNo, PstreamBuffers& buffers);

        //- Construct for writing into a standalone buffer.
        //- Data transfer is handled externally by the caller.
        explicit UOPstream
        (
            DynamicList<char>& sendBuf,
            IOstreamOption::streamFormat fmt = IOstreamOption::BINARY
        );


    //- Destructor, usually sends buffer on destruct.
    virtual ~UOPstream();


    // Member Functions

        //- Send buffer contents now and not in destructor [advanced usage].
        //- Returns true on success
        bool send();

        //- Use all write methods from base class
        using UOPstreamBase::write;


    // Static Functions

        //- Write buffer contents (contiguous types only) to given processor
        //  \return True on success
        template<class Type>
        static bool write
        (
            const UPstream::commsTypes commsType,
            const int toProcNo,
            const Type* buffer,
            std::streamsize count,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            //! [out] request information (for non-blocking)
            UPstream::Request* req = nullptr,
            //! optional send mode (normal | sync)
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );

        //- Write buffer contents (contiguous types only) to given processor
        //  \return True on success
        template<class Type>
        inline static bool write
        (
            //! [out] request information
            UPstream::Request& req,
            const int toProcNo,
            const Type* buffer,
            std::streamsize count,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );

        //- Send UList contiguous contents to given processor.
        template<class Type>
        inline static bool write
        (
            const UPstream::commsTypes commsType,
            const int toProcNo,
            const UList<Type>& buffer,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            //! [out] request information (for non-blocking)
            UPstream::Request* req = nullptr,
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );

        //- Send SubList contiguous contents to given processor.
        template<class Type>
        inline static bool write
        (
            const UPstream::commsTypes commsType,
            const int toProcNo,
            const SubList<Type> buffer,  // passed by shallow copy
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            //! [out] request information (for non-blocking)
            UPstream::Request* req = nullptr,
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );

        //- Send UList contiguous contents (non-blocking) to given processor.
        template<class Type>
        inline static bool write
        (
            //! [out] request information
            UPstream::Request& req,
            const int toProcNo,
            const UList<Type>& buffer,
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );

        //- Send SubList contiguous contents (non-blocking) to given processor.
        template<class Type>
        inline static bool write
        (
            //! [out] request information
            UPstream::Request& req,
            const int toProcNo,
            const SubList<Type> buffer,  // passed by shallow copy
            const int tag = UPstream::msgType(),
            const int communicator = UPstream::worldComm,
            const UPstream::sendModes sendMode = UPstream::sendModes::normal
        );
};


/*---------------------------------------------------------------------------*\
                         Class UOPBstream Declaration
\*---------------------------------------------------------------------------*/

//- Output inter-processor communications stream
//- using MPI broadcast - operating on external buffer.
//
// \note does not use commsType, tag etc.
class OpenFOAM_API UOPBstream
:
    public UOPstreamBase
{
    // Private Member Functions

        //- Buffer send, usually called by destructor
        bool bufferIPCsend();


public:

    // Constructors

        //- Construct with attached send buffer,
        //- optional communication characteristics and IO format
        explicit UOPBstream
        (
            DynamicList<char>& sendBuf,
            const int communicator = UPstream::worldComm,
            const bool sendAtDestruct = true,
            IOstreamOption::streamFormat fmt = IOstreamOption::BINARY
        );


    //- Destructor, usually sends buffer on destruct.
    virtual ~UOPBstream();


    // Member Functions

        //- Broadcast a zero value (buffer) size that can be matched
        //- by the UIPBstream constructor.
        static void send(Foam::zero, int communicator, int root=0);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "UOPstream.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
