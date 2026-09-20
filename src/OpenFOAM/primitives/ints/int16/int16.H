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

Primitive
    int16_t

Description
    16bit signed integer. I/O is done as an int32.

SourceFiles
    int16.C

\*---------------------------------------------------------------------------*/

//- Define Foam_pTraits_defines_typeName in .C files that
//  define the template statics of pTraits (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_pTraits_defines_typeName)
#  define Foam_pTraits_typeName_API
#else
#  define Foam_pTraits_typeName_API OpenFOAM_API
#endif

#ifndef Foam_primitives_int16_H
#define Foam_primitives_int16_H

#include <cstdint>

#include "direction.H"
#include "pTraits.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- A word representation of int16 value
inline word name(const int16_t val)
{
    return word(std::to_string(int(val)), false); // Needs no stripping
}

//- A word representation of int16 value
template<>
struct nameOp<int16_t>
{
    word operator()(const int16_t val) const
    {
        return word(std::to_string(int(val)), false); // Needs no stripping
    }
};


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Istream& operator>>(Istream& is, int16_t& val);
Ostream& operator<<(Ostream& os, const int16_t val);


/*---------------------------------------------------------------------------*\
                       Specialization pTraits<int16_t>
\*---------------------------------------------------------------------------*/

//- Template specialization for pTraits<int16_t>
template<>
class pTraits<int16_t>
{
    int16_t p_;

public:

    // Typedefs

        //- Component type
        typedef int16_t cmptType;


    // Member Constants

        //- Dimensionality of space
        static constexpr direction dim = 3;

        //- Rank of int16_t is 0
        static constexpr direction rank = 0;

        //- Number of components in int16_t is 1
        static constexpr direction nComponents = 1;


    // Limits

        static constexpr int16_t min_ = INT16_MIN;
        static constexpr int16_t max_ = INT16_MAX;


    // Static Data Members

        static Foam_pTraits_typeName_API const char* const typeName;
        static Foam_pTraits_typeName_API const char* const componentNames[];
        static Foam_pTraits_typeName_API const int16_t zero;
        static Foam_pTraits_typeName_API const int16_t one;
        static Foam_pTraits_typeName_API const int16_t min;
        static Foam_pTraits_typeName_API const int16_t max;
        static Foam_pTraits_typeName_API const int16_t rootMax;
        static Foam_pTraits_typeName_API const int16_t rootMin;


    // Constructors

        //- Copy construct from primitive
        explicit pTraits(int16_t val) noexcept : p_(val) {}

        //- Read construct from Istream
        explicit pTraits(Istream& is);


    // Member Functions

        //- Return the value
        operator int16_t() const noexcept { return p_; }

        //- Access the value
        operator int16_t&() noexcept { return p_; }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
