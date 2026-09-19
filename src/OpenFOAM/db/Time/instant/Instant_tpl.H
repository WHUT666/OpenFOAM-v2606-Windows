/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

Class
    Foam::Instant

Description
    A tuple of scalar value and key.
    The value often corresponds to a time value, thus the naming of the class.
    The key will usually be a time name or a file name etc.

\*---------------------------------------------------------------------------*/

#ifndef Foam_Instant_H
#define Foam_Instant_H

#include "scalar.H"
#include <utility>  // std::move

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class Instant Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class Instant
{
    // Private Data

        //- The value (eg, time)
        scalar val_;

        //- The name/key
        T key_;


public:

    // Public Classes

        //- Less function for sorting. Compares values only
        struct less
        {
            template<class T1, class T2>
            bool operator()
            (
                const Instant<T1>& a,
                const Instant<T2>& b
            ) const noexcept
            {
                return (a.value() < b.value());
            }
        };


    // Generated Methods

        //- Copy construct
        Instant(const Instant&) = default;

        //- Move construct
        Instant(Instant&&) = default;

        //- Copy assignment
        Instant& operator=(const Instant&) = default;

        //- Move assignment
        Instant& operator=(Instant&&) = default;


    // Constructors

        //- Default construct, with value = 0 and empty name
        Instant()
        :
            val_(0),
            key_()
        {}

        //- Copy construct from components
        Instant(scalar val, const T& key)
        :
            val_(val),
            key_(key)
        {}

        //- Move construct from components
        Instant(scalar val, T&& key)
        :
            val_(val),
            key_(std::move(key))
        {}


    // Member Functions

        //- The value (const access)
        scalar value() const noexcept { return val_; }

        //- The value (non-const access)
        scalar& value() noexcept { return val_; }

        //- The name/key (const access)
        const T& name() const noexcept { return key_; }

        //- The name/key (non-const access)
        T& name() noexcept { return key_; }


    // Comparison Methods (Static)

        //- Are values equal within (SMALL) rounding tolerance?
        static constexpr bool equal_to(scalar a, scalar b) noexcept
        {
            return ((a > b - SMALL) && (a < b + SMALL));
        }

        //- Are values less-equal within (SMALL) rounding tolerance?
        static constexpr bool less_equal(scalar a, scalar b) noexcept
        {
            return (a < b + SMALL);
        }

        //- Are values greater_equal within (SMALL) rounding tolerance?
        static constexpr bool greater_equal(scalar a, scalar b) noexcept
        {
            return (a > b - SMALL);
        }


    // Comparison Methods

        //- Is instant value equal to \p val within (SMALL) rounding?
        bool equal(scalar val) const noexcept
        {
            return equal_to(this->value(), val);
        }

        //- Is instant value equal to \p other within (SMALL) rounding?
        template<class T2>
        bool equal(const Instant<T2>& other) const noexcept
        {
            return equal_to(this->value(), other.value());
        }

        //- Is instant less-equal than \p val within (SMALL) rounding?
        bool less_equal(scalar val) const noexcept
        {
            return less_equal(this->value(), val);
        }

        //- Is instant less-equal than \p other within (SMALL) rounding?
        template<class T2>
        bool less_equal(const Instant<T2>& other) const noexcept
        {
            return less_equal(this->value(), other.value());
        }

        //- Is instant greater-equal than \p val within (SMALL) rounding?
        bool greater_equal(scalar val) const noexcept
        {
            return less_equal(val, this->value());
        }

        //- Is instant greater-equal than \p other within (SMALL) rounding?
        template<class T2>
        bool greater_equal(const Instant<T2>& other) const noexcept
        {
            return less_equal(other.value(), this->value());
        }


    // Member Operators

        //- Is instant less-equal than \p val within (SMALL) rounding?
        bool operator<=(scalar val) const noexcept
        {
            return less_equal(this->value(), val);
        }

        //- Is instant less-equal than \p other within (SMALL) rounding?
        template<class T2>
        bool operator<=(const Instant<T2>& other) const noexcept
        {
            return less_equal(this->value(), other.value());
        }

        //- Is instant greater-equal than \p val within (SMALL) rounding?
        bool operator>=(scalar val) const noexcept
        {
            return greater_equal(this->value(), val);
        }

        //- Is instant greater-equal than \p other within (SMALL) rounding?
        template<class T2>
        bool operator>=(const Instant<T2>& other) const noexcept
        {
            return greater_equal(this->value(), other.value());
        }
};


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

//- True if instant values are equal within rounding tolerance
template<class T1, class T2>
inline bool operator==(const Instant<T1>& a, const Instant<T2>& b) noexcept
{
    return a.equal(b.value());
}


//- True if instant values are not equal within rounding tolerance
template<class T1, class T2>
inline bool operator!=(const Instant<T1>& a, const Instant<T2>& b) noexcept
{
    return !a.equal(b.value());
}


template<class T1, class T2>
inline bool operator<(const Instant<T1>& a, const Instant<T2>& b) noexcept
{
    return (a.value() < b.value());
}


template<class T1, class T2>
inline bool operator>(const Instant<T1>& a, const Instant<T2>& b) noexcept
{
    return (b.value() < a.value());
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

//- Read instant tuple from Istream
template<class T>
inline Istream& operator>>(Istream& is, Instant<T>& inst)
{
    is >> inst.value() >> inst.name();
    return is;
}


//- Write instant tuple to Ostream
template<class T>
inline Ostream& operator<<(Ostream& os, const Instant<T>& inst)
{
    os << inst.value() << '\t' << inst.name();
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
