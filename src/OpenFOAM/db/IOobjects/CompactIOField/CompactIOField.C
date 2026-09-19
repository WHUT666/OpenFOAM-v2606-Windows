/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2018-2025 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "CompactIOField.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
bool Foam::CompactIOField<Type>::readIOcontents(bool readOnProc)
{
    typedef IOField<Type> plain_type;

    if (isReadRequired() || (isReadOptional() && headerOk()))
    {
        Istream& is = readStream(word::null, readOnProc);

        if (!readOnProc)
        {
            // no-op
        }
        else if (isHeaderClass(typeName))
        {
            // Compact form
            is >> *this;  // or: this->readCompact(is);
        }
        else if (isHeaderClass<plain_type>())
        {
            // Non-compact form
            is >> static_cast<content_type&>(*this);
        }
        else
        {
            FatalIOErrorInFunction(is)
                << "Unexpected class name " << headerClassName()
                << " expected " << typeName
                << " or " << plain_type::typeName << nl
                << "    while reading object " << name()
                << exit(FatalIOError);
        }

        close();
        return true;
    }

    return false;
}


template<class Type>
Foam::label Foam::CompactIOField<Type>::readIOsize(bool readOnProc)
{
    typedef IOField<Type> plain_type;

    label count(-1);

    if (isReadRequired() || (isReadOptional() && headerOk()))
    {
        Istream& is = readStream(word::null, readOnProc);

        if (!readOnProc)
        {
            // no-op
        }
        else
        {
            token tok(is);

            if (tok.isLabel())
            {
                // The majority of files will have lists with sizing prefix
                count = tok.labelToken();

                if (isHeaderClass(typeName))
                {
                    // Compact form: read offsets, not content
                    if (--count < 0)
                    {
                        count = 0;
                    }
                }
            }
            else
            {
                is.putBack(tok);

                if (isHeaderClass(typeName))
                {
                    // Compact form: can just read the offsets
                    labelList offsets(is);
                    count = Foam::max(0, (offsets.size()-1));
                }
                else if (isHeaderClass<plain_type>())
                {
                    // Non-compact form: need to read everything
                    Field<Type> list(is);
                    count = list.size();
                }
                else
                {
                    FatalIOErrorInFunction(is)
                        << "Unexpected class name " << headerClassName()
                        << " expected " << typeName
                        << " or " << plain_type::typeName << endl
                        << "    while reading object " << name()
                        << exit(FatalIOError);
                }
            }
        }
        close();
    }

    return count;
}


template<class Type>
bool Foam::CompactIOField<Type>::overflows() const
{
    // Can safely assume that int64 will not overflow
    if constexpr (sizeof(label) < sizeof(int64_t))
    {
        const UList<Type>& lists = *this;

        label total = 0;
        for (const auto& sublist : lists)
        {
            const label prev = total;
            total += sublist.size();
            if (total < prev)
            {
                return true;
            }
        }
    }
    return false;
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class Type>
Foam::CompactIOField<Type>::CompactIOField(const IOobject& io)
:
    regIOobject(io)
{
    readIOcontents();
}


template<class Type>
Foam::CompactIOField<Type>::CompactIOField
(
    const IOobject& io,
    const bool readOnProc
)
:
    regIOobject(io)
{
    readIOcontents(readOnProc);
}


template<class Type>
Foam::CompactIOField<Type>::CompactIOField
(
    const IOobject& io,
    Foam::zero
)
:
    regIOobject(io)
{
    readIOcontents();
}


template<class Type>
Foam::CompactIOField<Type>::CompactIOField
(
    const IOobject& io,
    const label len
)
:
    regIOobject(io)
{
    if (!readIOcontents())
    {
        Field<Type>::resize(len);
    }
}


template<class Type>
Foam::CompactIOField<Type>::CompactIOField
(
    const IOobject& io,
    const UList<Type>& content
)
:
    regIOobject(io)
{
    if (!readIOcontents())
    {
        Field<Type>::operator=(content);
    }
}


template<class Type>
Foam::CompactIOField<Type>::CompactIOField
(
    const IOobject& io,
    Field<Type>&& content
)
:
    regIOobject(io)
{
    Field<Type>::transfer(content);

    readIOcontents();
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template<class Type>
Foam::label Foam::CompactIOField<Type>::readContentsSize(const IOobject& io)
{
    IOobject rio(io, IOobjectOption::NO_REGISTER);
    if (rio.readOpt() == IOobjectOption::READ_MODIFIED)
    {
        rio.readOpt(IOobjectOption::MUST_READ);
    }
    rio.resetHeader();

    // Construct NO_READ, changing after construction
    const auto rOpt = rio.readOpt(IOobjectOption::NO_READ);

    CompactIOField<Type> reader(rio);
    reader.readOpt(rOpt);

    return reader.readIOsize();
}


template<class Type>
Foam::Field<Type> Foam::CompactIOField<Type>::readContents(const IOobject& io)
{
    IOobject rio(io, IOobjectOption::NO_REGISTER);
    if (rio.readOpt() == IOobjectOption::READ_MODIFIED)
    {
        rio.readOpt(IOobjectOption::MUST_READ);
    }
    rio.resetHeader();

    CompactIOField<Type> reader(rio);

    return Field<Type>(std::move(static_cast<Field<Type>&>(reader)));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool Foam::CompactIOField<Type>::writeObject
(
    IOstreamOption streamOpt,
    const bool writeOnProc
) const
{
    if
    (
        streamOpt.format() == IOstreamOption::BINARY
     && overflows()
    )
    {
        streamOpt.format(IOstreamOption::ASCII);

        WarningInFunction
            << "Overall number of elements of CompactIOField (size:"
            << this->size() << ") overflows a label (int"
            << (8*sizeof(label)) << ')' << nl
            << "    Switching to ascii writing" << endl;
    }

    if (streamOpt.format() != IOstreamOption::BINARY)
    {
        // Change type to be non-compact format type
        const word oldTypeName(typeName);

        const_cast<word&>(typeName) = IOField<Type>::typeName;

        bool good = regIOobject::writeObject(streamOpt, writeOnProc);

        // Restore type
        const_cast<word&>(typeName) = oldTypeName;

        return good;
    }

    return regIOobject::writeObject(streamOpt, writeOnProc);
}


template<class Type>
bool Foam::CompactIOField<Type>::writeData(Ostream& os) const
{
    return (os << *this).good();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
Foam::Istream& Foam::CompactIOField<Type>::readCompact(Istream& is)
{
    Field<Type>& lists = *this;

    // The base type for packed values
    typedef typename Type::value_type base_type;

    // Read compact: offsets + packed values
    const labelList offsets(is);
    Field<base_type> values(is);

    // Transcribe
    const label len = Foam::max(0, (offsets.size()-1));
    lists.resize_nocopy(len);

    auto iter = values.begin();

    for (label i = 0; i < len; ++i)
    {
        auto& list = lists[i];
        const label count = (offsets[i+1] - offsets[i]);

        list.resize_nocopy(count);

        std::move(iter, iter + count, list.begin());

        iter += count;
    }

    return is;
}


template<class Type>
Foam::Ostream& Foam::CompactIOField<Type>::writeCompact(Ostream& os) const
{
    const Field<Type>& lists = *this;

    // The base type for packed values
    typedef typename Type::value_type base_type;

    // Convert to compact format
    label total = 0;
    const label len = lists.size();

    // offsets
    {
        labelList offsets(len+1);

        for (label i = 0; i < len; ++i)
        {
            offsets[i] = total;
            total += lists[i].size();

            if (total < offsets[i])
            {
                FatalIOErrorInFunction(os)
                    << "Overall number of elements of CompactIOField (size:"
                    << len
                    << ") overflows the representation of a label" << nl
                    << "Please recompile with a larger representation"
                    << " for label" << exit(FatalIOError);
            }
        }
        offsets[len] = total;
        os << offsets;
    }

    // packed values: make deepCopy for writing
    {
        Field<base_type> values(total);

        auto iter = values.begin();

        for (const auto& list : lists)
        {
            iter = std::copy_n(list.begin(), list.size(), iter);

            // With IndirectList? [unlikely]
            // const label count = list.size();
            // for (label i = 0; i < count; (void)++i, (void)++iter)
            // {
            //     *iter = list[i];
            // }
        }
        os << values;
    }

    return os;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Type>
Foam::Istream& Foam::operator>>
(
    Foam::Istream& is,
    Foam::CompactIOField<Type>& lists
)
{
    return lists.readCompact(is);
}


template<class Type>
Foam::Ostream& Foam::operator<<
(
    Foam::Ostream& os,
    const Foam::CompactIOField<Type>& lists
)
{
    // Keep ASCII writing same
    if (os.format() != IOstreamOption::BINARY)
    {
        os << static_cast<const Field<Type>&>(lists);
    }
    else
    {
        lists.writeCompact(os);
    }

    return os;
}


// ************************************************************************* //
