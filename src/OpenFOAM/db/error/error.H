/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2015-2025 OpenCFD Ltd.
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
    Foam::error

Description
    Class to handle errors and exceptions in a simple, consistent stream-based
    manner.

    The error class is globally instantiated with a title string. Errors,
    messages and other data are piped to the messageStream class in the
    standard manner.  Manipulators are supplied for exit and abort that may
    terminate the program or throw an exception depending on whether the
    exception handling has been switched on (off by default).

Usage
    \code
        error << "message1" << "message2" << FoamDataType << exit(errNo);
        error << "message1" << "message2" << FoamDataType << abort();
    \endcode

SourceFiles
    error.C
    IOerror.C

SeeAlso
    Foam::errorManip
    Foam::errorManipArg

\*---------------------------------------------------------------------------*/

#ifndef Foam_error_H
#define Foam_error_H

#include "messageStream.H"
#include <memory>
#include <typeinfo>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class OpenFOAM_API prefixOSstream;
class OStringStream;
template<class T> class List;
template<class EnumType> class Enum;

/*---------------------------------------------------------------------------*\
                            Class error Declaration
\*---------------------------------------------------------------------------*/

class OpenFOAM_API error
:
    public std::exception,
    public messageStream
{
    // Private Member Functions

        //- Exit or abort code, with exception and jobinfo handling
        void exiting(const int errNo, const bool isAbort);


protected:

    // Protected Data

        string functionName_;
        string sourceFileName_;
        int sourceFileLineNumber_;
        bool throwing_;
        std::unique_ptr<OStringStream> messageStreamPtr_;


    // Protected Member Functions

        //- Exit or abort, without throwing or job control handling
        void simpleExit(const int errNo, const bool isAbort);


public:

    // Data Types

        //- Handling of errors. The exact handling depends on the local context.
        enum class handlerTypes : char
        {
            DEFAULT = 0,  //!< Default behaviour (local meaning)
            IGNORE,       //!< Ignore on errors/problems
            WARN,         //!< Warn on errors/problems
            STRICT        //!< Fatal on errors/problems
        };

        //- Names of the error handler types
        static const Enum<handlerTypes> handlerNames;


    // Constructors

        //- Construct from title string
        explicit error(const char* title);

        //- Construct from title string
        explicit error(const std::string& title) : error(title.c_str()) {}

        //- Construct from dictionary
        explicit error(const dictionary& errDict);

        //- Copy construct
        error(const error& err);


    //- Destructor
    virtual ~error() noexcept;


    // Static Functions

        //- Like Pstream::master but with a Pstream::parRun guard in case
        //- Pstream has not yet been initialised.
        //
        //  \param communicator is the numbered MPI communicator.
        //      By default it uses UPstream::worldComm
        static bool master(const int communicator = -1);

        //- Test if an age warning should be emitted.
        //  \param version is the old version (YYMM) for determining the
        //      age in months compared to the current OpenFOAM version
        //      as conveyed by the \c foamVersion::api value.
        static bool warnAboutAge(const int version) noexcept;

        //- Emit warning on stderr about something being old.
        //  \param what description for the warning
        //  \param version is the old version (YYMM) for determining the
        //      age in months compared to the current OpenFOAM version
        //      as conveyed by the \c foamVersion::api value.
        static bool warnAboutAge(const char* what, const int version);


    // Member Functions

        //- The accumulated error message
        string message() const;

        //- Clear any accumulated error messages
        void clear() const;

        //- The currently defined function name for output messages
        const string& functionName() const noexcept
        {
            return functionName_;
        }

        //- The currently defined source-file name for output messages
        const string& sourceFileName() const noexcept
        {
            return sourceFileName_;
        }

        //- The currently defined source-file line number for output messages
        int sourceFileLineNumber() const noexcept
        {
            return sourceFileLineNumber_;
        }

        //- Return the current exception throwing state (on or off)
        bool throwing() const noexcept
        {
            return throwing_;
        }

        //- Specify exception throwing state (on or off)
        //  \return the previous throwing state
        bool throwing(bool on) noexcept
        {
            bool old(throwing_);
            throwing_ = on;
            return old;
        }


    // Output

        //- Return OSstream for output operations
        OSstream& stream();

        //- Implicit cast to OSstream for << operations
        operator OSstream&()
        {
            return this->stream();
        }

        //- Explicit convert to OSstream for << operations
        OSstream& operator()()
        {
            return this->stream();
        }

        //- Define basic print message with originating function name,
        //- optionally with 'source file, line number'
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The originating function name
            std::string_view functionName,
            //! The source file name (optional)
            std::string_view sourceFileName = std::string_view(),
            //! The source file line number (optional)
            const int sourceFileLineNumber = -1
        );


    // Other

        //- Extract a dictionary representation of the error information
        operator dictionary() const;

        //- Demangle C++ symbol name (eg, name from typeid query).
        //  \note requires symbols only - without extra '(' etc.
        static word demangle(const char* symbol);

        //- Demangle C++ symbol name from typeid query.
        inline static word demangle(const std::type_info& ti)
        {
            return demangle(ti.name());
        }

        //- Demangle C++ symbol for given type.
        template<typename T>
        inline static word demangle()
        {
            return demangle(typeid(T).name());
        }

        //- Helper function to print a stack, with optional upper limit.
        //- Used when OpenFOAM IO not yet initialised.
        static void safePrintStack(std::ostream& os, int size = -1);

        //- Helper function to print a stack, with optional upper limit.
        static void printStack(Ostream& os, int size = -1);

        //- Return prefixOSstream with the current memory high-water mark
        //- (summed per-node).
        //- Return a dummy (null) stream if the rank does not correspond
        //- to a node leader.
        //  Caution: includes MPI communication
        static prefixOSstream& Pout_mem_hwm
        (
            //! The originating function name (optional)
            std::string_view functionName = std::string_view(),
            //! The source file name (optional)
            std::string_view sourceFileName = std::string_view(),
            //! The source file line number (optional)
            const int sourceFileLineNumber = -1
        );

        //- List of the per-node memory high-water mark values.
        //- These are summed per-node and aggregated across all node leaders.
        //  Caution: includes MPI communication
        static List<int64_t> list_mem_hwm
        (
            //! Use the max of any individual rank on the node instead
            //! of the total sum on the node.
            const bool useMaxValue = false
        );

        //- True if FOAM_ABORT is on.
        static bool useAbort();


        //- Exit : can be called for any error to exit program.
        //  Redirects to abort() when FOAM_ABORT is on.
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors.
        //  Prints stack before exiting.
        void abort();

        //- Print error message
        virtual void write(Ostream& os, const bool withTitle = true) const;


    // Housekeeping

        //- \deprecated(2021-11) use \c throwing(bool)
        bool throwExceptions(bool on = true) noexcept { return throwing(on); }

        //- \deprecated(2021-11) use \c throwing(false)
        bool dontThrowExceptions() noexcept { return throwing(false); }
};


/*---------------------------------------------------------------------------*\
                           Class IOerror Declaration
\*---------------------------------------------------------------------------*/

//- Report an I/O error
class OpenFOAM_API IOerror
:
    public error
{
    // Private Data

        string ioFileName_;
        label ioStartLineNumber_;
        label ioEndLineNumber_;


    // Private Member Functions

        //- Exit or abort code, with exception and jobinfo handling
        void exiting(const int errNo, const bool isAbort);


public:

    // Constructors

        //- Construct from title string
        explicit IOerror(const char* title);

        //- Construct from title string
        explicit IOerror(const std::string& title) : IOerror(title.c_str()) {}

        //- Construct from dictionary
        explicit IOerror(const dictionary& errDict);


    //- Destructor
    virtual ~IOerror() noexcept;


    // Member Functions

        //- The currently defined IO name for output messages
        const string& ioFileName() const noexcept
        {
            return ioFileName_;
        }

        //- The currently defined IO start-line number for output messages
        label ioStartLineNumber() const noexcept
        {
            return ioStartLineNumber_;
        }

        //- The currently defined IO end-line number
        label ioEndLineNumber() const noexcept
        {
            return ioEndLineNumber_;
        }


    // Output

        //- Define basic print message
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The originating function name
            std::string_view functionName,
            //! The source file name
            std::string_view sourceFileName,
            //! The source file line number
            const int sourceFileLineNumber,
            string ioFileName,
            const label ioStartLineNumber = -1,
            const label ioEndLineNumber = -1
        );

        //- Define basic print message for IO stream
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The originating function name
            std::string_view functionName,
            //! The source file name
            std::string_view sourceFileName,
            //! The source file line number
            const int sourceFileLineNumber,
            const IOstream& ioStream
        );

        //- Define basic print message for dictionary entries.
        //- Uses the dictionary::relativeName() on output.
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The originating function name
            std::string_view functionName,
            //! The source file name
            std::string_view sourceFileName,
            //! The source file line number
            const int sourceFileLineNumber,
            const dictionary& dict
        );


        //- Define basic print message for IO stream,
        //- independent of a source-file reference.
        //- Uses the dictionary::relativeName() on output.
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The context for the message
            std::string_view where,
            const IOstream& ioStream
        );

        //- Define basic print message for dictionary entries,
        //- independent of a source-file reference.
        //- Uses the dictionary::relativeName() on output.
        //  \return OSstream for further operations
        OSstream& operator()
        (
            //! The context for the message
            std::string_view where,
            const dictionary& dict
        );


        //- Print basic message and exit.
        //  Uses cerr if streams not yet constructed (at startup).
        //  Use in startup parsing instead of FatalError.
        static void SafeFatalIOError
        (
            //! The originating function name
            std::string_view functionName,
            //! The source file name
            std::string_view sourceFileName,
            //! The source file line number
            const int sourceFileLineNumber,
            const IOstream& ioStream,
            const std::string& msg
        );


    // Other

        //- Extract a dictionary representation of the error information
        operator dictionary() const;

        //- Exit : can be called for any error to exit program
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors
        void abort();

        //- Print error message
        virtual void write(Ostream& os, const bool withTitle = true) const;
};


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

//- Ostream operator
Ostream& operator<<(Ostream& os, const error& err);

//- Ostream operator
Ostream& operator<<(Ostream& os, const IOerror& err);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global error declarations: defined in error.C

//- Error stream (stdout output on all processes),
//- with additional 'FOAM FATAL ERROR' header text and stack trace.
extern OpenFOAM_API error   FatalError;

//- Error stream (stdout output on all processes),
//- with additional 'FOAM FATAL IO ERROR' header text and stack trace.
extern OpenFOAM_API IOerror FatalIOError;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Convenience macros to add the file name and line number to the function name

//- Report an error message using Foam::FatalError
//  for functionName in file __FILE__ at line __LINE__
#define FatalErrorIn(functionName)                                             \
    ::Foam::FatalError((functionName), __FILE__, __LINE__)

//- Report an error message using Foam::FatalError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define FatalErrorInFunction FatalErrorIn(FUNCTION_NAME)

//- Report an error message using Foam::FatalError
//  about unknown lookup type in table
#define FatalErrorInLookup(lookupTag, lookupName, lookupTable)                 \
    ::Foam::FatalError(FUNCTION_NAME, __FILE__, __LINE__)                      \
    << "Unknown " << (lookupTag) << " type " << (lookupName)                   \
    << "\n\nValid " << (lookupTag) << " types :\n"                             \
    << ((lookupTable).sortedToc()) << '\n'


//- Report an error message using Foam::FatalIOError
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define FatalIOErrorIn(functionName, ios)                                      \
    ::Foam::FatalIOError((functionName), __FILE__, __LINE__, (ios))

//- Report an error message using Foam::FatalIOError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define FatalIOErrorInFunction(ios) FatalIOErrorIn(FUNCTION_NAME, ios)


//- Report an error message using Foam::FatalIOError
//  about unknown lookup type in table
#define FatalIOErrorInLookup(ios, lookupTag, lookupName, lookupTable)          \
    ::Foam::FatalIOError(FUNCTION_NAME, __FILE__, __LINE__, (ios))             \
    << "Unknown " << (lookupTag) << " type " << (lookupName)                   \
    << "\n\nValid " << (lookupTag) << " types :\n"                             \
    << ((lookupTable).sortedToc()) << '\n'


//- Report an error message using Foam::FatalIOError
//  (or cerr if FatalIOError not yet constructed)
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SafeFatalIOErrorIn(functionName, ios, msg)                             \
    ::Foam::IOerror::SafeFatalIOError                                          \
    ((functionName), __FILE__, __LINE__, (ios), (msg))

//- Report an error message using Foam::FatalIOError
//  (or cerr if FatalIOError not yet constructed)
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SafeFatalIOErrorInFunction(ios, msg)                                   \
    SafeFatalIOErrorIn(FUNCTION_NAME, ios, msg)


//- Issue a FatalErrorIn for a function not currently implemented.
//  The functionName is printed and then abort is called.
//
//  This macro can be particularly useful when methods must be defined to
//  complete the interface of a derived class even if they should never be
//  called for this derived class.
#define notImplemented(functionName)                                           \
    FatalErrorIn(functionName)                                                 \
        << "Not implemented" << ::Foam::abort(FatalError);

//- Issue a FatalErrorIn for a function not currently implemented.
//  The FUNCTION_NAME is printed and then abort is called.
//
//  This macro can be particularly useful when methods must be defined to
//  complete the interface of a derived class even if they should never be
//  called for this derived class.
#define NotImplemented notImplemented(FUNCTION_NAME)

//- Report memory high-water mark using Foam::error::Pout_mem_hwm()
//- for functionName in file __FILE__ at line __LINE__.
//  Caution : includes MPI communication
#define PrintMemoryIn(functionName)                                            \
    ::Foam::error::Pout_mem_hwm((functionName), __FILE__,__LINE__)

//- Report memory high-water mark using Foam::error::Pout_mem_hwm()
//- for FUNCTION_NAME in file __FILE__ at line __LINE__
//  Caution : includes MPI communication
#define PrintMemoryInFunction                                                  \
    ::Foam::error::Pout_mem_hwm(FUNCTION_NAME, __FILE__, __LINE__)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "errorManip.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
