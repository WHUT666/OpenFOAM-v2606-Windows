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
    Foam::labelRange

Description
    A range or interval of labels defined by a start and a size.

SourceFiles
    labelRange.cxx
    labelRangeI.H

\*---------------------------------------------------------------------------*/

#ifndef Foam_labelRange_H
#define Foam_labelRange_H

#include "IntRange.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class labelRange;
template<class T> class MinMax;
template<class T> class Pair;

//- Read from Istream as bracketed (start size) tuple
Istream& operator>>(Istream&, labelRange&);

//- Write to Ostream as bracketed (start size) tuple
Ostream& operator<<(Ostream&, const labelRange&);

/*---------------------------------------------------------------------------*\
                         Class labelRange Declaration
\*---------------------------------------------------------------------------*/

class labelRange
:
    public IntRange<label>
{
public:

    // STL type definitions

        //- Type of values the range contains
        typedef label value_type;

        //- The type that can represent the size of the range
        typedef label size_type;

        //- Input iterator with const access
        using IntRange<label>::const_iterator;

        //- Reverse input iterator with const access
        using IntRange<label>::const_reverse_iterator;


    // Static Data Members

        //- Debugging
        static OpenFOAM_API int debug;


    // Generated Methods

        //- Copy construct
        labelRange(const labelRange&) noexcept = default;

        //- Move construct
        labelRange(labelRange&&) noexcept = default;

        //- Copy assignment
        labelRange& operator=(const labelRange&) noexcept = default;

        //- Move assignment
        labelRange& operator=(labelRange&&) noexcept = default;


    // Constructors

        //- Default construct an empty range (0,0)
        inline constexpr labelRange() noexcept;

        //- Construct a range with specified length, starting at zero (0,len)
        inline explicit constexpr labelRange(label len) noexcept;

        //- Construct a range from start/length. Applies clamp on length
        inline labelRange(label beg, label len) noexcept;

        //- Construct a range from start/size, enforces non-negative size.
        //  Optionally adjust the start to avoid any negative indices.
        //  \see adjust()
        inline labelRange(label beg, label len, bool adjustStart) noexcept;

        //- Construct from a min/max range (both inclusive),
        //- enforces non-negative size.
        //  Passing an invalid range results in an empty labelRange
        explicit labelRange(const MinMax<label>& range) noexcept;

        //- Construct from start (inclusive) and end (exclusive) values,
        //- enforces non-negative size.
        //  Passing an invalid range results in an empty labelRange
        explicit labelRange(const Pair<label>& start_end) noexcept;

        //- Implicit construct from an IntRange with the same or lower
        //- representation (no checks for signed vs unsigned).
        template
        <
            class IntT,
            class = std::enable_if_t
            <std::is_integral_v<IntT> && (sizeof(IntT) <= sizeof(value_type))>
        >
        labelRange(const IntRange<IntT>& rhs) noexcept
        :
            IntRange<label>(rhs.start(), rhs.size())
        {}

        //- Construct from Istream. Read a bracketed (start size) tuple
        explicit labelRange(Istream& is);


    // Member Functions

    // Edit

        //- Adjust the start to avoid negative indices.
        //  The size is decreased accordingly, but will never become negative.
        //  Eg, adjusting (-10, 15) becomes (0,5).
        //  adjusting (-20, 15) becomes (0,0)
        void adjust() noexcept;

        //- Reset start and length, no checks
        using IntRange<label>::reset;

        //- Reset start and length, enforces non-negative size.
        //  Optionally adjust the start to avoid any negative indices.
        inline void reset(label beg, label len, bool adjustStart) noexcept;


    // Other

        //- Return true if the ranges overlap.
        //  Optional test for ranges that also just touch each other
        bool overlaps(const labelRange& range, bool touches=false) const;

        //- Return a joined range, squashing any gaps in between
        //  A prior overlaps() check can be used to avoid squashing gaps.
        labelRange join(const labelRange& range) const;

        //- Calculate the intersection of the range with another.
        //  If there is no intersection, it returns an empty range with zero
        //  for start/size.
        labelRange subset(const labelRange& range) const;

        //- Calculate the intersection with the given start/size range.
        //  If there is no intersection, it returns an empty range with zero
        //  for start/size.
        labelRange subset(label start, label size) const;

        //- Calculate the intersection with the given 0/size range.
        //  If there is no intersection, it returns an empty range with zero
        //  for start/size.
        inline labelRange subset0(label len) const;


    // Operators

        //- Assign from an IntRange with the same or lower
        //- representation (no checks for signed vs unsigned).
        template
        <
            class IntT,
            class = std::enable_if_t
            <std::is_integral_v<IntT> && (sizeof(IntT) <= sizeof(value_type))>
        >
        void operator=(const IntRange<IntT>& rhs) noexcept
        {
            IntRange<label>::reset(rhs.start(), rhs.size());
        }


    // Housekeeping

        //- Deprecated(2020-09) True if range has size greater than zero
        //
        //  \deprecated(2020-09) - use bool operator - or good()
        bool valid() const noexcept { return good(); }
};


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

//- Conversion/extraction to labelRange operation (functor).
//  Specializations shall provide a corresponding \c operator().
//  For example,
//  \code
//  template<>
//  struct labelRangeOp<polyPatch>
//  {
//      labelRange operator()(const polyPatch& pp) const
//      {
//          return labelRange(pp.start(), pp.size());
//      }
//  };
//  \endcode
template<class> struct labelRangeOp;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "labelRangeI.H"

#endif

// ************************************************************************* //
