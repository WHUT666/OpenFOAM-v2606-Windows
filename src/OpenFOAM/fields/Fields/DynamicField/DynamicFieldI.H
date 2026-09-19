/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2016-2025 OpenCFD Ltd.
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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type, int SizeMin>
template<class ListType>
inline void Foam::DynamicField<Type, SizeMin>::doAssignDynList
(
    const ListType& list
)
{
    const label len = list.size();

    if (capacity_ < len)
    {
        // Needs more space for the copy operation
        List<Type>::setAddressableSize(capacity_);
        List<Type>::resize_nocopy(len);
        capacity_ = List<Type>::size();
    }

    // Perform copy into addressable portion
    List<Type>::setAddressableSize(len);
    List<Type>::operator=(list);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::doCapacity_copy
(
    label count,
    const label newCapacity
)
{
    if (newCapacity == capacity_)
    {
        return;
    }

    // Addressable length, possibly truncated by new capacity
    const label currLen = std::min(List<Type>::size(), newCapacity);

    // The count truncated by the new addressable range
    count = std::min(count, currLen);

    // Consistent allocated sizing
    List<Type>::setAddressableSize(capacity_);

    if (count > 0)
    {
        List<Type>::resize_copy(count, newCapacity);
    }
    else
    {
        List<Type>::resize_nocopy(newCapacity);
    }

    capacity_ = List<Type>::size();
    List<Type>::setAddressableSize(currLen);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::doReserve_copy
(
    label count,
    const label len
)
{
    if (capacity_ < len)
    {
        // Preserve addressed size
        const label currLen = List<Type>::size();

        // The count truncated by the new addressable range
        count = std::min(count, currLen);

        // Consistent allocated sizing
        List<Type>::setAddressableSize(capacity_);

        // Increase capacity (eg, doubling)
        capacity_ =
            Foam::ListPolicy::reserve_size<SizeMin, 2>(len, capacity_);

        if (count > 0)
        {
            List<Type>::resize_copy(count, capacity_);
        }
        else
        {
            List<Type>::resize_nocopy(capacity_);
        }

        capacity_ = List<Type>::size();
        List<Type>::setAddressableSize(currLen);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline constexpr Foam::DynamicField<Type, SizeMin>::DynamicField() noexcept
:
    Field<Type>(),
    capacity_(0)
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField(const label initialCapacity)
:
    Field<Type>(),
    capacity_(0)
{
    reserve_nocopy(initialCapacity);
}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    std::pair<label,label> sizing
)
:
    Field<Type>(std::max(sizing.first, sizing.second)),
    capacity_(Field<Type>::size())
{
    List<Type>::setAddressableSize(sizing.first);
}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const label len,
    const Type& val
)
:
    Field<Type>(len, val),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const label len,
    Foam::zero
)
:
    Field<Type>(len, Foam::zero{}),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const DynamicField<Type, SizeMin>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const DynamicField<Type, AnySizeMin>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<class Addr>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const IndirectListBase<Type, Addr>& list
)
:
    Field<Type>(list),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    List<Type>&& content
) noexcept
:
    Field<Type>(std::move(content)),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    DynamicList<Type, AnySizeMin>&& content
) noexcept
:
    Field<Type>(std::move(static_cast<List<Type>&>(content))),
    capacity_(content.capacity())
{
    content.setCapacity_unsafe(0);  // All contents moved
}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    DynamicField<Type, SizeMin>&& content
) noexcept
:
    Field<Type>(std::move(static_cast<List<Type>&>(content))),
    capacity_(content.capacity())
{
    content.setCapacity_unsafe(0);  // All contents moved
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    DynamicField<Type, AnySizeMin>&& content
) noexcept
:
    Field<Type>(std::move(static_cast<List<Type>&>(content))),
    capacity_(content.capacity())
{
    content.setCapacity_unsafe(0);  // All contents moved
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    DynamicField<Type, AnySizeMin>& content,
    bool reuse
)
:
    Field<Type>(),
    capacity_(0)
{
    if (reuse)
    {
        Field<Type>::transfer(static_cast<List<Type>&>(content));
        capacity_ = content.capacity();
        content.setCapacity_unsafe(0);  // All contents moved
    }
    else
    {
        Field<Type>::operator=(content);
        capacity_ = content.size();
    }
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    DynamicList<Type, AnySizeMin>& content,
    bool reuse
)
:
    Field<Type>(),
    capacity_(0)
{
    if (reuse)
    {
        Field<Type>::transfer(static_cast<List<Type>&>(content));
        capacity_ = content.capacity();
        content.setCapacity_unsafe(0);  // All contents moved
    }
    else
    {
        Field<Type>::operator=(content);
        capacity_ = content.size();
    }
}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    List<Type>& content,
    bool reuse
)
:
    Field<Type>(),
    capacity_(0)
{
    if (reuse)
    {
        Field<Type>::transfer(content);
    }
    else
    {
        Field<Type>::operator=(content);
    }
    capacity_ = Field<Type>::size();
}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const labelUList& mapAddressing
)
:
    Field<Type>(mapF, mapAddressing),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const labelListList& mapAddressing,
    const scalarListList& weights
)
:
    Field<Type>(mapF, mapAddressing, weights),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField
(
    const UList<Type>& mapF,
    const FieldMapper& map
)
:
    Field<Type>(mapF, map),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::DynamicField<Type, SizeMin>::DynamicField(Istream& is)
:
    Field<Type>(is),
    capacity_(Field<Type>::size())
{}


template<class Type, int SizeMin>
inline Foam::tmp<Foam::DynamicField<Type, SizeMin>>
Foam::DynamicField<Type, SizeMin>::clone() const
{
    return tmp<DynamicField<Type, SizeMin>>::New(*this);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline std::streamsize
Foam::DynamicField<Type, SizeMin>::capacity_bytes() const noexcept
{
    return std::streamsize(capacity_)*sizeof(Type);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::setCapacity
(
    const label len
)
{
    // Preserve all content
    doCapacity_copy(List<Type>::size(), len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::setCapacity_nocopy
(
    const label len
)
{
    // Preserve 0 content
    doCapacity_copy(0, len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::reserve
(
    const label len
)
{
    // Preserve all content
    doReserve_copy(List<Type>::size(), len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::reserve_nocopy
(
    const label len
)
{
    // Preserve 0 content
    doReserve_copy(0, len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::reserve_exact
(
    const label len
)
{
    if (capacity_ < len)
    {
        // Preserve addressed size
        const label currLen = List<Type>::size();

        // Consistent allocated sizing
        List<Type>::setAddressableSize(capacity_);

        // Exact length
        capacity_ = len;

        // if (!nocopy)
        {
            List<Type>::resize_copy(currLen, capacity_);
        }

        capacity_ = List<Type>::size();
        List<Type>::setAddressableSize(currLen);
    }
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::resize
(
    const label len
)
{
    // Preserve all content
    resize_copy(List<Type>::size(), len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::resize_copy
(
    label count,
    const label len
)
{
    // Preserve count elements
    doReserve_copy(count, len);
    List<Type>::setAddressableSize(len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::resize_fill
(
    const label len,
    const Type& val
)
{
    resize_nocopy(len);
    UList<Type>::operator=(val);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::resize_nocopy
(
    const label len
)
{
    // Preserve 0 content
    resize_copy(0, len);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::resize
(
    const label len,
    const Type& val
)
{
    const label oldLen = List<Type>::size();

    // Preserve all content
    resize(len);

    // Fill newly exposed with constant value
    if (oldLen < List<Type>::size())
    {
        std::fill
        (
            this->begin(oldLen), this->end(), val
        );
    }
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::clear() noexcept
{
    List<Type>::setAddressableSize(0);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::clearStorage()
{
    // Extra safety...?
    if (!List<Type>::cdata() && List<Type>::empty()) capacity_ = 0;

    // Consistent allocated sizing
    List<Type>::setAddressableSize(capacity_);
    List<Type>::clear();
    capacity_ = 0;
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::shrink_to_fit()
{
    const label currLen = List<Type>::size();
    if (currLen < capacity_)
    {
        List<Type>::setAddressableSize(capacity_);
        List<Type>::resize(currLen);
        capacity_ = List<Type>::size();
    }
}


template<class Type, int SizeMin>
inline void
Foam::DynamicField<Type, SizeMin>::swap(List<Type>& other)
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const List<Type>*>(this)
         == static_cast<const List<Type>*>(&other)
        )
    )
    {
        return;  // Self-swap is a no-op
    }

    // Remove unused storage
    this->shrink_to_fit();

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Update capacity
    capacity_ = List<Type>::size();
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::swap
(
    DynamicField<Type, AnySizeMin>& other
) noexcept
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const List<Type>*>(this)
         == static_cast<const List<Type>*>(&other)
        )
    )
    {
        return;  // Self-swap is a no-op
    }

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Swap capacity
    std::swap(this->capacity_, other.capacity_);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::swap
(
    DynamicList<Type, AnySizeMin>& other
) noexcept
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const List<Type>*>(this)
         == static_cast<const List<Type>*>(&other)
        )
    )
    {
        return;  // Self-swap is a no-op
    }

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Swap capacity
    auto old = capacity_;
    this->setCapacity_unsafe(other.capacity());
    other.setCapacity_unsafe(old);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::transfer(List<Type>& list)
{
    // No check for self-assignment (different types)

    // Consistent allocated sizing
    List<Type>::setAddressableSize(capacity_);
    Field<Type>::transfer(list);
    capacity_ = Field<Type>::size();
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::transfer
(
    DynamicList<Type, AnySizeMin>& other
)
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const UList<Type>*>(this)
         == static_cast<const UList<Type>*>(&other)
        )
    )
    {
        return;  // Self-assignment is a no-op
    }

    // Remove storage
    this->clearStorage();

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Update capacity
    capacity_ = other.capacity();
    other.setCapacity_unsafe(0);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::transfer
(
    DynamicField<Type, AnySizeMin>& other
)
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const UList<Type>*>(this)
         == static_cast<const UList<Type>*>(&other)
        )
    )
    {
        return;  // Self-assignment is a no-op
    }

    // Remove storage
    this->clearStorage();

    // Swap storage and addressable size
    UList<Type>::swap(other);

    // Update capacity
    capacity_ = other.capacity();
    other.setCapacity_unsafe(0);
}


template<class Type, int SizeMin>
template<class... Args>
inline Type& Foam::DynamicField<Type, SizeMin>::emplace_back(Args&&... args)
{
    // This could/should be better with inplace construction
    // (as per std::vector), but currently lacking the methods for that
    // so resize and move assign

    const label idx = List<Type>::size();
    resize(idx + 1);

    // move assign element
    this->operator[](idx) = Type(std::forward<Args>(args)...);
    return this->operator[](idx);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::push_back
(
    const Type& val
)
{
    const label idx = List<Type>::size();
    resize(idx + 1);

    this->operator[](idx) = val;  // copy element
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::push_back
(
    Type&& val
)
{
    const label idx = List<Type>::size();
    resize(idx + 1);

    this->operator[](idx) = std::move(val);  // move assign element
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::push_back
(
    const UList<Type>& list
)
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const UList<Type>*>(this)
         == static_cast<const UList<Type>*>(&list)
        )
    )
    {
        FatalErrorInFunction
            << "Attempted push_back to self"
            << abort(FatalError);
    }

    const label idx = List<Type>::size();
    resize(idx + list.size());

    std::copy(list.begin(), list.end(), this->begin(idx));
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::push_back
(
    List<Type>&& list
)
{
    if
    (
        FOAM_UNLIKELY
        (
            static_cast<const UList<Type>*>(this)
         == static_cast<const UList<Type>*>(&list)
        )
    )
    {
        FatalErrorInFunction
            << "Attempted push_back to self"
            << abort(FatalError);
    }

    const label idx = List<Type>::size();
    resize(idx + list.size());

    // Move the elements
    std::move(list.begin(), list.end(), this->begin(idx));

    list.clear();
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::pop_back(label n)
{
    if (n >= this->size())
    {
        this->clear();
    }
    else if (n > 0)
    {
        resize(this->size() - n);
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline Type& Foam::DynamicField<Type, SizeMin>::operator()
(
    const label i
)
{
    if (i >= List<Type>::size())
    {
        resize(i + 1);
    }

    return this->operator[](i);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    const Type& val
)
{
    UList<Type>::operator=(val);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    Foam::zero
)
{
    UList<Type>::operator=(Foam::zero{});
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    const UList<Type>& list
)
{
    doAssignDynList(list);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    const DynamicField<Type, SizeMin>& list
)
{
    if (this == &list)
    {
        return;  // Self-assignment is a no-op
    }

    doAssignDynList(list);
}


template<class Type, int SizeMin>
template<class Addr>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    const IndirectListBase<Type, Addr>& list
)
{
    // NOTE: Self-assignment needs special handling
    /// if
    /// (
    ///     static_cast<const UList<Type>*>(this)
    ///  == static_cast<const UList<Type>*>(&list.values())
    /// )

    doAssignDynList(list);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    List<Type>&& list
)
{
    this->transfer(list);
}


template<class Type, int SizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    DynamicField<Type, SizeMin>&& list
)
{
    this->transfer(list);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    DynamicField<Type, AnySizeMin>&& list
)
{
    this->transfer(list);
}


template<class Type, int SizeMin>
template<int AnySizeMin>
inline void Foam::DynamicField<Type, SizeMin>::operator=
(
    DynamicList<Type, AnySizeMin>&& list
)
{
    this->transfer(list);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type, int SizeMin>
inline Foam::Istream& Foam::DynamicField<Type, SizeMin>::readList
(
    Istream& is
)
{
    // Use DynamicList::readList for reading DynamicField.
    // The logic should be the same and this avoids duplicate code

    DynamicList<Type, SizeMin> list;
    this->swap(list);

    list.readList(is);
    this->swap(list);

    return is;
}


template<class Type, int SizeMin>
inline Foam::Istream& Foam::operator>>
(
    Istream& is,
    DynamicField<Type, SizeMin>& rhs
)
{
    return rhs.readList(is);
}


template<class Type, int SizeMin>
inline Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const DynamicField<Type, SizeMin>& rhs
)
{
    os << static_cast<const Field<Type>&>(rhs);
    return os;
}


// ************************************************************************* //
