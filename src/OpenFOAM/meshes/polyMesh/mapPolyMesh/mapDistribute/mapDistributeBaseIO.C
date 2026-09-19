/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2022-2026 OpenCFD Ltd.
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

#include "mapDistributeBase.H"
#include <dictionary.H>

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

namespace Foam
{

// The maps (labelListList) are not human-modifiable but if we need to
// inspect them in ASCII, it is much more convenient if each sub-list
// is flattened on a single line. Also reduce some extraneous newlines
// for binary output.
static Ostream& printMaps(Ostream& os, const labelListList& maps)
{
    if (maps.empty())
    {
        os << maps;
    }
    else
    {
        // Leading space (newline) handled by the caller
        os << maps.size() << nl << token::BEGIN_LIST << nl;

        // Compact single-line output for each labelList (ascii)
        // or with less whitespace (binary) - labelList is contiguous

        for (const auto& list : maps)
        {
            if (os.format() == IOstreamOption::BINARY)
            {
                const label len = list.size();

                os << len;
                if (len)
                {
                    // write(...) includes surrounding start/end delimiters
                    os.write(list.cdata_bytes(), list.size_bytes());
                }
                os << nl;
            }
            else
            {
                list.writeList(os) << nl;
            }
        }
        os << token::END_LIST;
    }

    return os;
}


// Read from sub-dictionary
static void readMapEntries
(
    const dictionary& dict,
    const word& mapName,
    bool& hasFlip,
    labelListList& maps
)
{
    const auto& subdict = dict.subDict(mapName);

    subdict.readEntry("flip", hasFlip);
    subdict.readEntry("maps", maps);
}


// Write as sub-dictionary content
static void writeMapEntries
(
    Ostream& os,
    const word& mapName,
    const bool hasFlip,
    const labelListList& maps
)
{
    os << nl;
    os.beginBlock(mapName);
    os.writeEntry("flip", hasFlip);
    if (maps.empty())
    {
        os.writeEntry("maps", maps);
    }
    else
    {
        os << indent << "maps" << nl;
        printMaps(os, maps);
        os.endEntry();
    }
    os.endBlock();
}

} // End namespace Foam


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

void Foam::mapDistributeBase::writeMap
(
    Ostream& os,
    const labelListList& mapElements
)
{
    printMaps(os, mapElements) << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::mapDistributeBase::mapDistributeBase
(
    const dictionary& dict,
    const label comm
)
:
    mapDistributeBase(comm)
{
    mapDistributeBase::readDict(dict);
}


Foam::mapDistributeBase::mapDistributeBase(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::mapDistributeBase::readDict(const dictionary& dict)
{
    dict.readEntry("constructSize", constructSize_);
    readMapEntries(dict, "subMap", subHasFlip_, subMap_);
    readMapEntries(dict, "constructMap", constructHasFlip_, constructMap_);
}


void Foam::mapDistributeBase::writeConstructMap(Ostream& os) const
{
    writeMap(os, constructMap_);
}


void Foam::mapDistributeBase::writeSubMap(Ostream& os) const
{
    writeMap(os, subMap_);
}


void Foam::mapDistributeBase::writeEntries(Ostream& os) const
{
    os.writeEntry("constructSize", constructSize_);
    //os.writeEntry("communicator", comm_);
    writeMapEntries(os, "subMap", subHasFlip_, subMap_);
    writeMapEntries(os, "constructMap", constructHasFlip_, constructMap_);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>(Istream& is, mapDistributeBase& map)
{
    is.fatalCheck(FUNCTION_NAME);

    is  >> map.constructSize_
        >> map.subMap_ >> map.constructMap_
        >> map.subHasFlip_ >> map.constructHasFlip_
        >> map.comm_;

    return is;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const mapDistributeBase& map)
{
    os  << map.constructSize() << nl;

    map.writeSubMap(os);
    map.writeConstructMap(os);

    // Flips as y/n instead of 1/0 (readability)
    os  << (map.subHasFlip() ? 'y' : 'n') << token::SPACE
        << (map.constructHasFlip() ? 'y' : 'n') << token::SPACE
        << map.comm() << nl;

    return os;
}


template<>
Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const InfoProxy<mapDistributeBase>& iproxy
)
{
    const auto& map = *iproxy;

    // Output as compact pseudo dictionary entries

    os.writeEntry("constructSize", map.constructSize());

    os  << indent << "local  { flip " << (map.subHasFlip() ? 'y' : 'n')
        << "; sizes ";
    map.subMapSizes().writeList(os) << "; }" << nl;

    os  << indent << "remote { flip " << (map.constructHasFlip() ? 'y' : 'n')
        << "; sizes ";
    map.constructMapSizes().writeList(os) << "; }" << nl;

    return os;
}


// ************************************************************************* //
