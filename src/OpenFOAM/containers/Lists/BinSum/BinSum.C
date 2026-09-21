/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2012-2016 OpenFOAM Foundation
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

#include "BinSum.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class IndexType, class ListType, class CombineOp>
Foam::BinSum<IndexType, ListType, CombineOp>::BinSum
(
    const IndexType min,
    const IndexType max,
    const IndexType delta
)
:
    ListType(ceil((max-min)/delta), Zero),
    min_(min),
    max_(max),
    delta_(delta),
    lowSum_(Zero),
    highSum_(Zero)
{}


template<class IndexType, class ListType, class CombineOp>
Foam::BinSum<IndexType, ListType, CombineOp>::BinSum
(
    const IndexType min,
    const IndexType max,
    const IndexType delta,
    const UList<IndexType>& indexVals,
    const ListType& vals,
    const CombineOp& cop
)
:
    ListType(ceil((max-min)/delta), Zero),
    min_(min),
    max_(max),
    delta_(delta),
    lowSum_(Zero),
    highSum_(Zero)
{
    forAll(indexVals, i)
    {
        add(indexVals[i], vals[i], cop);
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class IndexType, class ListType, class CombineOp>
void Foam::BinSum<IndexType, ListType, CombineOp>::add
(
    const IndexType& indexVal,
    const typename ListType::const_reference val,
    const CombineOp& cop
)
{
    if (indexVal < min_)
    {
        cop(lowSum_, val);
    }
    else if (indexVal >= max_)
    {
        cop(highSum_, val);
    }
    else
    {
        label index = (indexVal-min_)/delta_;
        cop(this->operator[](index), val);
    }
}


template<class IndexType, class ListType, class CombineOp>
void Foam::BinSum<IndexType, ListType, CombineOp>::add
(
    const UList<IndexType>& indexVals,
    const ListType& vals,
    const CombineOp& cop
)
{
    forAll(indexVals, i)
    {
        add(indexVals[i], vals[i], cop);
    }
}


// ************************************************************************* //
